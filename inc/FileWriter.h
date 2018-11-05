#pragma once

#include "IStreamWriter.h"
#include "ThreadPool.h"

namespace bulk {

/**
 * @brief Класс вывода блока команд в файл.
 */
class FileWriter : public IStreamWriter, public ThreadPool<2> {
  public:
    explicit FileWriter() : metrics_{"file"} {
    }

    ~FileWriter() override {
      stop();
    }

    /**
     * @brief Запись блока команд в поток.
     * @param context_id - id контекста из которого производится вывод.
     * @param bulk - блок команд.
     */
    void write(uint8_t context_id, const Bulk& bulk) final;

    /**
     * @brief Дать метрики работы потоков.
     * @return метрики по каждому из потоков.
     */
    Metrics& get_metrics() final;

  private:

    std::mutex metrics_mutex_{};
    Metrics metrics_;
};

} // namespace bulk.
