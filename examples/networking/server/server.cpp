//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "server.hpp"
#include <csignal>
#include <utility>

namespace http::server {

    //[server_construct Server constructor
    server::server(const std::string &address, const std::string &port,
                   const std::string &doc_root)
        : io_context_(1), signals_(io_context_), acceptor_(io_context_),
          connection_manager_(), request_handler_(doc_root) {
        setup_signals();
        setup_acceptor(address, port);
    }
    //]

    //[setup_acceptor Setup server acceptor
    void server::setup_acceptor(
        const std::string &address,
        const std::string &port) {

        // Open the acceptor with the option to
        // reuse the address (i.e. SO_REUSEADDR).
        asio::ip::tcp::resolver resolver(io_context_);
        asio::ip::tcp::endpoint endpoint =
            *resolver.resolve(address, port).begin();
        acceptor_.open(endpoint.protocol());
        acceptor_.set_option(asio::socket_base::reuse_address(true));
        acceptor_.bind(endpoint);
        acceptor_.listen();

        schedule_accept();
    }
    //]

    //[server_setup_signals Server setup_signals
    void server::setup_signals() {
        // Register to handle the signals that indicate when the server
        // should exit. It is safe to register for the same signal multiple
        // times in a program, provided all registration for the specified
        // signal is made through Asio.
        signals_.add(SIGINT);
        signals_.add(SIGTERM);
#if defined(SIGQUIT)
        signals_.add(SIGQUIT);
#endif // defined(SIGQUIT)
        schedule_await_stop();
    }
    //]

    //[server_run Server run
    void server::run() {
        // The io_context::run() call will block until all asynchronous
        // operations have finished. While the server is running, there is
        // always at least one asynchronous operation outstanding: the
        // asynchronous accept call waiting for new incoming connections.
        for (size_t i = 0; i < std::thread::hardware_concurrency(); ++i) {
            asio::post(pool_, [this] { io_context_.run(); });
        }
        pool_.join();
    }
    //]

    //[server_schedule_accept Server schedule_accept
    void server::schedule_accept() {
        acceptor_.async_accept([this](std::error_code ec,
                                      asio::ip::tcp::socket socket) {
            // Check whether the server was stopped by a signal before
            // this completion handler had a chance to run.
            if (!acceptor_.is_open()) {
                return;
            }

            if (!ec) {
                connection_manager_.start(std::make_shared<connection>(
                    std::move(socket), connection_manager_, request_handler_));
            }

            schedule_accept();
        });
    }
    //]

    //[server_schedule_await_stop Server schedule_await_stop
    void server::schedule_await_stop() {
        signals_.async_wait([this](std::error_code /*ec*/, int /*signo*/) {
            // The server is stopped by cancelling all outstanding
            // asynchronous operations. Once all operations have finished
            // the io_context::run() call will exit.
            acceptor_.close();
            connection_manager_.stop_all();
        });
    }
    //]

} // namespace http::server
