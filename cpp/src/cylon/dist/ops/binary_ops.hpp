//
// Created by nira on 3/6/21.
//

#ifndef CYLON_CPP_SRC_CYLON_API_BINARY_OPS_HPP_
#define CYLON_CPP_SRC_CYLON_API_BINARY_OPS_HPP_

#include <dist/ctx/context.hpp>

namespace cylon {
namespace dist {

/// returns a scalar
static Status Dot(const std::shared_ptr<Context> &ctx,
                  const std::shared_ptr<arrow::Array> &x,
                  const std::shared_ptr<arrow::Array> &y,
                  std::shared_ptr<arrow::Scalar> &result) { return {}; }

struct EqualsConfig {};
static Status Equals(const std::shared_ptr<Context> &ctx,
                     const std::shared_ptr<arrow::Array> &x,
                     const std::shared_ptr<arrow::Array> &y,
                     std::shared_ptr<arrow::BooleanScalar> &result) { return {}; }
static Status Equals(const std::shared_ptr<Context> &ctx,
                     const std::shared_ptr<arrow::Table> &x,
                     const std::shared_ptr<arrow::Table> &y,
                     std::shared_ptr<arrow::BooleanScalar> &result) { return {}; }

/// returns an array
static Status Sum(const std::shared_ptr<Context> &ctx,
                  const std::shared_ptr<arrow::Scalar> &x,
                  const std::shared_ptr<arrow::Scalar> &y,
                  const SumConfig &config,
                  std::shared_ptr<arrow::Array> &result) { return {}; } // or struct_scalar?

struct UnionConfig {};
static Status Union(const std::shared_ptr<Context> &ctx,
                    const std::shared_ptr<arrow::Array> &x,
                    const std::shared_ptr<arrow::Array> &y,
                    const UnionConfig &union_config,
                    std::shared_ptr<arrow::Array> &result) { return {}; }


/// returns a table

struct JoinConfig {};
static Status Join(const std::shared_ptr<Context> &ctx,
                   const std::shared_ptr<arrow::Table> &left_table,
                   const std::shared_ptr<arrow::Table> &right_table,
                   const JoinConfig &config,
                   std::shared_ptr<arrow::Table> &result) { return {}; }

static Status Union(const std::shared_ptr<Context> &ctx,
                    const std::shared_ptr<arrow::Table> &left_table,
                    const std::shared_ptr<arrow::Table> &right_table,
                    const UnionConfig &config,
                    std::shared_ptr<arrow::Table> &result) { return {}; }

}
}

#endif //CYLON_CPP_SRC_CYLON_API_BINARY_OPS_HPP_
