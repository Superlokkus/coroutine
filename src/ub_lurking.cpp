#include <iostream>

#include <boost/asio.hpp>

boost::asio::awaitable<void> blocking () {
    while(true){
        std::cout << "blocking running" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds{2});
    }

    co_return;
}

boost::asio::awaitable<void> blocking_too() {
    while(true){
        std::cout << "blocking too running" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds{2});
    }

    co_return; //If you remove this statement, then you will see "blocking too running"
}

int main(int argc, char* argv[]) {
    boost::asio::io_context context;
    auto strand = boost::asio::make_strand(context);

    boost::asio::co_spawn(strand, blocking(), boost::asio::detached);
    boost::asio::co_spawn(strand, blocking_too(), boost::asio::detached);

    std::vector<std::thread> io_run_threads_ {};
    const auto thread_count{std::max<unsigned>(std::thread::hardware_concurrency(), 4)};
    std::generate_n(std::back_inserter(io_run_threads_),
                    thread_count, [&]() {
                return std::thread{
                        [&]() {
                            std::cout << "Starting thread" << std::endl;
                            while (true) {
                                try {
                                    context.run();
                                    break;
                                } catch (std::exception &e) {
                                    std::cerr << e.what() << "\n";
                                }
                            }
                        }};
            });
    for (auto& thread : io_run_threads_) {
        thread.join();
    }

}