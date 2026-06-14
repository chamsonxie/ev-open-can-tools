# 005 — Plugin Engine Rules

The plugin engine (`include/plugin_engine.h`) is a JSON-defined rule system for CAN frame modification. Only compiled when `ESP32_DASHBOARD` is defined.

## Limits

- Max **8 plugins**, each with max **16 rules**, each with max **16 operations**
- Max **32 filter IDs** across all plugins
- Max **20 replay count**
- Periodic interval: 10–5000ms (default 100ms)

## Operations

| Type | Enum | Description |
|------|------|-------------|
| SET_BIT | 0 | Set/clear a single bit (index 0-63) |
| SET_BYTE | 1 | Set a byte to a value with optional mask |
| OR_BYTE | 2 | OR a byte with a value |
| AND_BYTE | 3 | AND a byte with a value |
| CHECKSUM | 4 | Compute and set vehicle checksum |
| COUNTER | 5 | Increment rolling counter |
| EMIT_PERIODIC | 6 | Emit a frame at a fixed interval |

## Rule Matching

- `bus`: CH (1), VEH (2), PARTY (4), or ANY (0)
- `id`: CAN ID to match
- `id_mask`: mask for CAN ID matching
- `mux_enabled` + `mux`: MUX matching (frame.data[0] & 0x07)
- `mux_mask`: MUX mask
- `match_byte`/`match_value`/`match_mask`: byte-level pattern matching
- `replay`: re-apply operations on each matching frame (up to `replay_count`)

## GTW UDS Silencing

- State machine to suppress gateway broadcasts via UDS diagnostic sequence
- Request ID: 0x684, Response ID: 0x685
- `PLUGIN_GTW_UDS_KEEPALIVE_MS` = 2000ms (TesterPresent cadence)
- `PLUGIN_GTW_UDS_RESPONSE_TIMEOUT_MS` = 400ms
- `PLUGIN_GTW_UDS_RETRY_BACKOFF_MS` = 5000ms
- Seed/key security access (max 6 seeds)
- Custom key byte via `PLUGIN_GTW_UDS_KEY_READY` define (default 0xAA)

## Plugin Storage

- Plugins stored as JSON in SPIFFS files (`/plugins/plugin_N.json`)
- Validated on load and before save
- Dashboard UI provides plugin editor
