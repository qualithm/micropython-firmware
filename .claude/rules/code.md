# MicroPython Firmware Code Guidelines

A MicroPython image on ESP-IDF, built by MicroPython's `ports/esp32` make flow from one pinned
release. clang-format, clang-tidy (`.clang-tidy`), ruff and the tests enforce style and most
bug-prone patterns. Fix what they flag; this file covers what they can't.

## Layout

- `modules/` is frozen into the image by `manifest.py`. Its tests sit in `tests/` as
  `test_<module>.py`, with fakes of board-only modules (`machine`, `network`) in `tests/fakes/`.
- `cmodules/<name>/` is one user C module: plain C logic with a header, which `test/host/` tests,
  and a thin `mod<name>.c` binding that parses arguments, calls the logic and raises.
- `boards/<BOARD>/` builds on MicroPython's board of that name and includes `boards/firmware.cmake`,
  which sets the name, frozen manifest and version for every board. Never patch the fetched tree.
- `version.txt` is the version; never set `PROJECT_VER` or the board name anywhere else.

## Python

- Import only what a board has: MicroPython's core and board modules. No `dataclasses`, `typing`,
  `enum`, `abc` or `logging` at run time.
- No import-time side effects: frozen modules import at boot. Take hardware objects as arguments.
- Tests use `unittest.TestCase` and `self.assert*`, so CPython and the Unix port run the same files.
  No pytest or `unittest.mock`; inject fakes.

## C

- C11. Logic returns `0` or a negative code its header documents, and writes results through
  out-parameters. No heap allocation; take caller buffers with a size, and use `snprintf`.
- A binding raises a MicroPython exception per error code, lowercase with no trailing punctuation.

## Security

Release images are unsigned. Never commit a signing key, an eFuse key or a device credential, and
never add a signing step to a template or example repo.

## When code changes

A behavior change carries tests covering its new lines: Host Tests gates `cmodules/` and `modules/`
at 80% each.

## Generated files

`.github/workflows/` and `.claude/checks/` come from dx; change the template and sync from dx, never
edit them here. `CI Required` and `PR Title` are required.
