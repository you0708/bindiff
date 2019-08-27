#ifndef IDA_MAIN_PLUGIN_H_
#define IDA_MAIN_PLUGIN_H_

#include <memory>

#include "third_party/zynamics/bindiff/ida/results.h"
#include "third_party/zynamics/binexport/ida/plugin.h"

namespace security {
namespace bindiff {

class Plugin : public binexport::IdaPlugin<Plugin> {
 public:
  static constexpr char kComment[] =
      "Structural comparison of executable objects";  // Status line
  static constexpr char kHotKey[] = "CTRL-6";

  Results* results() { return results_.get(); }
  void set_results(Results* value) { results_.reset(value); }

  bool alsologtostderr() const { return alsologtostderr_; }

  bool LoadResults();

  void VisualDiff(uint32_t index, bool call_graph_diff);

  void DiscardResults();
  void SaveAndDiscardResults();

  int Init() override;
  void InitActions();
  void InitMenus();

  bool Run(size_t argument) override;

  void Terminate() override;
  void TermMenus();

 private:
  bool init_done_ = false;
  bool alsologtostderr_ = false;
  std::unique_ptr<Results> results_;
};

}  // namespace bindiff
}  // namespace security

#endif  // IDA_MAIN_PLUGIN_H_