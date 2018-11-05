#pragma once

#include "IStreamWriter.h"

#include<memory>

namespace bulk {

/// Тип обозревателя.
using observer_t = std::unique_ptr<IStreamWriter>;

/**
 * @brief Интерфейс обозреваемого.
 */
class IObservable {
  public:
    virtual ~IObservable() = default;

    /**
     * @brief Подписка на получение блоков команд на вывод.
     * @param observer - подписчик.
     */
    virtual void subscribe(observer_t observer) = 0;

    /**
     * @brief Публикация блока команд.
     * @param bulk - блок команд.
     */
    virtual void publish(const Bulk& bulk) = 0;
};

} // namespace bulk.
