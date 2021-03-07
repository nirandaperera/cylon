//
// Created by nira on 3/6/21.
//

#ifndef CYLON_CPP_SRC_CYLON_API_STATUS_HPP_
#define CYLON_CPP_SRC_CYLON_API_STATUS_HPP_

namespace cylon {
namespace dist {

class Status{
 public:
  Status():is_ok(true){}

  explicit Status(bool is_ok) : is_ok(is_ok) {}

  bool IsOk() const {
    return is_ok;
  }

 private:
  bool is_ok;
};

}
}

#endif //CYLON_CPP_SRC_CYLON_API_STATUS_HPP_
