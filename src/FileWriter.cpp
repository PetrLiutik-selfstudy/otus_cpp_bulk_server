#include "FileWriter.h"

#include <fstream>

namespace bulk {

void FileWriter::write(uint8_t context_id, const Bulk& bulk) {
  add_job([this, context_id, bulk](){
    std::string file_name = "bulk" + std::to_string(bulk.time()) + "_" +
                            std::to_string(context_id) +  "_" +
                            std::to_string(get_job_id()) + ".log";
    std::fstream fs{file_name, std::ios::app};

    if(fs.is_open()) {
      fs << bulk;
      fs.close();
    }

    // Добавление метрики.
    std::lock_guard<std::mutex> lock(metrics_mutex_);
    metrics_.push(std::this_thread::get_id(), bulk);
  });
}

Metrics& FileWriter::get_metrics() {
  std::lock_guard<std::mutex> lock(metrics_mutex_);
  return metrics_;
}


} // namespace bulk.
