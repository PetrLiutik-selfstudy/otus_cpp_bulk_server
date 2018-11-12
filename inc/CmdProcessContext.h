#pragma once

#include "Bulk.h"
#include "CmdInterpreter.h"
#include "IObservable.h"
#include "Metrics.h"

#include <functional>
#include <list>
#include <memory>

namespace bulk {

using handle_t = void*;

/**
 * @brief Класс котнекста обработчика команд.
 */
class CmdProcessContext : public IObservable  {
  public:
    /**
     * @brief Конструктор.
     * @param bulk_size - макисмальный размер блока команд.
     */
    explicit CmdProcessContext(size_t bulk_size, uint8_t id) :
      interpreter_{bulk_size}, metrics_{"main"}, id_{id} {
    }

    ~CmdProcessContext() override = default;

    /**
     * @brief Подписка на получение блоков команд на вывод.
     * @param observer - подписчик.
     */
    void subscribe(observer_t observer) final;

    /**
    * @brief Проверка необходимости команды обработки данным контекстом.
    * @param data - входные данные, содержащие команды.
    * @param size - размер входных данных.
    */
    bool peek_process_by_this(const char* data, std::size_t size);

    /**
     * @brief Обработка входной команды.
     * @param handle - handle от имени которого выполняется обработка.
     * @param data - входные данные, содержащие команды.
     * @param size - размер входных данных.
     * @param finish_bulk - принудительное завершение блока команд.
     */
    void process(const handle_t& handle, const char* data, std::size_t size, bool finish_bulk = false);

    /**
     * @brief Проверка занятости контекста обработкой блока.
     * @return true - контекст занят, false - контекст свободен.
     */
    bool is_busy();

    /**
    * @brief Проверка занятости контекста обработкой блока конкретным handle'ом.
    * @return true - контекст занят, false - контекст свободен.
    */
    bool is_busy(const handle_t& handle);

    /**
     * @brief Вывод метрик в поток.
     * @param os - выходной поток.
     */
    void print_metrics(std::ostream& os);

  private:
    /**
     * @brief Публикация блока команд.
     * @param bulk - блок команд.
     */
    void publish(const Bulk& bulk) final;


    /// Список обозревателей, ожидающих вывод содержимого пула.
    std::list<observer_t> observers_{};
    /// Блок команд.
    Bulk bulk_{};
    /// Интерпретатор команд.
    CmdInterpreter interpreter_;
    /// Метрики основного потока.
    Metrics metrics_;
    /// Строка, содержащая данные со входными командами.
    std::string data_;
    /// id контекста.
    uint8_t id_;
    /// handle от имени которого выполняется обработка.
    handle_t handle_{};
};

} // namespace bulk.
