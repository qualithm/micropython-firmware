# Micropython Firmware

[![CI](https://github.com/qualithm/micropython-firmware/actions/workflows/ci.yaml/badge.svg)](https://github.com/qualithm/micropython-firmware/actions/workflows/ci.yaml)
[![codecov](https://codecov.io/gh/qualithm/micropython-firmware/graph/badge.svg)](https://codecov.io/gh/qualithm/micropython-firmware)
[![Release](https://img.shields.io/github/v/release/qualithm/micropython-firmware?display_name=tag&sort=semver)](https://github.com/qualithm/micropython-firmware/releases/latest)

MicroPython firmware images for ESP32 boards, with the Qualithm device SDK frozen in.

The Qualithm CLI flashes these images to boards where stock MicroPython can't hold the SDK and a TLS
handshake in memory at once, such as the ESP32-C3.

## Features

- MicroPython 1.29.0 for the ESP32-C3 and ESP32-S3, built on ESP-IDF
- The Qualithm device SDK ([device-sdk-py](https://github.com/qualithm/device-sdk-py)) frozen into
  flash, so it runs as bytecode from flash and leaves RAM to Wi-Fi and TLS
- `qualithm_firmware.SDK_VERSION` names the frozen SDK release, so a deploy knows not to copy
  another one to `/lib`
- `firmware.version()` returns the image's own version as `(major, minor, patch)`

## Supported Boards

| Board            | Chip     |
| ---------------- | -------- |
| ESP32_GENERIC_S3 | ESP32-S3 |
| ESP32_GENERIC_C3 | ESP32-C3 |

Each board in `boards/` builds on MicroPython's board of the same name.

## Quick Start

Every step runs in the pinned ESP-IDF image through Docker. Nothing else is installed locally. The
first build fetches the pinned MicroPython release into `.micropython/`. Build one board:

```bash
docker run --rm -v "$PWD":/project -w /project espressif/idf:v5.5.5@sha256:a9231d0697ab8f7517cc072e93b7c83e04907bfbfba80b6440d7dbbf90665cf2 bash .claude/checks/micropython-firmware.sh build ESP32_GENERIC_C3
```

The build ends by printing the flashable image's path, MicroPython's merged `firmware.bin` under
`.micropython/`.

## Flashing

`qualithm board flash` flashes the image the CLI pins for your board. To flash a release image by
hand, use [esptool](https://docs.espressif.com/projects/esptool/) from the host, since Docker on
macOS can't reach a USB serial port:

```bash
pipx run esptool --chip esp32c3 -p <your-serial-port> write_flash 0x0 micropython-firmware-0.1.1-esp32c3-unsigned.bin
```

Each release attaches one image per target as
`micropython-firmware-<version>-<target>-unsigned.bin`. Connect at 115200 baud for the REPL. Its
banner names the MicroPython version, the firmware and its version.

## Configuration

- `boards/firmware.cmake` sets what every board shares: the firmware's name, its version from
  `version.txt`, and the frozen manifest.
- `manifest.py` freezes MicroPython's own modules, `modules/` and the vendored device SDK.
- `vendor/device-sdk-py/` holds the device SDK release the image freezes. `VERSION` names it. It
  matches the SDK the Qualithm CLI pins, and `modules/qualithm_firmware.py` repeats it.
- `boards/<BOARD>/` holds one board's settings.

The MicroPython version is pinned in dx, not here.

## Development

Run every check CI runs, in CI's order, with the commands in
[`.claude/rules/checks.md`](.claude/rules/checks.md):

- **Format** — `clang-format`, configured by `.clang-format`, and `ruff format`
- **Lint** — `ruff`, configured by `ruff.toml`, and `clang-tidy` on the C modules' logic, configured
  by `.clang-tidy`
- **Host Tests** — the Unity tests in `test/host/` over `cmodules/`, and the frozen modules' tests
  under CPython coverage, each with an 80% line gate
- **Test (MicroPython Unix port)** — the same tests on MicroPython itself
- **Build** — both boards

Install the pre-commit hook, which runs the same list:

```bash
git config core.hooksPath .githooks
```

## Releases

`version.txt` is the version. A semver tag that matches it (`X.Y.Z`, no `v`) builds both boards and
attaches one merged, flashable image per target with its SHA-256 to the GitHub Release. The images
are unsigned development firmware.

## License

Apache-2.0
