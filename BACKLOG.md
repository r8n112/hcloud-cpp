# Backlog

Self-contained tasks for `hcloud-cpp`, ordered by priority. Pick the first task
whose status is `todo`, satisfy every acceptance criterion, then mark it `done`.

Already implemented: `CloudClient` (`TryCreate`, `Get`, `Post`), bearer auth,
base-URL normalisation, `Transport` interface, API error mapping.

## T001 — libcurl `Transport` implementation

- Status: todo
- Depends on: none

Add a real `Transport` backed by libcurl, discoverable with
`find_package(CURL)`. It must be optional: if CURL is not found, the library
still builds and the tests still pass.

Acceptance criteria:
- [ ] `CurlTransport` implements `hcloud::Transport`
- [ ] CMake option (for example `HCLOUD_CPP_WITH_CURL`) gates it; default OFF
- [ ] a CMake build with it ON compiles in CI (add `libcurl4-openssl-dev`)
- [ ] no network access from the test suite

## T002 — JSON layer and typed `Server` model

- Status: todo
- Depends on: T001 (optional)

Choose a JSON library (justify the choice) and add a typed `Server` model plus a
`Servers::List` call that returns `std::vector<Server>`.

Acceptance criteria:
- [ ] the dependency is justified in the PR/commit message
- [ ] a failing-first test parses a representative `{"servers":[...]}` fixture
- [ ] malformed JSON yields `ErrorCode::kApi` or a dedicated parse error
- [ ] `just ci` green

## T003 — Timeouts, retry and rate-limit handling

- Status: todo
- Depends on: T001

Add a configurable timeout, a single bounded retry for transient transport
failures, and explicit handling of HTTP 429 (rate limit) with a clear error.

Acceptance criteria:
- [ ] timeout is configurable and documented
- [ ] retry behaviour proven via the fake transport
- [ ] 429 surfaces a distinct, documented error
- [ ] `just ci` green

## T004 — Pagination helper

- Status: todo
- Depends on: T002

Hetzner Cloud list endpoints support `page`/`per_page` and return `meta`
pagination. Provide an iterator/helper that walks all pages.

Acceptance criteria:
- [ ] a failing-first test covers multi-page traversal using fixtures
- [ ] the helper stops on the last page and preserves order
- [ ] `just ci` green

## T005 — CLI binary

- Status: todo
- Depends on: T002

Add a thin `hcloud-cpp` CLI (token from `HCLOUD_TOKEN`) exposing a couple of
read-only commands (for example `servers list`).

Acceptance criteria:
- [ ] binary builds and a test drives it with a fake/injected transport
- [ ] token is never logged
- [ ] `just ci` green

## T006 — clang-tidy in CI + compiler matrix note

- Status: todo
- Depends on: none

`just lint` runs clang-tidy but CI does not. Add a `clang-tidy` job and document
the supported compiler versions.

Acceptance criteria:
- [ ] CI runs `clang-tidy` over `src` and `examples`
- [ ] README documents the compiler matrix
- [ ] `just ci` green
