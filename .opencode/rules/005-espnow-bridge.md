# 005 — ESP-NOW CAN 数据广播桥

## 概述

`espnow_broadcast.h` 实现了从特斯拉 CAN 总线到 ESP-NOW 无线广播的桥接。它将读取到的 CAN 帧实时转发给接收端（如 `teslaLight` 灯带项目）。

## 工作流程

1. **监听 CAN 帧**：在 `espnowUpdateFromFrame()` 中匹配 7 个 CAN ID
2. **提取信号**：从每个 CAN 帧的字节中按位提取信号值
3. **打包广播**：每 100ms 将当前信号快照打包为 53 字节结构体，通过 ESP-NOW 广播

## 监听的 CAN ID 与信号映射

| CAN ID | 信号来源 | 提取字段 |
|--------|---------|---------|
| `0x155` (341) | ESP_B | vehicleSpeed, standstill |
| `0x118` (280) | DI_systemStatus | gear, accelPedalPos, regenLight |
| `0x129` (297) | SCCM_steeringAngleSensor | steeringAngle, steeringAngleSpeed |
| `0x311` (785) | UI_warning | buckleStatus, leftBlinkerBlinking, rightBlinkerBlinking, anyDoorOpen, uiHighBeam |
| `0x39B` (923) | DAS_status | blindSpotRearLeft/Right, suppressSpeedWarning, forwardCollisionWarning, sideCollisionWarning, laneDepartureWarning |
| `0x39D` (925) | IBST_status | driverBrakeApply, brakeRodTravel |
| `0x3F5` (1013) | VCFRONT_lighting | 22 个灯光状态字段 |

## 广播配置

| 参数 | 值 |
|------|-----|
| 广播间隔 | 100ms (10 Hz) |
| 信道 | 1 |
| 目标 MAC | 广播地址 `FF:FF:FF:FF:FF:FF` |
| 包大小 | 53 字节 |
| 校验 | XOR (byte[0..51]) = byte[52] |
| 发现口令 | `"chamsonxie"` (10 字节) |

## 场景模拟

内置 `EspNowDefaultScenario` 提供 7 步驾驶循环：

1. 驻车 (2s) → 2. 驻车 (1.5s) → 3. 加速行驶 (3s) → 4. 巡航 (2s) → 5. 左转 (2.5s) → 6. 右转 (2s) → 7. 停止 (1s)

通过 Web 面板或 API 可启动场景模拟，用于测试接收端效果。

## 配对机制

- 接收端每 3 秒广播发现包（含口令）
- 发射端在 `recvCb()` 中匹配口令后将设备加入发现列表
- Web UI 支持扫描、配对、取消配对操作
- 配对成功后开始持续广播 CAN 数据

## 信号更新

`espnowUpdateFromFrame()` 在主 CAN 处理循环中被调用，每收到一条匹配的 CAN 帧就更新对应字段。最终的广播包包含所有字段的最新值。
