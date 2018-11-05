#pragma once

#include "Bulk.h"

#include <ostream>
#include <string>
#include <map>
#include <thread>

namespace bulk {

class Metrics {
  public:
    /**
     * @brief Конструктор метрики.
     * @param имя метрики.
     */
    explicit Metrics(std::string name) : name_{name}, storage_{} {
    }
    ~Metrics() = default;

    /**
     * @brief Добавление метрики по блоку команд.
     * @param thread_id - id потока добавляющего метрику.
     * @param bulk - блок команд.
     * @param rows - количество обработанных строк, для формирования блока команд.
     */
    void push(const std::thread::id& thread_id, const Bulk& bulk, size_t rows = 0);

    /**
     * @brief Очистка метрик.
     */
    void clear();

    /**
     * @brief Оператор вывода в поток блока команд.
     * @param os - поток вывода.
     * @param bulk - блок команд для вывода в поток.
     * @return
     */
    friend std::ostream& operator <<(std::ostream& os, const Metrics& metrics);

  private:

    /// Элемент хранилища метрик.
    class MetricsItem {
      public:
        MetricsItem() = default;
        explicit MetricsItem(size_t rows, size_t bulks, size_t cmds) : rows_{rows}, bulks_{bulks}, cmds_{cmds} {
        }
        ~MetricsItem() = default;

        void operator += (const MetricsItem& other) {
          rows_  += other.rows_;
          bulks_ += other.bulks_;
          cmds_  += other.cmds_;
        }

        size_t get_rows() const {
          return rows_;
        }

        size_t get_bulks() const {
          return bulks_;
        }

        size_t get_cmds() const {
          return cmds_;
        }

      private:
        size_t rows_{};
        size_t bulks_{};
        size_t cmds_{};
    };

    /// Название метрики.
    std::string name_;

    /// Хранилище метрик по потокам.
    std::map<std::thread::id, MetricsItem> storage_;
};

} // namespace bulk.
