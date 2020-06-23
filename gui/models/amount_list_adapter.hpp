#pragma once

#include "amounted_ingredient.h"

namespace recipe {
namespace gui {

class amount_list_adapter
{
public:
  virtual size_t size() = 0;
  virtual amounted_ingredient& at(size_t i) = 0;
  virtual amounted_ingredient const& at(size_t i) const = 0;
  virtual bool singleAmount() const = 0;
};

template <typename T>
class amount_list_adapter_impl : public amount_list_adapter
{
public:
  amount_list_adapter_impl(std::shared_ptr<T> obj);
  size_t size() override { return _object.size(); }
  amounted_ingredient& at(size_t i) override { return _object[i]; }
  amounted_ingredient const& at(size_t i) const override { return _object[i]; }
  bool singleAmount() const override { return false; }

private:
  std::shared_ptr<T> _object;
};

} // namespace gui
} // namespace recipe
