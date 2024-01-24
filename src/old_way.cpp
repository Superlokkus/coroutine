#include <iostream>
#include <memory>
#include <boost/asio.hpp>

struct connection : std::enable_shared_from_this<connection> {
    connection(boost::asio::io_context& context /*, ip end point of new TCP connection*/)
            : context_{context}, timer_{context_}
    {
    }
    void start_loop(std::shared_ptr<connection> myself) {
        timer_.expires_after(std::chrono::seconds{1});
        timer_.async_wait([self = shared_from_this()] (const auto& boost_error){
            if (boost_error) {
                throw std::runtime_error(boost_error.message());
            }
            self->op_a();
        });
    }
private:
    void op_a() {
        std::cout << "op_a " << ++session_data_ << "\n";
        timer_.expires_after(std::chrono::seconds{1});
        timer_.async_wait([self = shared_from_this()] (const auto& boost_error){
            if (boost_error) {
                throw std::runtime_error(boost_error.message());
            }
            self->op_b();
            self->timer_.expires_after(std::chrono::seconds{1});
            self->timer_.async_wait([self = self->shared_from_this()] (const auto& boost_error){
                if (boost_error) {
                    throw std::runtime_error(boost_error.message());
                }
                self->op_a();
            });
        });
    }
    void op_b() {
        std::cout << "op_b " << ++session_data_ << "\n";
    }
    boost::asio::io_context& context_;
    boost::asio::steady_timer timer_;
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
