#pragma once

#include <string>

namespace bulk {

/**
 * @brief Класс интерпретатора команд.
 */
class CmdInterpreter {
  public:
    /**
    * @brief Конструктор.
    * @param bulk_size - макисмальный размер блока команд.
    */
    explicit CmdInterpreter(size_t bulk_size) : bulk_size_(bulk_size) {
    }

    ~CmdInterpreter() = default;

    /**
    * @brief Проверка запуска динамического блока по принятой команды.
    * @param input - входная команда.
    * @return true - будет начат динамический блок, false - не будет начат динамический блок.
    */
    bool peek_dyn_bulk_start(const std::string &input);

    /**
     * @brief Интерпретация принятой команды.
     * @param input - входная команда.
     * @param cmd - команда для помещения в пул.
     * @return true - заверешен блок команд, false - блок команд не завершен.
     */
    bool interpret(const std::string& input, std::string& cmd);

    /**
     * @brief Проверка завершенности динамического блока.
     * @return true - динамический блок команд заверешен, false - динамический блок команд не завершен.
     */
    bool is_dyn_bulk_finished();

  private:

    /// Максимальный размер блока команд.
    size_t bulk_size_{};
    /// Текущий размер блока команд.
    size_t size_{};
    /// Количество незакрытых токенов.
    size_t tokens_{};
};

} // namespace bulk.
