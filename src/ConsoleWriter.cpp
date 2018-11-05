#include "ConsoleWriter.h"

#include <iostream>
#include <sstream>

namespace bulk {

void ConsoleWriter::write(uint8_t, const Bulk& bulk) {
  add_job([this, bulk](){
    os_ << bulk;

    // Добавление метрики.
    std::lock_guard<std::mutex> lock(metrics_mutex_);
    metrics_.push(std::this_thread::get_id(), bulk);
  });
}

Metrics& ConsoleWriter::get_metrics() {
  std::lock_guard<std::mutex> lock(metrics_mutex_);
  return metrics_;
}

} // namespace bulk.
