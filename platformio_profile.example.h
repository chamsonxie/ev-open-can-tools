#pragma once

// 本地构建配置示例。
// 将此文件复制或重命名为 platformio_profile.h，然后编辑 platformio_profile.h。
// platformio_profile.h 被 git 忽略，因此本地板卡选择和凭据
// 不会意外提交。

// ── 板卡选择 ────────────────────────────────────────────────────
// 取消注释以下行之一以匹配您的板卡：
// #define DRIVER_SAME51            // Adafruit Feather M4 CAN Express（原生 ATSAME51 CAN）
// #define DRIVER_TWAI // 带有内置 TWAI（CAN）外设的 ESP32 板卡

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
