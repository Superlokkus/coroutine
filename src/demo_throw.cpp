#include <iostream>

#include <boost/asio.hpp>

boost::asio::awaitable<void>  coroutine() {
    std::cout << "Coroutine executes...\n";
    throw std::runtime_error("Test throw from coroutine!");
    co_return;
}

int main(int argc, char* argv[]) {
    boost::asio::io_context context;

    boost::asio::co_spawn(context, coroutine(), boost::asio::detached);
    boost::asio::co_spawn(context, coroutine(), [](std::exception_ptr ptr) {
        if (ptr) {
            std::cout << "Rethrowing in completion handler" << std::endl;
            std::rethrow_exception(ptr);
        } else {
            std::cout << "Completed without error" << std::endl;
        }
    });

    boost::asio::post(context, [] () {
        throw std::runtime_error("Test throw from post!");
    });

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
    t.join();
}