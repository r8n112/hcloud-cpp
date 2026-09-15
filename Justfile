set shell := ["bash", "-eu", "-o", "pipefail", "-c"]

sources := "src include tests examples"
default: ci

configure:
    cmake --preset dev

build:
    cmake --build --preset dev

test:
    ctest --preset dev

fmt:
    find {{sources}} -type f \( -name '*.cpp' -o -name '*.hpp' \) -print0 | xargs -0 clang-format -i
    cmake-format -i CMakeLists.txt cmake/*.cmake tests/CMakeLists.txt examples/CMakeLists.txt 2>/dev/null || true

fmt-check:
    find {{sources}} -type f \( -name '*.cpp' -o -name '*.hpp' \) -print0 | xargs -0 clang-format --dry-run --Werror

lint:
    find src examples -type f \( -name '*.cpp' -o -name '*.hpp' \) -print0 | xargs -0 clang-tidy -p build/dev

release:
    cmake --preset release && cmake --build --preset release && ctest --preset release

ci: fmt-check build test lint

clean:
    rm -rf build
