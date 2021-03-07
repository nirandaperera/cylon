//
// Created by nira on 3/6/21.
//

#ifndef CYLON_CPP_SRC_CYLON_API_CONTEXT_HPP_
#define CYLON_CPP_SRC_CYLON_API_CONTEXT_HPP_

#include <utility>
#include <vector>
#include <memory>
#include <functional>

#include "dist/status.hpp"

namespace cylon {
namespace dist {

class Communicator {};

class Context {
 public:
  Context(int global_rank,
          int group_id,
          int group_rank,
          std::vector<int> group_memebers,
          std::vector<int> world_members)
      : global_rank(global_rank),
        group_id(group_id),
        group_rank(group_rank),
        group_members(std::move(group_memebers)),
        world_members(std::move(world_members)),
        communicator({}) {}

  int GetGlobalRank() const {
    return global_rank;
  }

  int GetGroupId() const {
    return group_id;
  }
  int GetGroupRank() const {
    return group_rank;
  }

  int GetGroupSize() const {
    return group_members.size();
  };

  int GetWorldSize() const {
    return world_members.size();
  }

  const std::vector<int> &GetGroupMembers() const {
    return group_members;
  }
  const std::vector<int> &GetWorldMembers() const {
    return world_members;
  }

  const std::shared_ptr<Communicator> &GetCommunicator() const {
    return communicator;
  }

 private:
  int global_rank, group_id, group_rank;
  const std::vector<int> group_members, world_members;
  std::shared_ptr<Communicator> communicator;
};

class LocalContext : public Context {
 public:
  LocalContext() : Context(0, 0, 0, {0}, {0}) {}
};

static Status SplitContext(const std::shared_ptr<Context> &source_ctx,
                           int num_groups,
                           const std::function<int(int)> &rank_mapper,
                           std::shared_ptr<Context> &output_ctx) {
  return Status(true);
}

}
}

#endif //CYLON_CPP_SRC_CYLON_API_CONTEXT_HPP_
