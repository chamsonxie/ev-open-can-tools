# Build And Flash

[Project Home](../) | [Documentation](index.md) | [Dashboard Guide](dashboard.md) | [Plugin System](plugins.md) | [Release Notes](../CHANGELOG.md)

The project is PlatformIO-only. Pick the correct board environment in `platformio.ini`, then choose the matching driver and default vehicle mode in your local `platformio_profile.h`.

## Supported PlatformIO Environments

| Env | Board / target | Notes |
| --- | --- | --- |
| `waveshare_ESP32_S3_RS485_CAN` | Waveshare ESP32-S3 RS485/CAN | TWAI dashboard build |
| `native` | Host machine | Native unit tests (Unity framework) |
| `native_dashboard` | Host machine | Dashboard mode handler tests |
| `native_log_buffer` | Host machine | Log ring buffer tests |

## Hardware Notes

Some CAN boards and adapters include an onboard 120 ohm termination resistor. When installing on an existing vehicle CAN bus, do not add another termination point: cut or remove the board's 120 ohm resistor if the adapter has one fitted.

## Selecting Driver, Vehicle, And Defaults

Create your local build config first:

```bash
cp platformio_profile.example.h platformio_profile.h
```

Then edit `platformio_profile.h`:

- choose one driver define
- choose one vehicle define
- set initial hotspot and OTA credentials
- uncomment optional feature defines when you want compile-time defaults changed

`platformio_profile.h` is ignored by git. Keep personal board choices, WiFi credentials, OTA credentials, and gateway keys there. Commit changes to `platformio_profile.example.h` only when you are changing the template for everyone.

You can also use the helper script:

```bash
python scripts/platformio_set_profile.py --driver DRIVER_TWAI --vehicle HW4
```

## Build

```bash
pio run -e waveshare_ESP32_S3_RS485_CAN
```

Replace `waveshare_ESP32_S3_RS485_CAN` with the environment you are targeting.

## Flash

```bash
pio run -e waveshare_ESP32_S3_RS485_CAN -t upload
```

For boards that need a different upload path or boot mode, use the normal PlatformIO upload flow for that board.

## First Boot

- ESP32 dashboard builds start their hotspot from `DASH_SSID` / `DASH_PASS`
- Open `http://192.168.4.1/` after connecting to the hotspot
- Change hotspot and OTA credentials after first boot
- Use the `WiFi Internet` card if you want plugin downloads or OTA updates from GitHub releases
- Use the `CAN Pins` card only on TWAI-based boards when you need non-default GPIO assignments

## Build Outputs

- ESP32-S3 builds produce a `firmware.bin`
- Manual dashboard OTA expects a `.bin` built for the exact target board
