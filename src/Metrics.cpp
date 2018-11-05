#include "Metrics.h"

namespace bulk {

void Metrics::push(const std::thread::id& thread_id, const Bulk& bulk, size_t rows) {
  storage_[thread_id] += MetricsItem{rows, 1, bulk.size()};
}

void Metrics::clear() {
  storage_.clear();
}

std::ostream& operator <<(std::ostream& os, const Metrics& metrics) {
  bool add_thread_num = metrics.storage_.size() > 1;
  size_t thread_num{1};
  for(const auto& it: metrics.storage_) {
    // Вывод информации о потоке.
    os << metrics.name_;
    if(add_thread_num)
      os << thread_num++;
    os << " thread_id " << it.first << " - ";

    // Вывод количества обработанных строк.
    if(it.second.get_rows() > 0)
      os << it.second.get_rows() << " row(s), ";

    // Вывод количества обработанных блоков команд.
    os << it.second.get_bulks() << " bulk(s), ";

    // Вывод количества обработанных команд.
    os << it.second.get_cmds() << " command(s) " << std::endl;
  }
  return os;
}

} // namespace bulk.
