#pragma once

// 本地构建配置示例。
// 将此文件复制或重命名为 platformio_profile.h，然后编辑 platformio_profile.h。
// platformio_profile.h 被 git 忽略，因此本地板卡选择和凭据
// 不会意外提交。

// ── 板卡选择 ────────────────────────────────────────────────────
// 取消注释以下行之一以匹配您的板卡：
// #define DRIVER_MCP2515           // Adafruit Feather RP2040 CAN（通过 SPI 的 MCP2515）
// #define DRIVER_SAME51            // Adafruit Feather M4 CAN Express（原生 ATSAME51 CAN）
// #define DRIVER_TWAI // 带有内置 TWAI（CAN）外设的 ESP32 板卡
// #define DRIVER_ESP32_EXT_MCP2515 // ESP32-S3 + 通过 SPI 的外部 MCP2515（使用 esp32_ext_mcp2515 环境）

// ── 车辆硬件选择 ────────────────────────────────────────────────
// 取消注释以下行之一以匹配您的车辆：
// #define LEGACY // HW3 改装
// #define HW3 // HW3
// #define HW4    // HW4

// ── 仪表盘凭据 ──────────────────────────────────────────────────
// 所有 ESP32 仪表盘构建必需。这些是首次启动时使用的初始值；
// 通过仪表盘 WiFi 热点卡在运行时更改它们
//（持久化存储在 NVS 中，固件更新后仍然保留）。
#define DASH_SSID "chamsonsEvTools" // WiFi AP 名称
#define DASH_PASS "1024381808"      // WiFi 密码（至少 8 个字符）
#define DASH_OTA_USER "admin"       // OTA 用户名
#define DASH_OTA_PASS "changeme"    // OTA 密码

// #define DASH_INJECTION_ON_BOOT  // 启动后自动开始注入；默认为停止

// ── GTW UDS 静默密钥 ────────────────────────────────────────────
// 插件规则中 gtw_silent: true 实际静默网关所必需。
// 没有此项，gtw_silent 在 JSON 中被接受但运行时被忽略。
//
// 固件期望使用单个 XOR 字节将 GTW 种子转换为密钥。
// 如果您有已知的种子/密钥对，请在本地
// Linux 机器上验证或推导该字节：
//
// python3 - <<'PY'
// seed = bytes.fromhex("001122334455")  # 替换为捕获到的种子
// key = bytes.fromhex("350417067160")   # 替换为匹配的密钥
// if len(seed) != len(key) or not seed:
//     raise SystemExit("seed/key length mismatch")
// xor_key = seed[0] ^ key[0]
// if any((s ^ xor_key) != k for s, k in zip(seed, key)):
//     raise SystemExit("seed/key pair is not a single-byte XOR key")
// print(f"0x{xor_key:02X}")
// PY
//
// 将打印出的字节粘贴到 PLUGIN_GTW_UDS_KEY_READY 之后。
// #define PLUGIN_GTW_UDS_KEY_READY 0xAB

// ── 行为选项 ──────────────────────────────────────────────────
// 取消注释以下任意一行：
// #define ISA_SPEED_CHIME_SUPPRESS    // 抑制 ISA 速度提示音；驾驶时限速标志将显示为空
// #define EMERGENCY_VEHICLE_DETECTION // 启用紧急车辆检测
// #define BYPASS_TLSSC_REQUIREMENT    // 始终启用 drivepilot，无需"交通灯和停车标志控制"开关
// #define NAG_KILLER                  // 抑制 Autosteer"手握方向盘"提示（CAN 880 计数器+1 回显，X179 引脚 2/3）
// #define ENHANCED_AUTOPILOT          // 在 HW3/HW4 上启用 UI_applyEceR79 覆盖，并在 HW4 上启用召唤
