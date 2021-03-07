//
// Created by nira on 3/6/21.
//

#ifndef CYLON_CPP_SRC_CYLON_API_DIST_PROPERTIES_HPP_
#define CYLON_CPP_SRC_CYLON_API_DIST_PROPERTIES_HPP_

#include <arrow/api.h>

namespace cylon {
namespace dist {

static Status DistLength(const std::shared_ptr<Context> &ctx,
                         const std::shared_ptr<arrow::Array> &array,
                         int64_t *dist_length) {

  return {};
}

}
}

#endif //CYLON_CPP_SRC_CYLON_API_DIST_PROPERTIES_HPP_
