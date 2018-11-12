#include "CmdInterpreter.h"

namespace bulk {

bool CmdInterpreter::peek_dyn_bulk_start(const std::string &input) {
  return (input == "{");
}

bool CmdInterpreter::interpret(const std::string &input, std::string &cmd) {
  if (input.empty()) {
    /// Завершение по EOF.
    if ((tokens_ == 0) && (size_ > 0)) {
      size_ = 0;
      return true;
    }
  } else if (input == "{") {
    /// Начало блока.
    if ((tokens_++ == 0) && (size_ > 0)) {
      size_ = 0;
      return true;
    }
  } else if (input == "}") {
    /// Конец блока.
    if (tokens_ > 0)
      if (--tokens_ == 0)
        return true;
  } else {
    /// Команда.
    cmd = input;
    if ((tokens_ == 0) && (++size_ == bulk_size_)) {
      size_ = 0;
      return true;
    }
  }
  return false;
}

bool CmdInterpreter::is_dyn_bulk_finished() {
  return tokens_ == 0;
}

} // namespace bulk.
