#pragma once

#include <boost/asio.hpp>

namespace ba = boost::asio;

namespace bulk {

/**
 * @brief Класс сервера обработки блоков команд.
 */
class BulkServer {
public:
  /**
   * @brief Конструктор сервера обработки блоков команд.
   * @param port - номер порта.
   * @param bulk_size- размер блока команд.
   */
  explicit BulkServer(uint16_t port, size_t bulk_size);

  ~BulkServer();

  /**
   * @brief Запуск сервера.
   */
  void start();

private:
  /**
   * @brief Обработка акцептирования сокета.
   */
  void handle_accept();

  /**
   * @brief Обработка останова сервера.
   */
  void handle_stop();

  ba::io_service        service_{};
  ba::ip::tcp::acceptor acceptor_;
  ba::ip::tcp::socket   sock_;
  size_t                bulk_size_;
};

} // namespace bulk.
