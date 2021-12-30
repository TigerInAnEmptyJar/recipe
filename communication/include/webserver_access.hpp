#pragma once

#include <functional>
#include <optional>
#include <string>

#include "plan.h"
#include "shopping_list.h"

namespace recipe::communication {

class webserver_access
{
public:
  webserver_access(std::string const& address, std::string const& port);
  ~webserver_access();

  void sendRecipe(recipe const& r, std::function<void()> onFinished);
  void updateRecipe(recipe const& r, std::function<void()> onFinished);
  void sendRecipeImage(recipe const& r, std::filesystem::path const& database_path,
                       std::function<void()> onFinished);
  void removeRecipe(boost::uuids::uuid const& id, std::function<void()> onFinished);

  void retrieveRecipe(boost::uuids::uuid const& id,
                      std::function<void(std::optional<recipe> r)> callback);

  void sendPlan(plan const& p, std::function<void()> onFinished);
  void updatePlan(plan const& p, std::function<void()> onFinished);
  void subscribe(plan const& p, std::string const& subscriber, int mealIndex, int recipeIndex,
                 std::function<void()> onFinished);
  void unsubscribe(plan const& p, std::string const& subscriber, int mealIndex, int recipeIndex,
                   std::function<void()> onFinished);
  void removePlan(boost::uuids::uuid const& id, std::function<void()> onFinished);

  void retrievePlan(boost::uuids::uuid const& id,
                    std::function<void(std::optional<plan> p)> callback);

  void sendShopping(shopping_list const& s, std::function<void()> onFinished);
  void updateShopping(shopping_list const& s, std::function<void()> onFinished);
  void retrieveShopping(boost::uuids::uuid const& id,
                        std::function<void(std::optional<shopping_list> s)> callback);
  void removeShopping(boost::uuids::uuid const& id, std::function<void()> onFinished);

private:
  struct Impl;
  std::unique_ptr<Impl> _p;
  std::string const _address;
  std::string const _port;
};

} // namespace recipe::communication
