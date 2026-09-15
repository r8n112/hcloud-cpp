#pragma once

#include <expected>
#include <memory>
#include <string>
#include <string_view>

#include "hcloud/error.hpp"
#include "hcloud/http.hpp"

namespace hcloud {

/// Default base URL of the Hetzner Cloud API.
inline constexpr std::string_view kDefaultBaseUrl = "https://api.hetzner.cloud/v1";

/// A low-level client for the Hetzner Cloud API.
///
/// The client builds authenticated requests and delegates the actual transfer
/// to a [`Transport`]. It does not parse response payloads yet; typed models are
/// tracked in `BACKLOG.md`.
class CloudClient {
public:
    /// Creates a client, validating the token and transport.
    ///
    /// Returns an [`ErrorCode::kInvalidArgument`] error if `token` or `base_url`
    /// is empty, or if `transport` is null.
    [[nodiscard]] static std::expected<CloudClient, Error> TryCreate(
        std::string token, std::shared_ptr<Transport> transport,
        std::string base_url = std::string(kDefaultBaseUrl));

    /// Sends a `GET` request to `path`.
    [[nodiscard]] std::expected<HttpResponse, Error> Get(std::string_view path) const;

    /// Sends a `POST` request with a JSON `body` to `path`.
    [[nodiscard]] std::expected<HttpResponse, Error> Post(std::string_view path,
                                                          std::string body) const;

    /// Returns the configured base URL (without a trailing slash).
    [[nodiscard]] const std::string& base_url() const noexcept {
        return base_url_;
    }

private:
    CloudClient(std::string token, std::shared_ptr<Transport> transport, std::string base_url);

    [[nodiscard]] std::expected<HttpResponse, Error> Send(std::string method, std::string_view path,
                                                          std::string body) const;

    std::string token_;
    std::shared_ptr<Transport> transport_;
    std::string base_url_;
};

}  // namespace hcloud
