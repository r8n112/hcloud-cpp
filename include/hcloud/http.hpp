#pragma once

#include <expected>
#include <string>
#include <utility>
#include <vector>

#include "hcloud/error.hpp"

namespace hcloud {

/// A single HTTP request.
struct HttpRequest {
    /// HTTP method, for example `GET` or `POST`.
    std::string method;
    /// Fully qualified URL.
    std::string url;
    /// Request headers as (name, value) pairs.
    std::vector<std::pair<std::string, std::string>> headers;
    /// Request body, empty for bodyless requests.
    std::string body;
};

/// A single HTTP response.
struct HttpResponse {
    /// HTTP status code.
    int status{0};
    /// Response body.
    std::string body;
};

/// Executes HTTP requests.
///
/// The real implementation will use libcurl; tests provide an in-memory
/// implementation so the suite never touches the network.
class Transport {
public:
    Transport() = default;
    Transport(const Transport&) = delete;
    Transport& operator=(const Transport&) = delete;
    Transport(Transport&&) = delete;
    Transport& operator=(Transport&&) = delete;
    virtual ~Transport() = default;

    /// Sends `request` and returns the response, or an error.
    [[nodiscard]] virtual std::expected<HttpResponse, Error> Send(const HttpRequest& request) = 0;
};

}  // namespace hcloud
