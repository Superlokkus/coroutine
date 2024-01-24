#include <boost/asio.hpp>
#include <boost/asio/experimental/awaitable_operators.hpp>
#include <iostream>
namespace asio = boost::asio;


boost::asio::awaitable<void> timer(auto duration) {
    auto [ec] = co_await asio::steady_timer{co_await asio::this_coro::executor, duration}.async_wait(
            as_tuple(asio::use_awaitable));
    std::cout << "Wait for " << (duration / 1.s) << "s: " << ec.message() << std::endl;
};

int main() {
}