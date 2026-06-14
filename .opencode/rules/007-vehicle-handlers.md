# 007 — Tesla Vehicle Handler Details

## CAN ID Reference

| ID (hex) | ID (dec) | Name | Used By |
|----------|----------|------|---------|
| 0x045 | 69 | DI_torqueDriver | Legacy |
| 0x118 | 280 | DI_systemStatus | All (gear, AP status) |
| 0x186 | 390 | DI_autonomyControl | All (ACA, follow distance) |
| 0x399 | 921 | GTW_autopilot | All (AP state, speed profile) |
| 0x3EE | 1006 | UI_selfParkRequest | Legacy (summon) |
| 0x3F8 | 1016 | UI_selfParkRequest | HW3/HW4 (summon) |
| 0x3FD | 1021 | UI_selfParkRequest (alt) | HW3/HW4 |
| 0x370 | 880 | SteeringColumn | NagHandler (nag suppression) |
| 0x7FF | 2047 | DI_systemStatus (alt) | HW3/HW4 |

## CAN 280 (DI_systemStatus) — Gear + Autopilot

- Byte 2 bits 5-7: DI_gear (0=INVALID, 1=P, 2=R, 3=N, 4=D, 7=SNA)
- `isVehicleParked()`: returns true for gear 0, 1, or 7
- Autopilot status fields in byte 0

## CAN 921 (GTW_autopilot) — Speed Profile Injection

- Byte 5 bits 2-4: GTW autopilot state (0=NONE, 1=HIGHWAY, 2=ENHANCED, 3=SELF_DRIVING, 4=BASIC)
- Speed profile set via `setSpeedProfileV12V13()` (byte 6 bits 1-2):
  - Profile 0: 1 car length
  - Profile 1: 2 car lengths (default)
  - Profile 2: 3 car lengths
  - Profile 3: 4 car lengths
- HW4 uses `setSpeedProfileHW4()` (byte 7 bits 4-6) with 5 profiles (0-4)

## CAN 390 (DI_autonomyControl) — Follow Distance → Profile

- MUX in byte 0 bits 0-2
- MUX 0: ACA state, follow distance in byte 7
- Follow distance: 1=1car, 2=2car, 3=3car, 4=4car (plus 5-7 for HW4)
- MUX 1: DAS autopilot status (byte 0 bits 0-3): 3-5 = active

## CAN 880 (SteeringColumn) — Nag Suppression

- `NagHandler` target
- Echo with counter+1 and checksum fix
- `handsOnLevel` check: byte 3 bits 5-6 must be 0b10
- Checksum at byte 7 (standard `computeVehicleChecksum`)
- Filter IDs: only 0x370 (880)

## CAN Bus Mask Enum

```cpp
CAN_BUS_ANY   = 0   (match any bus)
CAN_BUS_CH    = 1   (chassis bus)
CAN_BUS_VEH   = 2   (vehicle bus)
CAN_BUS_PARTY = 4   (party bus)
```

Default bus: `CAN_BUS_ANY` (set to `CAN_BUS_DEFAULT` at frame entry point)

## Feature-to-Handler Mapping

| Build Define | Handler | Vehicle |
|---|---|---|
| `LEGACY` | LegacyHandler | MCU1 (Model S/X pre-2021) |
| `HW3` | HW3Handler | Model 3/Y with HW3 |
| `HW4` | HW4Handler | Model 3/Y with HW4 / Cybertruck |
| `NAG_KILLER` | NagHandler | Any (nag only, no speed injection) |
| `ESP32_DASHBOARD` | Selectable via UI | Any (default HW3) |
