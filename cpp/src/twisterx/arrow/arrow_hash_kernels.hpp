/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// Created by nira on 5/21/20.
//

#ifndef TWISTERX_CPP_SRC_TWISTERX_ARROW_ARROW_HASH_KERNELS_HPP_
#define TWISTERX_CPP_SRC_TWISTERX_ARROW_ARROW_HASH_KERNELS_HPP_

#include <arrow/api.h>
#include <arrow/compute/kernel.h>
#include <glog/logging.h>
#include "../status.hpp"
#include "../join/join_config.h"
#include "iostream"
#include <unordered_set>
#include <chrono>
#include <list>

namespace twisterx {

/**
 * Kernel to join indices using hashing
 * @tparam ARROW_ARRAY_TYPE arrow array type to be used for static type casting
 */
template<class ARROW_ARRAY_TYPE>
class ArrowArrayIdxHashJoinKernel {
 public:
  using ARROW_TYPE = typename ARROW_ARRAY_TYPE::TypeClass;
  using CTYPE = typename ARROW_TYPE::c_type;
  using MMAP_TYPE = typename std::unordered_multimap<CTYPE, int64_t>;

  /**
   * perform index hash join
   * @param left_idx_col
   * @param right_idx_col
   * @param join_type
   * @param left_table_indices row indices of the left table
   * @param right_table_indices row indices of the right table
   * @return 0 if success; non-zero otherwise
   */
  int IdxHashJoin(const std::shared_ptr<arrow::Array> &left_idx_col,
                  const std::shared_ptr<arrow::Array> &right_idx_col,
                  const twisterx::join::config::JoinType join_type,
                  std::shared_ptr<std::vector<int64_t>> &left_table_indices,
                  std::shared_ptr<std::vector<int64_t>> &right_table_indices) {

//    std::unique_ptr<MMAP_TYPE> out_umm_ptr;

    switch (join_type) {
      case twisterx::join::config::JoinType::RIGHT: {
        // build hashmap using left col idx
        MMAP_TYPE out_umm_ptr = MMAP_TYPE(left_idx_col->length());
        BuildPhase(left_idx_col, out_umm_ptr);
        ProbePhase(out_umm_ptr, right_idx_col, left_table_indices, right_table_indices);
        break;
      }
      case twisterx::join::config::JoinType::LEFT: {
        // build hashmap using right col idx
        MMAP_TYPE out_umm_ptr = MMAP_TYPE(right_idx_col->length());
        BuildPhase(right_idx_col, out_umm_ptr);
        ProbePhase(out_umm_ptr, left_idx_col, right_table_indices, left_table_indices);
        break;
      }
      case twisterx::join::config::JoinType::INNER: {
        // build hashmap using col idx with smaller len
        if (left_idx_col->length() < right_idx_col->length()) {
          MMAP_TYPE out_umm_ptr = MMAP_TYPE(left_idx_col->length());
          BuildPhase(left_idx_col, out_umm_ptr);
          ProbePhaseNoFill(out_umm_ptr, right_idx_col, left_table_indices, right_table_indices);
        } else {
          MMAP_TYPE out_umm_ptr = MMAP_TYPE(right_idx_col->length());
          BuildPhase(right_idx_col, out_umm_ptr);
          ProbePhaseNoFill(out_umm_ptr, left_idx_col, right_table_indices, left_table_indices);
        }
        break;
      }
      case twisterx::join::config::JoinType::FULL_OUTER: {
        // build hashmap using col idx with smaller len

        // a key set to track matched keys from the other table
        // todo: use an index vector rather than a key set!
//        std::unordered_set<CTYPE> key_set;
//        std::unique_ptr<std::unordered_set<typename MMAP_TYPE::iterator>> key_set;

        if (left_idx_col->length() < right_idx_col->length()) {
          MMAP_TYPE out_umm_ptr = MMAP_TYPE(left_idx_col->length());
          std::unordered_set<CTYPE> key_set = std::unordered_set<CTYPE>(left_idx_col->length());
          BuildPhase(left_idx_col, out_umm_ptr, key_set);
          ProbePhaseOuter(out_umm_ptr, right_idx_col, key_set, left_table_indices, right_table_indices);
        } else {
          MMAP_TYPE out_umm_ptr = MMAP_TYPE(right_idx_col->length());
          std::unordered_set<CTYPE> key_set = std::unordered_set<CTYPE>(right_idx_col->length());
          BuildPhase(right_idx_col, out_umm_ptr, key_set);
          ProbePhaseOuter(out_umm_ptr, left_idx_col, key_set, right_table_indices, left_table_indices);
        }
        break;
      }
      default: {
        LOG(ERROR) << "not implemented!";
        return 1;
      }
    }
    return 0;
  }

 private:
  // build hashmap
  void BuildPhase(const std::shared_ptr<arrow::Array> &smaller_idx_col,
                  MMAP_TYPE &smaller_idx_map) {
    auto t1 = std::chrono::high_resolution_clock::now();
    auto reader0 = std::static_pointer_cast<ARROW_ARRAY_TYPE>(smaller_idx_col);

//    smaller_idx_map = std::make_unique<MMAP_TYPE>(smaller_idx_col->length());

    for (int64_t i = 0; i < reader0->length(); i++) {
      auto lValue = reader0->Value(i);
      auto val = (CTYPE) lValue;
      smaller_idx_map.insert(std::make_pair(val, i));
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    LOG(INFO) << "build_phase " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1)
        .count();
  }

  // builds hashmap as well as populate keyset
  void BuildPhase(const std::shared_ptr<arrow::Array> &smaller_idx_col,
                  MMAP_TYPE &smaller_idx_map,
                  std::unordered_set<CTYPE> &smaller_key_set) {
    auto t1 = std::chrono::high_resolution_clock::now();
    auto reader0 = std::static_pointer_cast<ARROW_ARRAY_TYPE>(smaller_idx_col);

//    smaller_idx_map = std::make_unique<MMAP_TYPE>(smaller_idx_col->length());
//    smaller_key_set = std::make_unique<std::unordered_set<CTYPE>>(smaller_idx_col->length());

    for (int64_t i = 0; i < reader0->length(); i++) {
      auto lValue = reader0->Value(i);
      auto val = (CTYPE) lValue;
      smaller_idx_map.insert(std::make_pair(val, i));
      smaller_key_set.emplace(val);
//      smaller_key_set->emplace(it); // save all iter positions
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    LOG(INFO) << "build_phase " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1)
        .count();
  }

  // probes hashmap and fill -1 for no matches
  void ProbePhase(const MMAP_TYPE &smaller_idx_map,
                  const std::shared_ptr<arrow::Array> &larger_idx_col,
                  std::shared_ptr<std::vector<int64_t>> &smaller_output,
                  std::shared_ptr<std::vector<int64_t>> &larger_output) {
    auto t1 = std::chrono::high_resolution_clock::now();
    auto reader1 = std::static_pointer_cast<ARROW_ARRAY_TYPE>(larger_idx_col);
    for (int64_t i = 0; i < reader1->length(); ++i) {
      auto val = (CTYPE) reader1->Value(i);
      const auto range = smaller_idx_map.equal_range(val);
      if (range.first == range.second) {
        smaller_output->push_back(-1);
        larger_output->push_back(i);
      } else {
        for (auto it = range.first; it != range.second; it++) {
          smaller_output->push_back(it->second);
          larger_output->push_back(i);
        }
      }
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    LOG(INFO) << "probe_phase " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1)
        .count();
  }

  // probes hashmap with no filling
  void ProbePhaseNoFill(const MMAP_TYPE &smaller_idx_map,
                        const std::shared_ptr<arrow::Array> &larger_idx_col,
                        std::shared_ptr<std::vector<int64_t>> &smaller_table_indices,
                        std::shared_ptr<std::vector<int64_t>> &larger_table_indices) {
    auto t1 = std::chrono::high_resolution_clock::now();
    auto reader1 = std::static_pointer_cast<ARROW_ARRAY_TYPE>(larger_idx_col);
    for (int64_t i = 0; i < reader1->length(); ++i) {
      auto val = (CTYPE) reader1->Value(i);
      auto range = smaller_idx_map.equal_range(val);
      for (auto it = range.first; it != range.second; it++) {
        smaller_table_indices->push_back(it->second);
        larger_table_indices->push_back(i);
      }
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    LOG(INFO) << "probe_phase " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1)
        .count();
  }

  // probes hashmap and removes matched keys from the keyset. Then traverses the remaining keys in the keyset and
  // fill with -1
  void ProbePhaseOuter(const MMAP_TYPE &smaller_idx_map,
                       const std::shared_ptr<arrow::Array> &larger_idx_col,
                       std::unordered_set<CTYPE> &smaller_key_set,
                       std::shared_ptr<std::vector<int64_t>> &smaller_table_indices,
                       std::shared_ptr<std::vector<int64_t>> &larger_table_indices) {
    auto t1 = std::chrono::high_resolution_clock::now();
    auto reader1 = std::static_pointer_cast<arrow::NumericArray<ARROW_TYPE>>(larger_idx_col);
    for (int64_t i = 0; i < reader1->length(); ++i) {
      auto val = (CTYPE) reader1->Value(i);
      auto range = smaller_idx_map.equal_range(val);
      if (range.first == range.second) {
        smaller_table_indices->push_back(-1);
        larger_table_indices->push_back(i);
      } else {
        for (auto it = range.first; it != range.second; it++) {
          smaller_key_set.erase(it->first); // todo: this erase would be inefficient
          smaller_table_indices->push_back(it->second);
          larger_table_indices->push_back(i);
        }
      }
    }

    // fill the remaining keys with -1
    // todo: use an index vector rather than a key set! this second probe is inefficient!
    for (auto it = smaller_key_set.begin(); it != smaller_key_set.end(); it++) {
      auto range = smaller_idx_map.equal_range(*it);
      for (auto it2 = range.first; it2 != range.second; it2++) {
        smaller_table_indices->push_back(it2->second);
        larger_table_indices->push_back(-1);
      }
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    LOG(INFO) << "probe_phase " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1)
        .count();
  }

  int IdxHashJoinOuter(const std::shared_ptr<arrow::Array> &left_idx_col,
                       const std::shared_ptr<arrow::Array> &right_idx_col,
                       std::shared_ptr<std::vector<std::list<int64_t>*>> &left_table_indices) {

    left_table_indices->resize(left_idx_col->length(), NULLPTR);
    MMAP_TYPE mmapleft;
    auto reader0 = std::static_pointer_cast<ARROW_ARRAY_TYPE>(left_idx_col);
    for (uint64_t i = 0; i < reader0->length(); i++) {
      auto lValue = reader0->Value(i);
      auto val = (CTYPE) lValue;
      mmapleft.insert(std::make_pair(val, i));
    }

    auto reader1 = std::static_pointer_cast<ARROW_ARRAY_TYPE>(right_idx_col);
    for (uint64_t i = 0; i < reader1->length(); i++) {
      auto lValue = reader1->Value(i);
      auto val = (CTYPE) lValue;
      const auto range = mmapleft.equal_range(val);
      for (auto it = range.first; it != range.second; it++) {
        if (left_table_indices[it->second] == NULL)
          left_table_indices[it->second] = new std::list<uint64_t>(); // make sure to de-allocate memory ...
        left_table_indices[it->second].push_back(i);
      }
    }
  }

  Table generateJoinTable(std::vector<std::list<uint64_t>* > &left_table_indices, Table t1, Table t2) {
    Table out;
    std::vector<bool> rightSelected(t2.num_row(), false);
    for(int i=0; i < left_table_indices.size(); ++i) {
      Row rLeft = t1.get(i);
      if (left_table_indices[i] != NULL) {
        for(auto it = left_table_indices[i]->begin(); it != left_table_indices[i]->end(); ++it) {
          uint64_t rightTabRowId = (*it);
          Row rRight = t2.get(rightTabRowId);
          out.insert(combine(rLeft, rRight));
          rightSelected[rightTabRowId] = true;
        }

        delete left_table_indices[i];
      } else
        out.insert(combine(rLeft, NULL));
    }

    for(int i=0; i < rightSelected.size(); ++i) {
      if (!rightSelected[i]) {
        Row rowRight = t2.get(i);
        out.insert(combine(NULL, rowRight))
      }
    }

    return out;
  }
};
}
#endif //TWISTERX_CPP_SRC_TWISTERX_ARROW_ARROW_HASH_KERNELS_HPP_
