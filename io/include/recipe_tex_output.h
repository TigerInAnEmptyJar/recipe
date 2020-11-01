#pragma once

#include "plan.h"

#include <filesystem>
#include <iostream>
#include <iterator>

namespace recipe {
namespace io {

class recipe_tex_output
{
public:
  enum class plan_mode
  {
    plan_to_register,
    plan_final,
    plan_with_recipes,
    shoppinglist,
  };
  virtual void write(plan const& out, plan_mode mode, std::filesystem::path const& path) const = 0;
  virtual void write(plan const& out, plan_mode mode, std::ostream& output) const = 0;
  virtual void write(recipe const& out, std::filesystem::path const& path) const = 0;
  virtual void write(recipe const& out, std::ostream& output) const = 0;
  virtual void writeHeader(std::ostream& output) const = 0;
  virtual void writeFooter(std::ostream& output) const = 0;

  template <std::input_iterator it_t>
  void write(it_t b, it_t e, std::ostream& output)
  {
    std::for_each(b, e, [this, &output](auto r) { write(r, output); });
  }
};

} // namespace io
} // namespace recipe
