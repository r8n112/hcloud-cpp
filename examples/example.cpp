//! Demonstrates building an authenticated request without touching the network.

#include <expected>
#include <iostream>
#include <memory>

#include "hcloud/client.hpp"
#include "hcloud/http.hpp"

namespace {

class StubTransport final : public hcloud::Transport {
public:
    std::expected<hcloud::HttpResponse, hcloud::Error> Send(
        const hcloud::HttpRequest& request) override {
        std::cout << request.method << " " << request.url << '\n';
        return hcloud::HttpResponse{200, "{}"};
    }
};

}  // namespace

int main() {
    auto client = hcloud::CloudClient::TryCreate("token", std::make_shared<StubTransport>());
    if (!client) {
        std::cerr << "error: " << hcloud::ToString(client.error().code) << ": "
                  << client.error().message << '\n';
        return 1;
    }

    const auto response = client->Get("/servers");
    if (!response) {
        std::cerr << "error: " << hcloud::ToString(response.error().code) << ": "
                  << response.error().message << '\n';
        return 1;
    }

    std::cout << "HTTP " << response->status << '\n';
    return 0;
}
