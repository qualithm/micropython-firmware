#!/usr/bin/env bash
# Run one of ci.yaml's micropython-firmware jobs inside the pinned ESP-IDF
# image, the way CI runs it: .claude/checks.json passes this script to
# `docker run` with the repo mounted at /project.
#
# Synced by `dx chat sync` from dx/chat-templates/checks/; change the template, not this file.
#
# Usage: bash .claude/checks/micropython-firmware.sh format|lint|host-test|unix-port|build BOARD
# format and lint read the NUL-separated files to check on stdin.

set -euo pipefail

# The pins ci.yaml sets in env:; dx config check holds them equal.
MICROPYTHON_VERSION=1.29.0
MICROPYTHON_SHA256=d925a7c664e79a2bdf3dfcb285ba5e2237041cc35a0bd4ee573b6c5711efeca0
mp=".micropython/micropython-$MICROPYTHON_VERSION"

# Fetch the release tarball once into .micropython/ (gitignored), check its
# checksum, and unpack it without the libraries of ports this never builds.
fetch() {
    if [[ -f "$mp/.fetched" ]]; then
        return
    fi
    local tarball="$mp.tar.xz"
    mkdir -p .micropython
    curl -fsSL --retry 5 --retry-all-errors --retry-delay 5 -o "$tarball" \
        "https://github.com/micropython/micropython/releases/download/v$MICROPYTHON_VERSION/micropython-$MICROPYTHON_VERSION.tar.xz"
    echo "$MICROPYTHON_SHA256  $tarball" | sha256sum -c -
    rm -rf "$mp"
    tar -xJf "$tarball" -C .micropython \
        --exclude='*/lib/stm32lib' --exclude='*/lib/nrfx' --exclude='*/lib/btstack' \
        --exclude='*/lib/CMSIS_5' --exclude='*/lib/CMSIS_6' --exclude='*/lib/nxp_driver' \
        --exclude='*/lib/psoc-edge' --exclude='*/lib/alif*' --exclude='*/lib/fsp' \
        --exclude='*/lib/asf4' --exclude='*/lib/pico-sdk'
    rm "$tarball"
    touch "$mp/.fetched"
}

case "${1:-}" in
format)
    pip install --quiet clang-format==22.1.8 ruff==0.17.1 < /dev/null
    xargs -0 --no-run-if-empty clang-format --dry-run --Werror
    ruff format --check modules tests
    ;;
lint)
    pip install --quiet clang-tidy==22.1.8 ruff==0.17.1 < /dev/null
    ruff check modules tests
    cmake -S test/host -B build/host > /dev/null < /dev/null
    xargs -0 --no-run-if-empty clang-tidy -p build/host --quiet
    ;;
host-test)
    pip install --quiet gcovr==8.6 coverage==7.16.2
    cmake -S test/host -B build/host > /dev/null
    cmake --build build/host
    ./build/host/host_test
    gcovr --root . --filter cmodules/ --print-summary --fail-under-line 80 build/host
    PYTHONPATH=tests/fakes:modules coverage run --source=modules -m unittest discover -s tests
    coverage report --fail-under=80
    ;;
unix-port)
    fetch
    make -s -C "$mp/mpy-cross" -j "$(nproc)"
    make -s -C "$mp/ports/unix" -j "$(nproc)" MICROPY_PY_FFI=0
    stdlib="$mp/lib/micropython-lib/python-stdlib"
    mkdir -p "$mp/test-lib/unittest"
    cp "$stdlib/unittest/unittest/__init__.py" "$stdlib/unittest-discover/unittest/__main__.py" \
        "$mp/test-lib/unittest/"
    cp "$stdlib/argparse/argparse.py" "$stdlib/fnmatch/fnmatch.py" "$mp/test-lib/"
    "$mp/ports/unix/build-standard/micropython" -c 'import sys; print(sys.version)' \
        | grep "MicroPython v$MICROPYTHON_VERSION"
    MICROPYPATH="tests/fakes:modules:$mp/test-lib:.frozen" \
        "$mp/ports/unix/build-standard/micropython" -m unittest discover -s tests
    ;;
build)
    board="${2:?usage: build BOARD}"
    fetch
    make -s -C "$mp/mpy-cross" -j "$(nproc)"
    # Into the port's own build-<board>/: make passes BUILD on to its mpy-cross sub-make.
    make -C "$mp/ports/esp32" BOARD="$board" BOARD_DIR="$PWD/boards/$board" \
        USER_C_MODULES="$PWD/cmodules/micropython.cmake"
    echo "image: $mp/ports/esp32/build-$board/firmware.bin"
    ;;
*)
    echo "usage: $0 format|lint|host-test|unix-port|build BOARD" >&2
    exit 2
    ;;
esac
