#pragma once

#include "plan.h"

#include <filesystem>
#include <functional>
#include <optional>
#include <vector>

namespace recipe {
namespace io {

class plan_io
{
public:
  //! The function-type for providing access to the stored recipes.
  using finder_t = std::function<std::optional<recipe>(boost::uuids::uuid)>;

  virtual void write(plan const& out, std::filesystem::path const& path) const = 0;

  virtual std::optional<plan> read(std::filesystem::path const& path,
                                   finder_t recipe_finder) const = 0;
};

} // namespace io
} // namespace recipe
