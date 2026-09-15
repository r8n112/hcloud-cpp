# hcloud-cpp

A low-level **C++23** client for the Hetzner Cloud API.

Hetzner publishes official clients for Go and Python but none for C++. This
project fills that gap with a small, dependency-free core: authenticated request
construction over a pluggable transport, `std::expected` errors, and no
exceptions in the hot path.

## Status

Early (`0.1.0`). Implemented: `CloudClient` (base URL, bearer auth, `GET`/`POST`),
the `Transport` interface, and API error mapping. Typed models and a libcurl
transport are tracked in [BACKLOG.md](BACKLOG.md).

## Design

- **Testable by construction.** `Transport` is an interface; tests provide an
  in-memory implementation, so the suite never touches the network.
- **`std::expected` for recoverable errors**; the core throws nothing.
- **Strict tooling.** Warnings are errors (`cmake/CompilerWarnings.cmake`);
  CI builds with ASan/UBSan across GCC/Clang.
- **No dependencies yet.** HTTP/JSON libraries arrive with the tasks that need
  them, with justification.

## Usage

```cpp
#include <memory>
#include "hcloud/client.hpp"

auto client = hcloud::CloudClient::TryCreate("your-token", transport);
if (!client) { /* handle client.error() */ }

auto response = client->Get("/servers");
if (!response) { /* handle response.error() */ }
```

See `examples/example.cpp` for a runnable, network-free demonstration.

## Development

```sh
just configure   # cmake --preset dev (ASan/UBSan)
just build
just test
just lint        # clang-tidy
just release     # optimized, no sanitizers
just ci
```

On macOS, ASan currently hangs with AppleClang 17; use
`R8N112_SANITIZE=undefined` (or the `release` preset) locally. Linux CI is
unaffected.

## License

MIT OR Apache-2.0.
