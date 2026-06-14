#!/usr/bin/env python3
"""Tesla Model Y (HW3/HW4) CAN 录制翻译器
使用项目内 opendbc + 固件已知的 bit 布局，将 can-records/*.csv 翻译为简洁中文。

重点信号（固件实际使用的）：
- 档位 (280 DI_systemStatus byte2 bits5-7)
- 油门踏板 (280 byte4 * 0.4 %)
- 刹车状态
- 驻车判断（P / SNA / 无效）
- 召唤请求 (1016 spr, 1021 mux)
- Autopilot 状态 (2047 mux=2 GTW_autopilot)
- ACA 位（autonomy control active，用于 summon 门控）

注意：当前录制文件仅包含 280/1016/1021/2047，没有车速信号（常见车速信号在 0x116 DI_torque2 / 0x155 ESP_B 等）。
因此输出中“车速”会标注为“(录制中未包含)”。

用法示例：
  python3 scripts/translate_can_records.py
  python3 scripts/translate_can_records.py --files "can-records/can_recording*.csv" --changes-only
"""

import argparse
import csv
import glob
import os
import sys
from typing import Any, Dict, Iterable, List, Optional, Tuple

# 与 include/can_helpers.h / handlers.h 一致的映射
GEAR_MAP = {
    0: "无效",
    1: "P",
    2: "R",
    3: "N",
    4: "D",
    7: "SNA",
}

AP_MAP = {
    0: "NONE",
    1: "HIGHWAY",
    2: "ENHANCED",
    3: "SELF_DRIVING",
    4: "BASIC",
}

BRAKE_MAP = {0: "OFF", 1: "ON", 2: "INVALID"}

# 固件中用于召唤门控的关键位
# 280: byte6 bit2 = ACA
# 1016: (data[3]>>4)&0xF = spr（非0表示有召唤请求）
# 1021: byte0 & 0x07 = mux

def _b(row: Dict[str, str], i: int) -> int:
    return int(row.get(f"b{i}", 0))

def decode_280(row: Dict[str, str]) -> Dict[str, Any]:
    b2 = _b(row, 2)
    b4 = _b(row, 4)
    b6 = _b(row, 6)
    gear_raw = (b2 >> 5) & 0x07
    brake_raw = (b2 >> 3) & 0x03
    sys_state = b2 & 0x07
    accel = b4 * 0.4
    aca = bool(b6 & 0x04)
    parked = gear_raw in (0, 1, 7)
    return {
        "gear": GEAR_MAP.get(gear_raw, str(gear_raw)),
        "gear_raw": gear_raw,
        "parked": parked,
        "accel_pct": round(accel, 1),
        "brake": BRAKE_MAP.get(brake_raw, str(brake_raw)),
        "brake_raw": brake_raw,
        "sys_state": sys_state,
        "aca": aca,
    }

def decode_1016(row: Dict[str, str]) -> Dict[str, Any]:
    b3 = _b(row, 3)
    spr = (b3 >> 4) & 0x0F
    return {"summon_req": spr != 0, "spr_raw": spr}

def decode_1021(row: Dict[str, str]) -> Dict[str, Any]:
    b0 = _b(row, 0)
    mux = b0 & 0x07
    return {
        "mux": mux,
        "b3": _b(row, 3),
        "b5": _b(row, 5),
        "b6": _b(row, 6),
        "b7": _b(row, 7),
    }

def decode_2047(row: Dict[str, str]) -> Dict[str, Any]:
    b0 = _b(row, 0)
    b5 = _b(row, 5)
    mux = b0 & 0x07
    if mux != 2:
        return {"mux": mux}
    ap_raw = (b5 >> 2) & 0x07
    return {"mux": 2, "gtw_ap": AP_MAP.get(ap_raw, str(ap_raw)), "gtw_ap_raw": ap_raw}

def fmt_state(s: Dict[str, Any]) -> str:
    parts = []
    # 按用户要求风格：车速：xxx， 档位：xx ， ...
    # 本次录制未包含车速信号（常见车速在 0x116/0x155 等）
    parts.append("车速：(录制中未包含)")
    if "gear" in s:
        p = "驻车" if s.get("parked") else "非驻车"
        parts.append(f"档位：{s['gear']}（{p}）")
    if "accel_pct" in s:
        parts.append(f"油门：{s['accel_pct']}%")
    if "brake" in s:
        parts.append(f"刹车：{s['brake']}")
    if "aca" in s:
        parts.append(f"ACA：{'是' if s['aca'] else '否'}")
    if "summon_req" in s:
        parts.append(f"召唤请求：{'是' if s['summon_req'] else '否'}")
    if "gtw_ap" in s:
        parts.append(f"AP状态：{s['gtw_ap']}")
    if "mux" in s and s.get("mux") is not None and "gtw_ap" not in s:
        parts.append(f"1021_mux：{s['mux']}")
    return "，".join(parts)

def significant_keys(s: Dict[str, Any]) -> Tuple:
    # 用于变化检测的关键字段（忽略原始 raw 值和次要 mux）
    return (
        s.get("gear_raw"),
        s.get("parked"),
        round(s.get("accel_pct", 0), 1) if "accel_pct" in s else None,
        s.get("brake_raw"),
        s.get("aca"),
        s.get("summon_req"),
        s.get("gtw_ap_raw"),
        s.get("mux") if s.get("mux") != 2 else None,  # 仅当非 AP mux 时关注
    )

def process_csv(path: str, changes_only: bool = True, min_interval_ms: int = 0) -> List[str]:
    lines: List[str] = []
    last_sig: Optional[Tuple] = None
    last_t = 0
    base_ts: Optional[int] = None

    try:
        with open(path, newline="") as fh:
            reader = csv.DictReader(fh)
            for row in reader:
                try:
                    ts = int(row.get("ts_ms", 0))
                except Exception:
                    continue
                if base_ts is None:
                    base_ts = ts
                iid = int(row.get("id", 0))
                if iid == 0x118:
                    dec = decode_280(row)
                elif iid == 0x3f8:
                    dec = decode_1016(row)
                elif iid == 0x3fd:
                    dec = decode_1021(row)
                elif iid == 0x7ff:
                    dec = decode_2047(row)
                else:
                    continue

                sig = significant_keys(dec)
                if changes_only and sig == last_sig:
                    continue
                if min_interval_ms > 0 and (ts - last_t) < min_interval_ms:
                    continue

                rel_ms = ts - (base_ts or ts)
                state_str = fmt_state(dec)
                if not state_str:
                    continue

                line = f"[{rel_ms:7d} ms] {state_str}"
                lines.append(line)
                last_sig = sig
                last_t = ts
    except Exception as e:
        lines.append(f"解析失败: {e}")
    return lines

def main() -> None:
    ap = argparse.ArgumentParser(description="Tesla CAN 录制信号翻译器（中文简洁输出）")
    ap.add_argument(
        "--files",
        default="can-records/can_recording*.csv",
        help="录制 CSV 路径 glob（默认 can-records/can_recording*.csv）",
    )
    ap.add_argument("--changes-only", action="store_true", default=True, help="仅在关键状态变化时输出（默认开启）")
    ap.add_argument("--all-frames", dest="changes_only", action="store_false", help="输出每个可解析帧（会很长）")
    ap.add_argument("--interval", type=int, default=0, help="最小输出间隔（毫秒），0 表示不限制")
    args = ap.parse_args()

    files = sorted(glob.glob(args.files))
    if not files:
        print(f"未找到录制文件: {args.files}", file=sys.stderr)
        sys.exit(1)

    print("Tesla Model Y CAN 信号翻译器")
    print("来源: opendbc + 固件 bit 布局 (280=DI_systemStatus, 1016/1021=自泊车, 2047=GTW_AP)")
    print("注意: 录制仅含 280/1016/1021/2047，无车速帧（常见车速信号 0x116 DI_torque2 / 0x155 ESP_B）。\n")

    for f in files:
        print(f"=== {os.path.basename(f)} ===")
        out = process_csv(f, changes_only=args.changes_only, min_interval_ms=args.interval)
        if not out:
            print("  (无关键状态变化或无可解析帧)")
        else:
            for ln in out:
                print("  " + ln)
        print()

if __name__ == "__main__":
    main()
