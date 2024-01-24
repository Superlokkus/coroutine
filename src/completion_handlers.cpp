#include <iostream>
#include <boost/asio.hpp>

boost::asio::awaitable<void> do_stuff(boost::asio::io_context& context) {
    std::cout << "Waiting for 5 seconds in coroutine" << std::endl;
    boost::asio::steady_timer timer{context};
    timer.expires_after(std::chrono::seconds{5});
    co_await timer.async_wait(boost::asio::use_awaitable);
    std::cout << "Done waiting" << std::endl;
}

int main() {
    boost::asio::io_context context;
    auto future = boost::asio::co_spawn(context, do_stuff(context), boost::asio::use_future);
    std::cout << "Starting worker thread" << std::endl;
    std::thread t([&context]() {
        try {
            while (true) {
                context.run();
                return;
            }
        } catch (std::exception &e) {
            std::cerr << "Exception in context::run(): " << e.what() << "\n";
        }
    });
    std::cout << "Waiting for future" << std::endl;
    future.get();
    t.join();
}

