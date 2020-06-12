#include "io_provider.h"

#include <algorithm>

namespace recipe {
namespace io {

std::shared_ptr<amounted_io> io_provider::amounted(boost::uuids::uuid id) const
{
  auto element = std::find_if(std::begin(_amounted), std::end(_amounted),
                              [id](auto item) { return item.first == id; });
  if (element != std::end(_amounted)) {
    return element->second.second;
  }
  return {};
}

std::shared_ptr<ingredient_io> io_provider::ingredient(boost::uuids::uuid id) const
{
  auto element = std::find_if(std::begin(_ingredient), std::end(_ingredient),
                              [id](auto item) { return item.first == id; });
  if (element != std::end(_ingredient)) {
    return element->second.second;
  }
  return {};
}

std::shared_ptr<recipe_io> io_provider::recipe(boost::uuids::uuid id) const
{
  auto element = std::find_if(std::begin(_recipe), std::end(_recipe),
                              [id](auto item) { return item.first == id; });
  if (element != std::end(_recipe)) {
    return element->second.second;
  }
  return {};
}

void io_provider::install(boost::uuids::uuid id, std::string const& name,
                          std::shared_ptr<amounted_io> io)
{
  auto element = std::find_if(std::begin(_amounted), std::end(_amounted),
                              [id](auto item) { return item.first == id; });
  if (element == std::end(_amounted)) {
    _amounted.insert(std::make_pair(id, std::make_pair(name, io)));
  }
}

void io_provider::install(boost::uuids::uuid id, std::string const& name,
                          std::shared_ptr<ingredient_io> io)
{
  auto element = std::find_if(std::begin(_ingredient), std::end(_ingredient),
                              [id](auto item) { return item.first == id; });
  if (element == std::end(_ingredient)) {
    _ingredient.insert(std::make_pair(id, std::make_pair(name, io)));
  }
}

void io_provider::install(boost::uuids::uuid id, std::string const& name,
                          std::shared_ptr<recipe_io> io)
{
  auto element = std::find_if(std::begin(_recipe), std::end(_recipe),
                              [id](auto item) { return item.first == id; });
  if (element == std::end(_recipe)) {
    _recipe.insert(std::make_pair(id, std::make_pair(name, io)));
  }
}

void io_provider::uninstall_amounted(boost::uuids::uuid id)
{
  auto element = std::find_if(std::begin(_amounted), std::end(_amounted),
                              [id](auto item) { return item.first == id; });
  if (element != std::end(_amounted)) {
    _amounted.erase(element);
  }
}

void io_provider::uninstall_ingredient(boost::uuids::uuid id)
{
  auto element = std::find_if(std::begin(_ingredient), std::end(_ingredient),
                              [id](auto item) { return item.first == id; });
  if (element != std::end(_ingredient)) {
    _ingredient.erase(element);
  }
}

void io_provider::uninstall_recipe(boost::uuids::uuid id)
{
  auto element = std::find_if(std::begin(_recipe), std::end(_recipe),
                              [id](auto item) { return item.first == id; });
  if (element != std::end(_recipe)) {
    _recipe.erase(element);
  }
}

std::map<boost::uuids::uuid, std::pair<std::string, std::shared_ptr<amounted_io>>> io_provider::
    installed_amounted() const
{
  return _amounted;
}

std::map<boost::uuids::uuid, std::pair<std::string, std::shared_ptr<ingredient_io>>> io_provider::
    installed_ingredient() const
{
  return _ingredient;
}

std::map<boost::uuids::uuid, std::pair<std::string, std::shared_ptr<recipe_io>>> io_provider::
    installed_recipe() const
{
  return _recipe;
}

} // namespace io
} // namespace recipe
