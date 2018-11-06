#include "BulkServer.h"
#include "BulkClientSession.h"

namespace bulk {

BulkServer::BulkServer(uint16_t port, size_t bulk_size) :
  acceptor_(service_, ba::ip::tcp::endpoint(ba::ip::tcp::v4(), port)),
  sock_(service_),
  bulk_size_{bulk_size} {
}

BulkServer::~BulkServer() {
}

void BulkServer::handle_accept() {
  acceptor_.async_accept(sock_, [this](const boost::system::error_code& ec) {
    if(!ec) {
      std::make_shared<BulkClientSession>(bulk_size_, std::move(sock_))->start();
    }
    handle_accept();
  });
}

void BulkServer::start() {
  ba::signal_set signals(service_, SIGINT, SIGTERM);
  signals.async_wait([this, &signals](const boost::system::error_code&, int) {
    handle_stop();
  });
  handle_accept();
  service_.run();
}

void BulkServer::handle_stop() {
  service_.stop();
}

} // namespace bulk.
