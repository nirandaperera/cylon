//
// Created by niranda on 8/13/20.
//

#include "aggregate.hpp"

namespace cylon {
namespace aggregate {
arrow::Status Reduce(const std::shared_ptr<arrow::Table> &in,
                     const std::vector<int64_t> &project_columns,
                     const std::vector<Operator> &operators,
                     std::shared_ptr<arrow::Table> &output,
                     arrow::MemoryPool *memory_pool) {
  return arrow::Status::OK();
}
}
}