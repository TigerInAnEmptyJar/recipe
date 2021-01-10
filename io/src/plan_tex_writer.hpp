#pragma once

#include "plan.h"
#include "plan_io.h"

#include <filesystem>

namespace recipe {
namespace io {

class plan_tex_writer_register : public plan_io
{
  void write(plan const& out, std::filesystem::path const& path) const override;

  std::optional<plan> read(std::filesystem::path const& path,
                           finder_t recipe_finder) const override;
};

class plan_tex_writer_final : public plan_io
{
  void write(plan const& out, std::filesystem::path const& path) const override;

  std::optional<plan> read(std::filesystem::path const& path,
                           finder_t recipe_finder) const override;
};

class plan_tex_writer_with_recipes : public plan_io
{
  void write(plan const& out, std::filesystem::path const& path) const override;

  std::optional<plan> read(std::filesystem::path const& path,
                           finder_t recipe_finder) const override;
};

} // namespace io
} // namespace recipe
