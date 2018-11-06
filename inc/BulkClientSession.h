#pragma once

#include "CmdProcessor.h"

#include <array>
#include <memory>

#include <boost/asio.hpp>

namespace ba = boost::asio;

namespace bulk {

/**
 * @brief Класс сессии клиента.
 */
class BulkClientSession : public std::enable_shared_from_this<BulkClientSession> {
public:
  /**
   * @brief Конструктор клиентской сессии.
   * @param bulk_size - размер блока команд.
   * @param sock - сокет, через который установлено соединение с клиентом.
   */
  explicit BulkClientSession(size_t bulk_size, ba::ip::tcp::socket sock);

  ~BulkClientSession() = default;

  /**
 * @brief Запустить клиентскую сессию.
 */
  void start();

private:

  /**
   * @brief Обработка асинхронного чтения данных из сокета.
   */
  void handle_read();

  /// Размер приемного буфера.
  static constexpr size_t buf_len = 1024;

  size_t                             bulk_size_;
  ba::ip::tcp::socket                sock_;
  handle_t                           handle_{};
  std::array<char, buf_len>          buf_;
  std::shared_ptr<BulkClientSession> self_;
};

} // namespace bulk.
