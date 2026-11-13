---
description:
  "Exact pre-commit commands for the micropython-firmware CI archetype, kept in sync with ci.yaml"
---

# Pre-commit Checks

This repo's `ci.yaml` is generated from `dx/ci-templates/micropython-firmware.yaml` via `dx ci sync`
(check for drift with `dx ci drift`). Every command runs one CI job's steps through
`.claude/checks/micropython-firmware.sh`, in the same pinned ESP-IDF image as CI, through Docker:

```bash
git ls-files -z "*.c" "*.h" | docker run --rm -i -v "$PWD":/project -w /project espressif/idf:v5.5.5@sha256:a9231d0697ab8f7517cc072e93b7c83e04907bfbfba80b6440d7dbbf90665cf2 bash .claude/checks/micropython-firmware.sh format   # `clang-format -i` and `ruff format` (same versions) rewrite the files
git ls-files -z "cmodules/*.c" ":!cmodules/*/mod*.c" | docker run --rm -i -v "$PWD":/project -w /project espressif/idf:v5.5.5@sha256:a9231d0697ab8f7517cc072e93b7c83e04907bfbfba80b6440d7dbbf90665cf2 bash .claude/checks/micropython-firmware.sh lint
docker run --rm -v "$PWD":/project -w /project espressif/idf:v5.5.5@sha256:a9231d0697ab8f7517cc072e93b7c83e04907bfbfba80b6440d7dbbf90665cf2 bash .claude/checks/micropython-firmware.sh host-test
docker run --rm -v "$PWD":/project -w /project espressif/idf:v5.5.5@sha256:a9231d0697ab8f7517cc072e93b7c83e04907bfbfba80b6440d7dbbf90665cf2 bash .claude/checks/micropython-firmware.sh unix-port
docker run --rm -v "$PWD":/project -w /project espressif/idf:v5.5.5@sha256:a9231d0697ab8f7517cc072e93b7c83e04907bfbfba80b6440d7dbbf90665cf2 bash .claude/checks/micropython-firmware.sh build ESP32_GENERIC_S3
docker run --rm -v "$PWD":/project -w /project espressif/idf:v5.5.5@sha256:a9231d0697ab8f7517cc072e93b7c83e04907bfbfba80b6440d7dbbf90665cf2 bash .claude/checks/micropython-firmware.sh build ESP32_GENERIC_C3
```

They match CI's jobs in order: Format, Lint, Host Tests (Unity over `cmodules/` and CPython coverage
over `modules/`, each gated at 80% of lines), Test (MicroPython Unix port), and Build for each
board.

- **The MicroPython source.** The first command that needs it fetches the pinned release tarball
  into `.micropython/` (gitignored), checks its SHA-256 and unpacks it once. Delete `.micropython/`
  to fetch again.
- **What the gates cover.** A user C module's `mod<name>.c` binding needs MicroPython's headers,
  which the host build lacks, so it sits outside clang-tidy and the C gate: keep it thin.
  MicroPython has no coverage tool, so CPython measures the same `unittest` tests the Unix port then
  runs.
- **Formatting.** `clang-format -i` and `ruff format`, at the versions the script pins.
