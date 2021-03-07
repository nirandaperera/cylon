//
// Created by nira on 3/6/21.
//

#ifndef CYLON_CPP_SRC_CYLON_API_COMM_HPP_
#define CYLON_CPP_SRC_CYLON_API_COMM_HPP_

#include "arrow/api.h"

namespace cylon {
namespace dist {

struct SendConfig {};
static Status Send(const std::shared_ptr<Context> &ctx,
                   const std::shared_ptr<arrow::Scalar> &x,
                   int to,
                   const SendConfig &config) { return {}; }
static Status Send(const std::shared_ptr<Context> &ctx,
                   const std::shared_ptr<arrow::Array> &x,
                   int to,
                   const SendConfig &config) { return {}; }
static Status Send(const std::shared_ptr<Context> &ctx,
                   const std::shared_ptr<arrow::Table> &x,
                   int to,
                   const SendConfig &config) { return {}; }

struct RecvConfig {};
static Status Recv(const std::shared_ptr<Context> &ctx,
                   int from,
                   const RecvConfig &config,
                   std::shared_ptr<arrow::Scalar> &output) { return {}; }
static Status Recv(const std::shared_ptr<Context> &ctx,
                   int from,
                   const RecvConfig &config,
                   std::shared_ptr<arrow::Array> &output) { return {}; }
static Status Recv(const std::shared_ptr<Context> &ctx,
                   int from,
                   const RecvConfig &config,
                   std::shared_ptr<arrow::Table> &output) { return {}; }

struct ShuffleConfig {};
static Status Shuffle(const std::shared_ptr<Context> &ctx,
                      const std::shared_ptr<arrow::Array> &array,
                      const ShuffleConfig &config,
                      std::shared_ptr<arrow::Array> &output) { return {}; }
static Status Shuffle(const std::shared_ptr<Context> &ctx,
                      const std::shared_ptr<arrow::Table> &array,
                      const ShuffleConfig &config,
                      std::shared_ptr<arrow::Table> &output) { return {}; }

struct AllReduceConfig {};
static Status AllReduce(const std::shared_ptr<Context> &ctx,
                        const std::shared_ptr<arrow::Scalar> &array,
                        const AllReduceConfig &config,
                        std::shared_ptr<arrow::Scalar> &output) { return {}; }
static Status AllReduce(const std::shared_ptr<Context> &ctx,
                        const std::shared_ptr<arrow::Array> &array,
                        const AllReduceConfig &config,
                        std::shared_ptr<arrow::Array> &output) { return {}; }

struct AllGatherConfig {};
static Status AllGather(const std::shared_ptr<Context> &ctx,
                        const std::shared_ptr<arrow::Scalar> &x,
                        const AllGatherConfig &config,
                        std::vector<std::shared_ptr<arrow::Scalar>> &output) { return {}; }
static Status AllGather(const std::shared_ptr<Context> &ctx,
                        const std::shared_ptr<arrow::Array> &x,
                        const AllGatherConfig &config,
                        std::vector<std::shared_ptr<arrow::Array>> &output) { return {}; }
static Status AllGather(const std::shared_ptr<Context> &ctx,
                        const std::shared_ptr<arrow::Table> &x,
                        const AllGatherConfig &config,
                        std::vector<std::shared_ptr<arrow::Table>> &output) { return {}; }

struct BroadcastConfig {};
static Status Broadcast(const std::shared_ptr<Context> &ctx,
                        const std::shared_ptr<arrow::Scalar> &x,
                        int root,
                        const BroadcastConfig &config,
                        std::shared_ptr<arrow::Scalar> &output) { return {}; }
static Status Broadcast(const std::shared_ptr<Context> &ctx,
                        const std::shared_ptr<arrow::Array> &x,
                        int root,
                        const BroadcastConfig &config,
                        std::shared_ptr<arrow::Array> &output) { return {}; }
static Status Broadcast(const std::shared_ptr<Context> &ctx,
                        const std::shared_ptr<arrow::Table> &x,
                        int root,
                        const BroadcastConfig &config,
                        std::shared_ptr<arrow::Table> &output) { return {}; }
}
}

#endif //CYLON_CPP_SRC_CYLON_API_COMM_HPP_
