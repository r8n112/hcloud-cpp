# AGENTS.md — operating instructions for automated agents

This repository follows the r8n112 engineering standard (C++23, CMake, Catch2).
If you are an autonomous coding agent, read this file in full before changing
anything.

## What this project is

`hcloud-cpp` is a low-level C++23 client for the Hetzner Cloud API: it builds
authenticated requests and hands them to a `Transport`. There is no JSON parsing
or real HTTP transport yet — those are the first items in `BACKLOG.md`.

## Non-negotiables

- **TDD.** Add a failing Catch2 test first, then the minimum implementation.
- **No undefined behaviour.** Sanitizer builds (ASan/UBSan, TSan) must stay
  green. No signed-overflow reliance, no out-of-bounds, no use-after-move.
- **Explicit ownership.** `std::unique_ptr` for owned resources; references /
  `std::span` for non-owning. No `new`/`delete` in application code.
- **`std::expected` for recoverable errors.** Exceptions only at API boundaries;
  never let one escape a `noexcept` function.
- **Tests never touch the network.** Implement `hcloud::Transport` with an
  in-memory fake (see `tests/client_test.cpp`).
- **Do not weaken warnings.** `cmake/CompilerWarnings.cmake` promotes warnings
  to errors; do not remove entries without justification.
- **Small, signed, conventional commits** with the configured persona key.

## Commands

```sh
just configure   # cmake --preset dev (ASan/UBSan)
just build
just test
just lint        # clang-tidy
just release     # optimized, no sanitizers
just ci          # format check + build + test + lint
```

Local note: with AppleClang 17, ASan hangs on this machine. Use
`-DR8N112_SANITIZE=undefined` or the `release` preset locally. Linux CI runs the
full matrix.

## Architecture

- `include/hcloud/error.hpp` — `Error`, `ErrorCode`, `ToString`.
- `include/hcloud/http.hpp` — `HttpRequest`, `HttpResponse`, `Transport`.
- `include/hcloud/client.hpp` — `CloudClient` (`TryCreate`, `Get`, `Post`).
- `src/client.cpp` — implementation (request building, auth, status mapping).
- `tests/client_test.cpp` — Catch2 tests with a `RecordingTransport`.
- `examples/example.cpp` — network-free usage example.

## Conventions

- Namespace `hcloud`; include paths `hcloud/<header>.hpp`.
- Class `CamelCase`, functions `CamelCase`, variables `lower_case`, private
  members suffixed `_`, enum values prefixed `k` (see `.clang-tidy`).
- Public headers document every type and member.

## How to add a feature

1. Read `BACKLOG.md`; pick the first `todo` task.
2. Add a failing Catch2 test under `tests/`.
3. Implement the minimum under `include/` and `src/`.
4. `just ci` until green, including the sanitizer build (or `undefined` locally).
5. Commit: `feat(<scope>): <summary>`, signed off, referencing the task id.

## Definition of done

- [ ] A test was added first and now passes.
- [ ] `just ci` is green, including a sanitizer build.
- [ ] Public headers documented; ownership and `noexcept` explicit.
- [ ] No new dependency without justification.
- [ ] Commit is signed, conventional, and references the backlog task id.

## Backlog

`BACKLOG.md` lists self-contained tasks with acceptance criteria.
