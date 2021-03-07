//
// Created by nira on 3/6/21.
//


#include <dist/api.hpp>

#include <arrow/api.h>

using namespace cylon::dist;

#define CHECK_STATUS(expr) \
  do{                      \
  const auto& st = (expr); \
  if(!st.IsOk()) return 1; \
  } while(0)

std::shared_ptr<arrow::Table> GetTable() {
  const std::vector<std::shared_ptr<arrow::Field>> schema_vector{};
  const std::vector<std::shared_ptr<arrow::Array>> col_vector{};
  const auto &schema = std::make_shared<arrow::Schema>(schema_vector);
  return arrow::Table::Make(schema, col_vector);
}

int main() {

  static const std::vector<int> WORLD{0, 1, 2, 4};

  // Make a context (MPI, ucx, local)
  const auto &local_ctx = std::make_shared<LocalContext>();
  const auto &ctx = std::make_shared<cylon::dist::Context>(0, 0, 0, WORLD, WORLD);

  // this is a local t1. Assume 10 rows
  const auto &t1 = GetTable();
  const auto &t2 = GetTable();
  std::shared_ptr<arrow::Table> output;

  /// let's split the world into 2 groups {[0, 1], [2,3]}
  // todo:: how to handle sub-division of groups
  std::shared_ptr<Context> ctx1;
  CHECK_STATUS(SplitContext(ctx, 0, [](int x) { return x / 2; }, ctx1));

  /// distributed properties
  int64_t length;
  CHECK_STATUS(DistLength(local_ctx, t1, &length)); // length = 10
  CHECK_STATUS(DistLength(ctx, t1, &length)); // length = 40
  CHECK_STATUS(DistLength(ctx1, t1, &length)); // length = 20

  /// dist unary op
  CHECK_STATUS(Sort(local_ctx, t1, {}, output)); // output = t1 local sort
  CHECK_STATUS(Sort(ctx, t1, {}, output)); // output = t1 sort globally
  CHECK_STATUS(Sort(ctx1, t1, {}, output)); // output = t1 sort in each group

  /// dist binary/ n-ary op
  CHECK_STATUS(Join(local_ctx, t1, t2, {}, output)); // output = local join
  CHECK_STATUS(Join(ctx, t1, t2, {}, output)); // output = t1 X t2 globally
  CHECK_STATUS(Join(ctx1, t1, t2, {}, output)); // output = t1 X t2 in each group


  if (ctx->GetGlobalRank() == 0) {
    CHECK_STATUS(Send(ctx, t1, 1, {})); // send t1 to process globally ranked 1
  } else if (ctx->GetGlobalRank() == 1) {
    CHECK_STATUS(Recv(ctx, 0, {}, output)); // output = recv t1 from process globally ranked 0
  }

  if (ctx->GetGroupRank() == 0) {
    CHECK_STATUS(Send(ctx, t1, 1, {})); // send t1 to process group rank 1. i.e. 0 --> 1 and 2 --> 3
  } else if (ctx->GetGroupRank() == 1) {
    CHECK_STATUS(Recv(ctx, 0, {}, output));
  }

  CHECK_STATUS(Broadcast(ctx, t1, 0, {}, output)); // everyone receives t1
  CHECK_STATUS(Broadcast(ctx1, t1, 0, {}, output)); // rank 0, 1 recv 0's t1 and rank 2, 3 recv 2's t1


  return 0;
}