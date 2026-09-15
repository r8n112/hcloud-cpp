#include "hcloud/client.hpp"

#include <cstddef>
#include <utility>

namespace hcloud {
namespace {

constexpr std::string_view kAuthorizationPrefix = "Bearer ";
constexpr std::size_t kErrorBodyLimit = 200;

std::string TrimLeadingSlashes(std::string_view path) {
    std::size_t start = 0;
    while (start < path.size() && path[start] == '/') {
        ++start;
    }
    return std::string(path.substr(start));
}

std::string Truncate(std::string_view text) {
    if (text.size() <= kErrorBodyLimit) {
        return std::string(text);
    }
    return std::string(text.substr(0, kErrorBodyLimit));
}

}  // namespace

CloudClient::CloudClient(std::string token, std::shared_ptr<Transport> transport,
                         std::string base_url)
    : token_(std::move(token)), transport_(std::move(transport)), base_url_(std::move(base_url)) {}

std::expected<CloudClient, Error> CloudClient::TryCreate(std::string token,
                                                         std::shared_ptr<Transport> transport,
                                                         std::string base_url) {
    if (token.empty()) {
        return std::unexpected(Error{ErrorCode::kInvalidArgument, "token must not be empty"});
    }
    if (transport == nullptr) {
        return std::unexpected(Error{ErrorCode::kInvalidArgument, "transport must not be null"});
    }
    if (base_url.empty()) {
        return std::unexpected(Error{ErrorCode::kInvalidArgument, "base URL must not be empty"});
    }
    while (!base_url.empty() && base_url.back() == '/') {
        base_url.pop_back();
    }
    return CloudClient{std::move(token), std::move(transport), std::move(base_url)};
}

std::expected<HttpResponse, Error> CloudClient::Get(std::string_view path) const {
    return Send("GET", path, {});
}

std::expected<HttpResponse, Error> CloudClient::Post(std::string_view path,
                                                     std::string body) const {
    return Send("POST", path, std::move(body));
}

std::expected<HttpResponse, Error> CloudClient::Send(std::string method, std::string_view path,
                                                     std::string body) const {
    HttpRequest request;
    request.method = std::move(method);
    request.url = base_url_ + "/" + TrimLeadingSlashes(path);
    request.headers.emplace_back("Accept", "application/json");
    request.headers.emplace_back("Authorization", std::string(kAuthorizationPrefix) + token_);
    if (request.method == "POST") {
        request.headers.emplace_back("Content-Type", "application/json");
    }
    request.body = std::move(body);

    auto response = transport_->Send(request);
    if (!response) {
        return std::unexpected(response.error());
    }
    if (response->status < 200 || response->status > 299) {
        return std::unexpected(Error{ErrorCode::kApi, "API returned HTTP " +
                                                          std::to_string(response->status) + ": " +
                                                          Truncate(response->body)});
    }
    return response;
}

}  // namespace hcloud
