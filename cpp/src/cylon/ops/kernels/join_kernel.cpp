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

#include "ops/kernels/join_kernel.hpp"

#include <utility>

namespace cylon {
namespace kernel {

JoinKernel::JoinKernel(const std::shared_ptr<cylon::CylonContext> &ctx,
           const std::shared_ptr<arrow::Schema> &schema,
           const std::shared_ptr<cylon::join::config::JoinConfig> &join_config) {
  this->ctx = ctx;
  this->schema = schema;
  this->join_config = join_config;
}

void JoinKernel::InsertTable(int tag, std::shared_ptr<cylon::Table> table) {
  if (tag == 100) {
    left_tables.push_back(table->get_table());
  } else if (tag == 200) {
    right_tables.push_back(table->get_table());
  } else {
    LOG(FATAL) << "Un-recognized tag "<< tag;
  }   
}

cylon::Status JoinKernel::Finalize(std::shared_ptr<cylon::Table> &result) {
  size_t kI = left_tables.size();
  std::vector<std::shared_ptr<arrow::Table>> joined_tables;
  arrow::MemoryPool *kPool = cylon::ToArrowPool(this->ctx.get());
  for (size_t i = 0; i < kI; i++) {
    std::shared_ptr<arrow::Table> left_tab = left_tables[i];
    std::shared_ptr<arrow::Table> right_tab = right_tables[i];
    std::shared_ptr<arrow::Table> out;
    arrow::Status st = cylon::join::joinTables(left_tab, right_tab, *join_config, &out, kPool);
    joined_tables.push_back(out);
  }
  arrow::Result<std::shared_ptr<arrow::Table>> concat_tables =
      arrow::ConcatenateTables(joined_tables, arrow::ConcatenateTablesOptions::Defaults(), kPool);

  if (concat_tables.ok()) {
    auto final_table = concat_tables.ValueOrDie();
    LOG(INFO) << "Done concatenating tables, rows :  " << final_table->num_rows();
    cylon::Table::FromArrowTable(this->ctx.get(), final_table, &result);
    return Status(static_cast<int>(concat_tables.status().code()),
        concat_tables.status().message());
  } else {
    return Status(static_cast<int>(concat_tables.status().code()),
                  concat_tables.status().message());
  }
}

}  // namespace kernel
}  // namespace cylon
