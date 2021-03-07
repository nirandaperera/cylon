//
// Created by nira on 3/6/21.
//

#ifndef CYLON_CPP_SRC_CYLON_API_UNARY_OPS_HPP_
#define CYLON_CPP_SRC_CYLON_API_UNARY_OPS_HPP_

#include <dist/ctx/context.hpp>

namespace cylon {
namespace dist {

/// returns a Scalar
struct SumConfig {};
static Status Sum(const std::shared_ptr<Context> &ctx,
                  const std::shared_ptr<arrow::Scalar> &scalar,
                  const SumConfig &config,
                  std::shared_ptr<arrow::Scalar> &result) { return {}; }
static Status Sum(const std::shared_ptr<Context> &ctx,
                  const std::shared_ptr<arrow::Array> &array,
                  const SumConfig &config,
                  std::shared_ptr<arrow::Scalar> &result) { return {}; }

/// returns a series
struct IndexSortConfig {};
static Status IndexSort(const std::shared_ptr<Context> &ctx,
                        const std::shared_ptr<arrow::Array> &array,
                        const IndexSortConfig &config,
                        std::shared_ptr<arrow::Int64Array> &result) { return {}; }
static Status IndexSort(const std::shared_ptr<Context> &ctx,
                        const std::shared_ptr<arrow::Table> &table,
                        const IndexSortConfig &config,
                        std::shared_ptr<arrow::Int64Array> &result) { return {}; }

struct SortConfig {};
static Status Sort(const std::shared_ptr<Context> &ctx,
                   const std::shared_ptr<arrow::Array> &array,
                   const SortConfig &config,
                   std::shared_ptr<arrow::Array> &result) { return {}; }

/// returns a Table

static Status Sort(const std::shared_ptr<Context> &ctx,
                   const std::shared_ptr<arrow::Table> &table,
                   const SortConfig &config,
                   std::shared_ptr<arrow::Table> &result) { return {}; }

struct GroupByConfig {};
static Status GroupBy(const std::shared_ptr<Context> &ctx,
                      const std::shared_ptr<arrow::Table> &table,
                      const GroupByConfig &config,
                      std::shared_ptr<arrow::Table> &result) { return {}; }

struct ValueCountsConfig {};
static Status ValueCounts(const std::shared_ptr<Context> &ctx,
                          const std::shared_ptr<arrow::Array> &array,
                          const ValueCountsConfig &config,
                          std::shared_ptr<arrow::Table> &result) { return {}; }

}
}

#endif //CYLON_CPP_SRC_CYLON_API_UNARY_OPS_HPP_
