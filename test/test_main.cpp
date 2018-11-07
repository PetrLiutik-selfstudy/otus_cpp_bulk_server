#include "gtest/gtest.h"
#include "../inc/ver.h"
#include "../inc/CmdProcessContext.h"

#include <vector>
#include <string>
#include <sstream>

TEST(ver_test_case, ver_major_test) {
  EXPECT_GE(ver_major(), 1);
}

TEST(ver_test_case, ver_minor_test) {
  EXPECT_GE(ver_minor(), 1);
}

TEST(ver_test_case, ver_patch_test) {
  EXPECT_GE(ver_patch(), 1);
}

namespace bulk {

/**
 * @brief Класс для тестового вывода блока команд.
 */
class TestWriter : public IStreamWriter {
  public:
    TestWriter() {
      bulk_pool_.clear();
      metrics_.clear();
      time_ = std::time_t{};
      context_id_ = uint8_t{};
    }

    ~TestWriter() override = default;

    void write(uint8_t context_id, const Bulk& bulk) final {
      bulk_pool_ = bulk.get_cmds();
      time_ = bulk.time();
      metrics_.push(std::this_thread::get_id(), bulk);
      context_id_ = context_id;
    }

    static auto get_bulk() {
      return bulk_pool_;
    }

    static auto get_time() {
      return time_;
    }

    Metrics& get_metrics() final {
      return metrics_;
    }

    static Metrics& get_metrics_stub() {
      return metrics_;
    }

    static auto get_context_id() {
      return time_;
    }

  private:
    static std::vector<std::string> bulk_pool_;
    static std::time_t time_;
    static Metrics metrics_;
    static uint8_t context_id_;
};

std::vector<std::string> TestWriter::bulk_pool_{};
std::time_t TestWriter::time_{};
Metrics TestWriter::metrics_{"test"};
uint8_t TestWriter::context_id_{};

} // namespace bulk.

TEST(context_test_case, subscribe_test) {
  bulk::CmdProcessContext context{1, 1};
  auto test_writer = std::make_unique<bulk::TestWriter>();
  context.subscribe(std::move(test_writer));

  std::string str = "cmd1\n";

  context.process(nullptr, str.c_str(), str.size());

  EXPECT_EQ(bulk::TestWriter::get_bulk(), std::vector<std::string>{"cmd1"});
  EXPECT_NE(bulk::TestWriter::get_time(), std::time_t{});
}


TEST(context_test_case, partial_bulk_test) {
  bulk::CmdProcessContext context{3, 1};
  auto test_writer = std::make_unique<bulk::TestWriter>();
  context.subscribe(std::move(test_writer));

  std::string str = "cmd1\ncmd2\n";

  context.process(nullptr, str.c_str(), str.size());
  context.process(nullptr, nullptr, 0, true); // Принудительное завершение блока.

  std::vector<std::string> result{"cmd1", "cmd2"};
  EXPECT_EQ(bulk::TestWriter::get_bulk(), result);
  EXPECT_NE(bulk::TestWriter::get_time(), std::time_t{});
}

TEST(context_test_case, full_bulk_test) {
  bulk::CmdProcessContext context{3, 1};
  auto test_writer = std::make_unique<bulk::TestWriter>();
  context.subscribe(std::move(test_writer));

  std::string str = "cmd1\ncmd2\ncmd3\n";

  context.process(nullptr, str.c_str(), str.size());

  std::vector<std::string> result{"cmd1", "cmd2", "cmd3"};
  EXPECT_EQ(bulk::TestWriter::get_bulk(), result);
  EXPECT_NE(bulk::TestWriter::get_time(), std::time_t{});
}

TEST(context_test_case, full_tail_bulk_test) {
  bulk::CmdProcessContext context{3, 1};
  auto test_writer = std::make_unique<bulk::TestWriter>();
  context.subscribe(std::move(test_writer));

  std::string str = "cmd1\ncmd2\ncmd3\ncmd4\ncmd5\n";

  context.process(nullptr, str.c_str(), str.size());
  context.process(nullptr, nullptr, 0, true); // Принудительное завершение блока.

  std::vector<std::string> result{"cmd4", "cmd5"};
  EXPECT_EQ(bulk::TestWriter::get_bulk(), result);
  EXPECT_NE(bulk::TestWriter::get_time(), std::time_t{});
}

TEST(context_test_case, partial_dyn_bulk_test) {
  bulk::CmdProcessContext context{3, 1};
  auto test_writer = std::make_unique<bulk::TestWriter>();
  context.subscribe(std::move(test_writer));

  std::string str = "cmd1\ncmd2\n{\ncmd3\ncmd4\ncmd5\ncmd6\n";

  context.process(nullptr, str.c_str(), str.size());

  std::vector<std::string> result{"cmd1", "cmd2"};
  EXPECT_EQ(bulk::TestWriter::get_bulk(), result);
  EXPECT_NE(bulk::TestWriter::get_time(), std::time_t{});
}

TEST(context_test_case, full_dyn_bulk_test) {
  bulk::CmdProcessContext context{3, 1};
  auto test_writer = std::make_unique<bulk::TestWriter>();
  context.subscribe(std::move(test_writer));

  std::string str = "cmd1\ncmd2\n{\ncmd3\ncmd4\ncmd5\ncmd6\n}\n";

  context.process(nullptr, str.c_str(), str.size());

  std::vector<std::string> result{"cmd3", "cmd4", "cmd5", "cmd6"};
  EXPECT_EQ(bulk::TestWriter::get_bulk(), result);
  EXPECT_NE(bulk::TestWriter::get_time(), std::time_t{});
}

TEST(context_test_case, full_dyn_bulk_1_test) {
  bulk::CmdProcessContext context{3, 1};
  auto test_writer = std::make_unique<bulk::TestWriter>();
  context.subscribe(std::move(test_writer));

  std::string str = "cmd1\ncmd2\n{\ncmd3\n{\ncmd4\ncmd5\n}\ncmd6\n}\n";

  context.process(nullptr, str.c_str(), str.size());

  std::vector<std::string> result{"cmd3", "cmd4", "cmd5", "cmd6"};
  EXPECT_EQ(bulk::TestWriter::get_bulk(), result);
  EXPECT_NE(bulk::TestWriter::get_time(), std::time_t{});
}

TEST(context_test_case, eof_bulk_test) {
  bulk::CmdProcessContext context{3, 1};
  auto test_writer = std::make_unique<bulk::TestWriter>();
  context.subscribe(std::move(test_writer));

  std::string str = "cmd1\ncmd2\n{\ncmd3\ncmd4\ncmd5\ncmd6\n}\ncmd7\ncmd8\n";

  context.process(nullptr, str.c_str(), str.size());
  context.process(nullptr, nullptr, 0, true); // Принудительное завершение блока.

  std::vector<std::string> result{"cmd7", "cmd8"};
  EXPECT_EQ(bulk::TestWriter::get_bulk(), result);
  EXPECT_NE(bulk::TestWriter::get_time(), std::time_t{});
}

TEST(context_test_case, metrics_test) {
  bulk::CmdProcessContext context{3, 1};
  auto test_writer = std::make_unique<bulk::TestWriter>();
  context.subscribe(std::move(test_writer));

  std::string str = "cmd1\ncmd2\n{\ncmd3\ncmd4\ncmd5\ncmd6\n}\ncmd7\ncmd8\n";

  context.process(nullptr, str.c_str(), str.size());
  context.process(nullptr, nullptr, 0, true); // Принудительное завершение блока.

  std::stringstream ss1;
  ss1 << "test";
  ss1 << " thread_id " << std::this_thread::get_id() << " - ";
  ss1 << 3 << " bulk(s), ";
  ss1 << 8 << " command(s) " << std::endl;

  std::stringstream ss2;
  ss2 << bulk::TestWriter::get_metrics_stub();

  EXPECT_EQ(ss1.str(), ss2.str());
}

TEST(context_test_case, context_id_test) {
  bulk::CmdProcessContext context{1, 1};
  auto test_writer = std::make_unique<bulk::TestWriter>();
  context.subscribe(std::move(test_writer));

  std::string str = "cmd1\n";

  context.process(nullptr, str.c_str(), str.size());

  EXPECT_NE(bulk::TestWriter::get_context_id(), 1);
}

TEST(bulk_test_case, print_bulk_test) {
  bulk::Bulk bulk;
  bulk.push("cmd1");
  bulk.push("cmd2");
  bulk.push("cmd3");

  std::stringstream ss;
  ss << bulk;

  std::string result{"bulk: cmd1, cmd2, cmd3\n"};

  EXPECT_EQ(ss.str(), result);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
