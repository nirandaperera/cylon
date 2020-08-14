//
// Created by niranda on 8/13/20.
//

#ifndef CYLON_CPP_SRC_CYLON_AGGREGATE_HPP_
#define CYLON_CPP_SRC_CYLON_AGGREGATE_HPP_

#include <memory>
#include <arrow/table.h>

namespace cylon {
namespace aggregate {

enum Operator {
  SUM,
  COUNT,
  MIN,
  MAX,
  PROD,
};

arrow::Status Reduce(const std::shared_ptr<arrow::Table> &in,
                     const std::vector<int64_t> &project_columns,
                     const std::vector<Operator> &operators,
                     std::shared_ptr<arrow::Table> &output,
                     arrow::MemoryPool *memory_pool);

}
}

#endif //CYLON_CPP_SRC_CYLON_AGGREGATE_HPP_
