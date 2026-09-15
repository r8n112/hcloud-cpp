#include "hcloud/client.hpp"

#include <catch2/catch_test_macros.hpp>
#include <expected>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "hcloud/http.hpp"

namespace {

class RecordingTransport final : public hcloud::Transport {
public:
    std::expected<hcloud::HttpResponse, hcloud::Error> Send(
        const hcloud::HttpRequest& request) override {
        requests.push_back(request);
        return hcloud::HttpResponse{status, body};
    }

    std::vector<hcloud::HttpRequest> requests;
    int status{200};
    std::string body{"{}"};
};

std::string HeaderValue(const hcloud::HttpRequest& request, std::string_view name) {
    for (const auto& [key, value] : request.headers) {
        if (key == name) {
            return value;
        }
    }
    return {};
}

}  // namespace

TEST_CASE("rejects an empty token", "[client]") {
    auto transport = std::make_shared<RecordingTransport>();
    const auto client = hcloud::CloudClient::TryCreate("", transport);
    REQUIRE_FALSE(client.has_value());
    REQUIRE(client.error().code == hcloud::ErrorCode::kInvalidArgument);
}

TEST_CASE("rejects a null transport", "[client]") {
    const auto client = hcloud::CloudClient::TryCreate("token", nullptr);
    REQUIRE_FALSE(client.has_value());
    REQUIRE(client.error().code == hcloud::ErrorCode::kInvalidArgument);
}

TEST_CASE("builds an authenticated GET request", "[client]") {
    auto transport = std::make_shared<RecordingTransport>();
    const auto client = hcloud::CloudClient::TryCreate("secret", transport);
    REQUIRE(client.has_value());

    const auto response = client->Get("/servers");
    REQUIRE(response.has_value());

    REQUIRE(transport->requests.size() == 1);
    const auto& request = transport->requests.front();
    REQUIRE(request.method == "GET");
    REQUIRE(request.url == "https://api.hetzner.cloud/v1/servers");
    REQUIRE(HeaderValue(request, "Authorization") == "Bearer secret");
    REQUIRE(HeaderValue(request, "Accept") == "application/json");
    REQUIRE(request.body.empty());
}

TEST_CASE("builds a POST request with a JSON body", "[client]") {
    auto transport = std::make_shared<RecordingTransport>();
    const auto client = hcloud::CloudClient::TryCreate("secret", transport);
    REQUIRE(client.has_value());

    const auto response = client->Post("servers", R"({"name":"web-1"})");
    REQUIRE(response.has_value());

    const auto& request = transport->requests.front();
    REQUIRE(request.method == "POST");
    REQUIRE(request.url == "https://api.hetzner.cloud/v1/servers");
    REQUIRE(HeaderValue(request, "Content-Type") == "application/json");
    REQUIRE(request.body == R"({"name":"web-1"})");
}

TEST_CASE("normalises a trailing slash in the base URL", "[client]") {
    auto transport = std::make_shared<RecordingTransport>();
    const auto client =
        hcloud::CloudClient::TryCreate("secret", transport, "https://api.hetzner.cloud/v1/");
    REQUIRE(client.has_value());
    REQUIRE(client->base_url() == "https://api.hetzner.cloud/v1");

    const auto response = client->Get("servers");
    REQUIRE(response.has_value());
    REQUIRE(transport->requests.front().url == "https://api.hetzner.cloud/v1/servers");
}

TEST_CASE("maps a non-success status to an API error", "[client]") {
    auto transport = std::make_shared<RecordingTransport>();
    transport->status = 401;
    transport->body = R"({"error":{"message":"unauthorized"}})";

    const auto client = hcloud::CloudClient::TryCreate("secret", transport);
    REQUIRE(client.has_value());

    const auto response = client->Get("servers");
    REQUIRE_FALSE(response.has_value());
    REQUIRE(response.error().code == hcloud::ErrorCode::kApi);
}

TEST_CASE("propagates transport errors", "[client]") {
    class FailingTransport final : public hcloud::Transport {
    public:
        std::expected<hcloud::HttpResponse, hcloud::Error> Send(
            const hcloud::HttpRequest&) override {
            return std::unexpected(hcloud::Error{hcloud::ErrorCode::kTransport, "boom"});
        }
    };

    auto transport = std::make_shared<FailingTransport>();
    const auto client = hcloud::CloudClient::TryCreate("secret", transport);
    REQUIRE(client.has_value());

    const auto response = client->Get("servers");
    REQUIRE_FALSE(response.has_value());
    REQUIRE(response.error().code == hcloud::ErrorCode::kTransport);
}
