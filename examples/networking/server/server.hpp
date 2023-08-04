//
// server.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include "connection.hpp"
#include "connection_manager.hpp"
#include "request_handler.hpp"
#include <asio.hpp>
#include <string>

namespace http::server {

    //[server_define Define top-level class for the HTTP server.
    class server {
      public:
        server(const server &) = delete;
        server &operator=(const server &) = delete;

        // Construct the server to listen on the specified TCP address and
        // port, and serve up files from the given directory.
        explicit server(const std::string &address, const std::string &port,
                        const std::string &doc_root);

        // Run the server's io_context loop.
        void run();

      private:
        // Perform an asynchronous accept operation.
        void schedule_accept();

        // Wait for a request to stop the server.
        void schedule_await_stop();

        // The io_context used to perform asynchronous operations.
        asio::io_context io_context_;

        // Executor for the io context
        asio::thread_pool pool_;

        // The signal_set is used to register for process termination
        // notifications.
        asio::signal_set signals_;

        // Acceptor used to listen for incoming connections.
        asio::ip::tcp::acceptor acceptor_;

        // The connection manager which owns all live connections.
        connection_manager connection_manager_;

        // The handler for all incoming requests.
        request_handler request_handler_;

        // Helper class to setup signals
        void setup_signals();

        // Helper class to setup acceptor
        void setup_acceptor(const std::string &address,
                            const std::string &port);
    };
    //]

} // namespace http::server

#endif // HTTP_SERVER_HPP
