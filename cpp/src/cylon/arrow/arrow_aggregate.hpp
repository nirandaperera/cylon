//
// Created by niranda on 8/13/20.
//

#ifndef CYLON_CPP_SRC_CYLON_ARROW_ARROW_AGGREGATE_HPP_
#define CYLON_CPP_SRC_CYLON_ARROW_ARROW_AGGREGATE_HPP_

namespace cylon{
namespace aggregate{

template<class ARROW_ARRAY_TYPE, typename CTYPE>
class ArrowArraySumKernel {
  int execute(const std::shared_ptr<arrow::Array> &left_idx_col, CTYPE *output){
    return 0;
  }
}


}
}

#endif //CYLON_CPP_SRC_CYLON_ARROW_ARROW_AGGREGATE_HPP_
