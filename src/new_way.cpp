#include <iostream>
#include <memory>
#include <boost/asio.hpp>

struct connection : std::enable_shared_from_this<connection> {
    connection(boost::asio::io_context& context /*, ip end point of new TCP connection*/)
            : context_{context}
    {
    }
    void start_loop(std::shared_ptr<connection> myself) {
        boost::asio::co_spawn(context_, loop(myself), boost::asio::detached);
    }
    boost::asio::awaitable<void> loop(std::shared_ptr<connection> myself) {
        boost::asio::steady_timer timer{context_};
        while (true) {
            timer.expires_after(std::chrono::seconds{1});
            co_await timer.async_wait(boost::asio::use_awaitable);
            op_a();
            timer.expires_after(std::chrono::seconds{1});
            co_await timer.async_wait(boost::asio::use_awaitable);
            op_b();
        }
    }
private:
    void op_a() {
        std::cout << "op_a " << ++session_data_ << "\n";
    }
    void op_b() {
        std::cout << "op_b " << ++session_data_ << "\n";
    }
    boost::asio::io_context& context_;
    int session_data_{};
};

int main() {
    boost::asio::io_context context;
    {
        /* suppose this is the TCP acceptor*/
        auto new_connection = std::make_shared<connection>(context);
        new_connection->start_loop(new_connection);
    }
    context.run();
}
