#include "io_provider.h"

#include "amounted_json_io.hpp"
#include "ingredient_json_io.hpp"
#include "recipe_json_io.hpp"

#include <boost/uuid/string_generator.hpp>

#include <algorithm>

namespace recipe {
namespace io {

void io_provider::setup()
{
  boost::uuids::string_generator gen;
  install(gen("b7ae614d-4f41-439a-b23f-b3c366416991"), "Json Shopping-list Format",
          std::make_shared<amounted_json_io>());
  install(gen("67af15f6-ea32-42e9-aa9e-27d4df5d804d"), "Json Ingredient Format",
          std::make_shared<ingredient_json_io>());
  install(gen("0e7b4a59-96cc-4fe4-8b61-cc80096ad698"), "Json Recipe Format",
          std::make_shared<recipe_json_io>());
}

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
