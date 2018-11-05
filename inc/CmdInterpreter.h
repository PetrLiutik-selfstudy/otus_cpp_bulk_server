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
     * @brief Интерпретация принятой команды.
     * @param input - входная команда.
     * @param cmd - команда для помещения в пул.
     * @return true - заверешен блок команд, false - блок команд не завершен.
     */
    bool interpret(const std::string& input, std::string& cmd);

  private:

    /// Максимальный размер блока команд.
    size_t bulk_size_{};
    /// Текущий размер блока команд.
    size_t size_{};
    /// Количество незакрытых токенов.
    size_t tokens_{};
};

} // namespace bulk.
