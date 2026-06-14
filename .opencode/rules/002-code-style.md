# 002 — Code Style & Conventions

## C++ Formatting (`.clang-format`)

```
BasedOnStyle: LLVM
IndentWidth: 4
UseTab: Never
ColumnLimit: 0
BreakBeforeBraces: Allman
AllowShortFunctionsOnASingleLine: All
SortIncludes: false
AccessModifierOffset: -4
```

- **Allman braces** (braces on next line)
- **Spaces only** (no tabs)
- **No column limit**
- **Includes are NOT sorted** — keep existing order
- Run `clang-format --style=file` before committing

## Naming Conventions

- **Classes/Structs**: PascalCase (`CarManagerBase`, `CanFrame`, `PluginOp`)
- **Functions**: camelCase (`setBit`, `readMuxID`, `computeVehicleChecksum`)
- **Variables**: camelCase (`frameCount`, `speedProfile`, `appDriver`)
- **Macros/Defines**: UPPER_SNAKE_CASE (`PLUGIN_MAX`, `CAN_BUS_DEFAULT`, `DRIVER_TWAI`)
- **Enums**: PascalCase enum class or plain enum with PascalCase values
- **Namespaces**: lowercase (`can`, not used in this project; all code is global inline)

## Project Conventions

- **`#pragma once`** for header guards (no `#ifndef` guards)
- **`inline`** for free functions/variables in headers (ODR-safe)
- **`static`** for file-local globals in `.cpp` files
- **`if constexpr`** for compile-time branching in templates
- **No trailing return type** syntax unless necessary
- **Null pointer**: use `nullptr` (not `NULL` or `0`)
- **Integer literals**: use `0` for null, `0U` for unsigned zero
- **Prefer `uint8_t` bitfields** over bitfields (`: 1`) for portability
- **CAN frame data**: always 8 bytes (`uint8_t data[8]`)

## Python Style

- Follow PEP 8 (enforced by ruff)
- Use `argparse` for CLI scripts
- `unittest.TestCase` for tests
- No type annotations required (Python 3.8+ compatible)
