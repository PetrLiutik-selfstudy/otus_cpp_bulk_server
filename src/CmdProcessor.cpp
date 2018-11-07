#include "CmdProcessor.h"
#include "ConsoleWriter.h"
#include "FileWriter.h"

namespace bulk {

CmdProcessor& CmdProcessor::get_instance() {
  static CmdProcessor instance{};
  return instance;
}

handle_t CmdProcessor::create_context(size_t bulk_size) {
  auto context = std::make_shared<CmdProcessContext>(bulk_size, ++context_id_);
  auto handle = reinterpret_cast<handle_t>(context.get());

  auto console_writer = std::make_unique<bulk::ConsoleWriter>();
//  auto file_writer = std::make_unique<bulk::FileWriter>();

  console_writer->start();
//  file_writer->start();

  context->subscribe(std::move(console_writer));
//  context->subscribe(std::move(file_writer));

  std::unique_lock<std::mutex> lock(contexts_mutex_);
  contexts_.emplace(std::make_pair(handle, context));

  return handle;
}

void CmdProcessor::destroy_context(const handle_t& handle) {
  std::unique_lock<std::mutex> lock(contexts_mutex_);
  auto it = contexts_.find(handle);
  if(it != contexts_.cend()) {
    it->second->process(handle, nullptr, 0, true);
    contexts_.erase(it);
  }
}

void CmdProcessor::process(const handle_t& handle, const char* data, std::size_t size) {
  std::unique_lock<std::mutex> lock(contexts_mutex_);

  size_t n{};
  for(auto& it: contexts_) {
    if(it.second->is_busy(handle)) {
      it.second->process(handle, data, size);
      return;
    }
    if(!it.second->is_busy())
      n++;
  }

  size_t i = ++seed_ % n;
  for(auto& it: contexts_) {
    if(!--i && !it.second->is_busy()) {
      it.second->process(handle, data, size);
      return;
    }
  }
}

} // namespace bulk.
