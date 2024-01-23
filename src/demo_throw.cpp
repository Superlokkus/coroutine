#include <iostream>

#include <boost/asio.hpp>

boost::asio::awaitable<void>  coroutine_detached() {
    std::cout << "Coroutine executes detached\n";
    throw std::runtime_error("Test throw from coroutine!");
    co_return;
}

boost::asio::awaitable<void>  coroutine_with_rethrow_completion_handler() {
    std::cout << "Coroutine executes with rethrow completion handler\n";
    throw std::runtime_error("Test throw from coroutine!");
    co_return;
}

int main(int argc, char* argv[]) {
    boost::asio::io_context context;

    boost::asio::co_spawn(context, coroutine_detached(), boost::asio::detached);
    boost::asio::co_spawn(context, coroutine_with_rethrow_completion_handler(), [](std::exception_ptr ptr) {
        if (ptr) {
            std::cout << "Rethrowing in completion handler" << std::endl;
            std::rethrow_exception(ptr);
        } else {
            std::cout << "Completed without error" << std::endl;
        }
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