#!/usr/bin/env python3
"""Tesla CAN 录制翻译器 — my-dbc-sign.csv 定制版

支持的 7 个 CAN ID（含所有信号）:
  0x118 DI_systemStatus:  档位/油门/动能回收
  0x155 ESP_B:            车速/静止标志
  0x129 SCCM_steering:    方向盘转角/角速度
  0x311 UI_warning:       安全带/转向灯/车门/远光
  0x39B DAS_status:       盲区/碰撞预警/车道偏离/超速警告
   0x107 IBST_status:      制动踏板行程/制动状态
  0x3F5 VCFRONT_lighting: 全部23个灯光信号

用法:
  python3 scripts/translate_can_records.py
  python3 scripts/translate_can_records.py --files "can-records/*.csv" --all-frames
"""

import argparse
import csv
import glob
import os
import sys
from typing import Any, Dict, Iterable, List, Optional, Tuple

GEAR_MAP = {0: "无效", 1: "P", 2: "R", 3: "N", 4: "D", 7: "SNA"}
LIGHT_MAP = {0: "关", 1: "开", 2: "故障", 3: "无效"}
REQ_MAP = {0: "关", 1: "低有效", 2: "高有效"}
HAZARD_REQ_MAP = {
    0: "无", 1: "按钮", 2: "闭锁", 3: "解锁",
    4: "误锁", 5: "碰撞", 6: "报警", 7: "DAS", 8: "诊断",
}
BLIND_SPOT_MAP = {0: "无警告", 1: "1级", 2: "2级", 3: "无效"}
LANE_DEP_MAP = {0: "无", 1: "左", 2: "右", 3: "左严重", 4: "右严重"}
SIDE_COLL_MAP = {0: "无", 1: "左", 2: "右", 3: "左右"}
FCW_MAP = {0: "无", 1: "预警", 3: "无效"}
BRAKE_APPLY_MAP = {0: "未初始化", 1: "未制动", 2: "制动中", 3: "故障"}


def _b(row: Dict[str, str], i: int) -> int:
    return int(row.get(f"b{i}", 0))


def _extract_intel(data, start, length):
    """Extract bits in Intel (little-endian) layout."""
    v = 0
    for i in range(length):
        b = start + i
        if data[b // 8] & (1 << (b % 8)):
            v |= (1 << i)
    return v


def _extract_motorola(data, start, length):
    """Extract bits in Motorola (big-endian) layout."""
    v = 0
    for i in range(length):
        m = start + i
        physBit = (m // 8) * 8 + (7 - (m % 8))
        if data[physBit // 8] & (1 << (physBit % 8)):
            v |= (1 << i)
    return v


def bytes_from_row(row, n=8):
    return [_b(row, i) for i in range(n)]


def decode_118(row):
    """0x118 DI_systemStatus"""
    b = bytes_from_row(row)
    if len(b) < 5:
        return {}
    gear_raw = (b[2] >> 5) & 0x07
    accel = b[4] * 0.4
    regen = (b[3] >> 2) & 0x01
    parked = gear_raw in (0, 1, 7)
    return {
        "gear": GEAR_MAP.get(gear_raw, str(gear_raw)),
        "gear_raw": gear_raw,
        "parked": parked,
        "accel_pct": round(accel, 1),
        "regen": regen,
    }


def decode_155(row):
    """0x155 ESP_B"""
    b = bytes_from_row(row)
    if len(b) < 6:
        return {}
    rawSpeed = _extract_intel(b, 42, 10)
    kph = -1 if rawSpeed == 1023 else rawSpeed * 0.5
    still = _extract_intel(b, 41, 1) != 0
    return {
        "speed_kph": round(kph, 1) if kph >= 0 else None,
        "standstill": still,
    }


def decode_129(row):
    """0x129 SCCM_steeringAngleSensor"""
    b = bytes_from_row(row)
    if len(b) < 6:
        return {}
    angle = _extract_intel(b, 16, 14) * 0.1 - 819.2
    speed = _extract_intel(b, 32, 14) * 0.5 - 4096.0
    return {
        "steering_angle": round(angle, 1),
        "steering_speed": round(speed),
    }


def decode_311(row):
    """0x311 UI_warning"""
    b = bytes_from_row(row)
    if len(b) < 7:
        return {}
    buckle = _extract_motorola(b, 13, 1) != 0
    leftBlink = _extract_motorola(b, 25, 2)
    rightBlink = _extract_intel(b, 26, 2)
    door = _extract_motorola(b, 28, 1) != 0
    hiBeam = _extract_motorola(b, 50, 1) != 0
    return {
        "buckle": "扣" if buckle else "未扣",
        "left_blinker": leftBlink,
        "right_blinker": rightBlink,
        "door_open": door,
        "high_beam": hiBeam,
    }


def decode_39b(row):
    """0x39B DAS_status"""
    b = bytes_from_row(row)
    if len(b) < 5:
        return {}
    bsL = _extract_intel(b, 4, 2)
    bsR = _extract_intel(b, 6, 2)
    ssw = _extract_intel(b, 13, 1)
    fcw = _extract_intel(b, 22, 2)
    scw = _extract_intel(b, 32, 2)
    ldw = _extract_intel(b, 37, 3)
    return {
        "blind_spot_l": BLIND_SPOT_MAP.get(bsL, str(bsL)),
        "blind_spot_r": BLIND_SPOT_MAP.get(bsR, str(bsR)),
        "fcw": FCW_MAP.get(fcw, str(fcw)),
        "scw": SIDE_COLL_MAP.get(scw, str(scw)),
        "ldw": LANE_DEP_MAP.get(ldw, str(ldw)),
        "suppress_speed_warning": ssw,
    }


def decode_39d(row):
    """0x107 IBST_status"""
    b = bytes_from_row(row)
    if len(b) < 3:
        return {}
    brakeApply = _extract_intel(b, 16, 2)
    rodRaw = _extract_intel(b, 21, 12)
    rodMM = rodRaw * 0.015625 - 5.0
    return {
        "brake_apply": BRAKE_APPLY_MAP.get(brakeApply, str(brakeApply)),
        "brake_rod_mm": round(rodMM, 1),
    }


def decode_3f5(row):
    """0x3F5 VCFRONT_lighting (23 signals)"""
    b = bytes_from_row(row)
    if len(b) < 8:
        return {}
    return {
        "indicator_left": REQ_MAP.get(b[0] & 0x03, "?"),
        "indicator_right": REQ_MAP.get((b[0] >> 2) & 0x03, "?"),
        "hazard_req": HAZARD_REQ_MAP.get((b[0] >> 4) & 0x0F, "?"),
        "ambient_brightness": b[1],
        "switch_brightness": b[2],
        "approach_light": (b[3] >> 1) & 0x01,
        "see_you_home": (b[3] >> 2) & 0x01,
        "hazard_backlight": (b[3] >> 3) & 0x01,
        "low_beam_l": LIGHT_MAP.get((b[3] >> 4) & 0x03, "?"),
        "low_beam_r": LIGHT_MAP.get((b[3] >> 6) & 0x03, "?"),
        "high_beam_l": LIGHT_MAP.get(b[4] & 0x03, "?"),
        "high_beam_r": LIGHT_MAP.get((b[4] >> 2) & 0x03, "?"),
        "drl_l": LIGHT_MAP.get((b[4] >> 4) & 0x03, "?"),
        "drl_r": LIGHT_MAP.get((b[4] >> 6) & 0x03, "?"),
        "fog_l": LIGHT_MAP.get(b[5] & 0x03, "?"),
        "fog_r": LIGHT_MAP.get((b[5] >> 2) & 0x03, "?"),
        "side_repeater_l": LIGHT_MAP.get((b[5] >> 6) & 0x03, "?"),
        "side_repeater_r": LIGHT_MAP.get(b[6] & 0x03, "?"),
        "turn_signal_l": LIGHT_MAP.get((b[6] >> 2) & 0x03, "?"),
        "turn_signal_r": LIGHT_MAP.get((b[6] >> 4) & 0x03, "?"),
        "park_l": LIGHT_MAP.get((b[6] >> 6) & 0x03, "?"),
        "park_r": LIGHT_MAP.get(b[7] & 0x03, "?"),
        "high_beam_switch": (b[7] >> 2) & 0x01,
    }


def fmt_state(s: Dict[str, Any]) -> str:
    parts = []

    if "speed_kph" in s:
        speed = s.get("speed_kph")
        if speed is not None:
            still = " 静止" if s.get("standstill") else ""
            parts.append(f"车速:{speed}km/h{still}")
        else:
            parts.append("车速:无效")

    if "gear" in s:
        parts.append(f"档位:{s['gear']}{' 驻车' if s.get('parked') else ' 非驻车'}")
    if "accel_pct" in s:
        parts.append(f"油门:{s['accel_pct']}%")
    if "regen" in s:
        parts.append(f"动能回收:{'回收中' if s['regen'] else '关闭'}")

    if "steering_angle" in s:
        parts.append(f"方向盘:{s['steering_angle']}°")
    if "steering_speed" in s:
        parts.append(f"角速度:{s['steering_speed']}°/s")

    if "buckle" in s:
        parts.append(f"安全带:{s['buckle']}")
    if "left_blinker" in s:
        parts.append(f"左转:{'闪烁' if s['left_blinker'] else '关'}")
    if "right_blinker" in s:
        parts.append(f"右转:{'闪烁' if s['right_blinker'] else '关'}")
    if "door_open" in s:
        parts.append(f"门:{'开' if s['door_open'] else '关'}")
    if "high_beam" in s:
        parts.append(f"远光:{'开' if s['high_beam'] else '关'}")

    if "blind_spot_l" in s:
        parts.append(f"盲区:L={s['blind_spot_l']} R={s['blind_spot_r']}")
    if "fcw" in s:
        parts.append(f"前碰:{s['fcw']}")
    if "scw" in s:
        parts.append(f"侧碰:{s['scw']}")
    if "ldw" in s:
        parts.append(f"车道偏离:{s['ldw']}")
    if "suppress_speed_warning" in s:
        parts.append(f"超速警告:{'抑制' if s['suppress_speed_warning'] else '正常'}")

    if "brake_apply" in s:
        parts.append(f"制动:{s['brake_apply']}")
    if "brake_rod_mm" in s:
        parts.append(f"踏板行程:{s['brake_rod_mm']}mm")

    if "turn_signal_l" in s and "turn_signal_r" in s:
        parts.append(f"转向灯状态:L={s['turn_signal_l']} R={s['turn_signal_r']}")
    if "low_beam_l" in s:
        parts.append(f"近光:L={s['low_beam_l']} R={s['low_beam_r']}")
    if "high_beam_l" in s:
        parts.append(f"远光:L={s['high_beam_l']} R={s['high_beam_r']}")
    if "drl_l" in s:
        parts.append(f"DRL:L={s['drl_l']} R={s['drl_r']}")
    if "park_l" in s:
        parts.append(f"驻车灯:L={s['park_l']} R={s['park_r']}")
    if "hazard_req" in s and s['hazard_req'] != "无":
        parts.append(f"危险灯请求:{s['hazard_req']}")

    return "  ".join(parts) if parts else "(无解析数据)"


def significant_keys(s: Dict[str, Any]) -> Tuple:
    return (
        s.get("gear_raw"),
        s.get("parked"),
        s.get("speed_kph"),
        s.get("standstill"),
        s.get("door_open"),
        s.get("high_beam"),
        s.get("brake_apply"),
        s.get("steering_angle"),
        s.get("left_blinker"),
        s.get("right_blinker"),
    )


DECODERS = {
    0x118: decode_118,
    0x155: decode_155,
    0x129: decode_129,
    0x311: decode_311,
    0x39B: decode_39b,
    0x107: decode_39d,
    0x3F5: decode_3f5,
}


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
                decoder = DECODERS.get(iid)
                if decoder is None:
                    continue

                dec = decoder(row)
                sig = significant_keys(dec)
                if changes_only and sig == last_sig:
                    continue
                if min_interval_ms > 0 and (ts - last_t) < min_interval_ms:
                    continue

                rel_ms = ts - (base_ts or ts)
                state_str = fmt_state(dec)
                if not state_str:
                    continue

                line = f"[{rel_ms:7d} ms] 0x{iid:03X} {state_str}"
                lines.append(line)
                last_sig = sig
                last_t = ts
    except Exception as e:
        lines.append(f"解析失败: {e}")
    return lines


def main() -> None:
    ap = argparse.ArgumentParser(description="Tesla CAN 录制翻译器（my-dbc-sign 定制版）")
    ap.add_argument(
        "--files",
        default="can-records/can_recording*.csv",
        help="录制 CSV 路径 glob",
    )
    ap.add_argument("--changes-only", action="store_true", default=True, help="仅在关键状态变化时输出（默认开启）")
    ap.add_argument("--all-frames", dest="changes_only", action="store_false", help="输出每个可解析帧")
    ap.add_argument("--interval", type=int, default=0, help="最小输出间隔（毫秒），0 表示不限制")
    args = ap.parse_args()

    files = sorted(glob.glob(args.files))
    if not files:
        print(f"未找到录制文件: {args.files}", file=sys.stderr)
        sys.exit(1)

    print("Tesla CAN 信号翻译器（my-dbc-sign 定制版）")
    print("支持的 7 个 CAN ID: 0x118/155/129/311/39B/39D/3F5\n")

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
