//
// Created by nira on 3/6/21.
//

#ifndef CYLON_CPP_SRC_CYLON_API_N_ARY_OPS_HPP_
#define CYLON_CPP_SRC_CYLON_API_N_ARY_OPS_HPP_

namespace cylon {
namespace dist {

/// these are same as binary ops

struct MergeConfig {};
static Status Merge(const std::shared_ptr<Context> &ctx,
                    const std::vector<std::shared_ptr<arrow::Table>> &tables,
                    const MergeConfig &config,
                    std::shared_ptr<arrow::Table> &result) {
  return {};
}

}
}

#endif //CYLON_CPP_SRC_CYLON_API_N_ARY_OPS_HPP_
