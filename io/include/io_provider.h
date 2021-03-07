#pragma once

#include "amounted_io.h"
#include "ingredient_io.h"
#include "plan_io.h"
#include "recipe_io.h"
#include "shopping_io.h"

#include <boost/uuid/uuid.hpp>

#include <map>
#include <memory>

namespace recipe {
namespace io {

/*!
 * \brief The io_provider class
 * This class provides access to all installed input/output formats.
 */
class io_provider
{
public:
  void setup();

  std::shared_ptr<amounted_io> amounted(boost::uuids::uuid id) const;
  std::shared_ptr<ingredient_io> ingredient(boost::uuids::uuid id) const;
  std::shared_ptr<recipe_io> recipe(boost::uuids::uuid id) const;
  std::shared_ptr<plan_io> plan(boost::uuids::uuid id) const;
  std::shared_ptr<shopping_io> shopping(boost::uuids::uuid id) const;

  void install(boost::uuids::uuid id, std::string const& name, std::shared_ptr<amounted_io> io);
  void install(boost::uuids::uuid id, std::string const& name, std::shared_ptr<ingredient_io> io);
  void install(boost::uuids::uuid id, std::string const& name, std::shared_ptr<recipe_io> io);
  void install(boost::uuids::uuid id, std::string const& name, std::shared_ptr<plan_io> io);
  void install(boost::uuids::uuid id, std::string const& name, std::shared_ptr<shopping_io> io);

  void uninstall_amounted(boost::uuids::uuid id);
  void uninstall_ingredient(boost::uuids::uuid id);
  void uninstall_recipe(boost::uuids::uuid id);
  void uninstall_plan(boost::uuids::uuid id);
  void uninstall_shopping(boost::uuids::uuid id);

  std::map<boost::uuids::uuid, std::pair<std::string, std::shared_ptr<amounted_io>>>
  installed_amounted() const;
  std::map<boost::uuids::uuid, std::pair<std::string, std::shared_ptr<ingredient_io>>>
  installed_ingredient() const;
  std::map<boost::uuids::uuid, std::pair<std::string, std::shared_ptr<recipe_io>>> installed_recipe()
      const;
  std::map<boost::uuids::uuid, std::pair<std::string, std::shared_ptr<plan_io>>> installed_plan()
      const;
  std::map<boost::uuids::uuid, std::pair<std::string, std::shared_ptr<shopping_io>>>
  installed_shopping() const;

private:
  std::map<boost::uuids::uuid, std::pair<std::string, std::shared_ptr<amounted_io>>> _amounted;
  std::map<boost::uuids::uuid, std::pair<std::string, std::shared_ptr<ingredient_io>>> _ingredient;
  std::map<boost::uuids::uuid, std::pair<std::string, std::shared_ptr<recipe_io>>> _recipe;
  std::map<boost::uuids::uuid, std::pair<std::string, std::shared_ptr<plan_io>>> _plan;
  std::map<boost::uuids::uuid, std::pair<std::string, std::shared_ptr<shopping_io>>> _shopping;
};

} // namespace io
} // namespace recipe
