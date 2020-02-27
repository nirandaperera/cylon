#include "arrow_join.hpp"
#include "../join/join.hpp"

namespace twisterx {
  ArrowJoin::ArrowJoin(int worker_id, const std::vector<int> &source, const std::vector<int> &targets, int leftEdgeId,
      int rightEdgeId, twisterx::JoinCallback *callback, std::shared_ptr<arrow::Schema> schema) {
    leftCallBack_ = std::make_shared<AllToAllCallback>(&leftTables_);
    rightCallBack_ = std::make_shared<AllToAllCallback>(&rightTables_);
    leftAllToAll_ = std::make_shared<ArrowAllToAll>(worker_id, source, targets, leftEdgeId, leftCallBack_.get(), schema);
    rightAllToAll_ = std::make_shared<ArrowAllToAll>(worker_id, source, targets, rightEdgeId, rightCallBack_.get(), schema);
  }

  bool ArrowJoin::isComplete() {
    bool left = leftAllToAll_->isComplete();
    bool right = rightAllToAll_->isComplete();

    std::shared_ptr<std::unordered_map<int64_t, arrow::ArrayBuilder>> column_builders;
    std::shared_ptr<std::unordered_map<int64_t, arrow::ArrayBuilder>> right_builders;
    if (left && right) {
      LOG(INFO) << "Received everything to join";
      join::join(leftTables_, rightTables_, (int64_t)0, (int64_t)0, join::JoinType::INNER, join::JoinAlgorithm::SORT,
                 arrow::default_memory_pool());
      // join
      return true;
    }
    return false;
  }

  AllToAllCallback::AllToAllCallback(std::vector<std::shared_ptr<arrow::Table>>* table) {
    tables_ = table;
  }

  bool AllToAllCallback::onReceive(int source, std::shared_ptr<arrow::Table> table) {
    tables_->push_back(table);
  }
}


