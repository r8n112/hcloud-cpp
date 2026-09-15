#pragma once

#include <string>
#include <string_view>

namespace hcloud {

/// Error categories reported by the client.
enum class ErrorCode : unsigned char {
    /// Invalid argument, for example an empty token.
    kInvalidArgument,
    /// The transport failed before a response was received.
    kTransport,
    /// The API returned a non-success status.
    kApi,
};

/// Human-readable description of an ErrorCode.
[[nodiscard]] constexpr std::string_view ToString(ErrorCode code) noexcept {
    switch (code) {
        case ErrorCode::kInvalidArgument:
            return "invalid argument";
        case ErrorCode::kTransport:
            return "transport";
        case ErrorCode::kApi:
            return "api";
    }
    return "unknown";
}

/// Error produced by the client.
struct Error {
    /// Category.
    ErrorCode code{ErrorCode::kTransport};
    /// Human-readable message.
    std::string message;
};

}  // namespace hcloud
