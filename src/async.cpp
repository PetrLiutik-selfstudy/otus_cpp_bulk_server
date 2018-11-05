#include "async.h"
#include "CmdProcessor.h"

namespace async {

handle_t connect(std::size_t bulk_size) {
  return bulk::CmdProcessor::get_instance().create_context(bulk_size);
}

void receive(handle_t handle, const char* data, std::size_t size) {
  bulk::CmdProcessor::get_instance().process(handle, data, size);
}

void disconnect(handle_t handle) {
  return bulk::CmdProcessor::get_instance().destroy_context(handle);
}

}
