#include "amounted_json_io.hpp"
#include "ingredient_json_io.hpp"
#include "io_provider.h"
#include "plan_json_io.hpp"
#include "recipe_json_io.hpp"
#include "shopping_json_io.h"

#include <gtest/gtest.h>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

class amounted_test : public recipe::io::amounted_io
{
  void write(std::vector<recipe::amounted_ingredient> const&,
             std::filesystem::path const&) const override
  {
  }
  std::optional<std::vector<recipe::amounted_ingredient>> read(
      std::filesystem::path const&,
      std::function<std::optional<recipe::ingredient>(boost::uuids::uuid)>) const override
  {
    return {};
  }
};

class ingredient_test : public recipe::io::ingredient_io
{
  void write(std::vector<recipe::ingredient> const&, std::filesystem::path const&) const override {}
  std::optional<std::vector<recipe::ingredient>> read(std::filesystem::path const&) const override
  {
    return {};
  }
};

class recipe_test : public recipe::io::recipe_io
{
  void write(std::vector<recipe::recipe> const&, std::filesystem::path const&) const override {}
  std::optional<std::vector<recipe::recipe>> read(
      std::filesystem::path const&,
      std::function<std::optional<recipe::ingredient>(boost::uuids::uuid)>) const override
  {
    return {};
  }
};

class plan_test : public recipe::io::plan_io
{
  void write(recipe::plan const&, std::filesystem::path const&) const override {}
  std::optional<recipe::plan> read(
      std::filesystem::path const&,
      std::function<std::optional<recipe::recipe>(boost::uuids::uuid)>) const override
  {
    return {};
  }
};

class shopping_test : public recipe::io::shopping_io
{
  void write(recipe::shopping_list const&, std::filesystem::path const&) const override {}
  std::optional<recipe::shopping_list> read(std::filesystem::path const&,
                                            recipe::io::shopping_io::finder_t) const override
  {
    return {};
  }

  std::string serialize(recipe::shopping_list const&) const override { return {}; };

  virtual std::optional<recipe::shopping_list> deserialize(
      std::string const&, recipe::io::shopping_io::finder_t) const override
  {
    return {};
  }
};

class io_test : public testing::Test
{
public:
  void SetUp() override
  {
    _ingredients.push_back(recipe::ingredient{"Milk",
                                              recipe::ingredient::category_t::diary,
                                              recipe::ingredient::amount_type::mass,
                                              true,
                                              {"/tmp/milk.jpg"}});
    _ingredients.push_back(recipe::ingredient{"Ham",
                                              recipe::ingredient::category_t::meat,
                                              recipe::ingredient::amount_type::mass,
                                              true,
                                              {"/tmp/ham.jpg"}});
    _ingredients.push_back(recipe::ingredient{"Apple",
                                              recipe::ingredient::category_t::fruit,
                                              recipe::ingredient::amount_type::piece,
                                              false,
                                              {"/tmp/apple.jpg"}});

    _ingredients[0].id(boost::uuids::random_generator{}());
    _ingredients[1].id(boost::uuids::random_generator{}());
    _ingredients[2].id(boost::uuids::random_generator{}());

    setupAmounted();
    setupRecipe();
    setupPlan();

    _finder_good = [this](boost::uuids::uuid const& id) -> std::optional<recipe::ingredient> {
      auto item = std::find_if(std::begin(_ingredients), std::end(_ingredients),
                               [id](auto element) { return element.id() == id; });
      if (item != std::end(_ingredients)) {
        return *item;
      }
      return {};
    };
    _recipe_finder_good = [this](boost::uuids::uuid const& id) -> std::optional<recipe::recipe> {
      auto item = std::find_if(std::begin(_recipes), std::end(_recipes),
                               [id](auto element) { return element.id() == id; });
      if (item != std::end(_recipes)) {
        return *item;
      }
      return {};
    };
  }

  void setupAmounted()
  {
    _amounted.clear();
    _amounted.push_back(recipe::amounted_ingredient{_ingredients[0]});
    _amounted.push_back(recipe::amounted_ingredient{_ingredients[0]});
    _amounted.push_back(recipe::amounted_ingredient{_ingredients[1]});
    _amounted.push_back(recipe::amounted_ingredient{_ingredients[2]});
    _amounted.push_back(recipe::amounted_ingredient{_ingredients[1]});
    _amounted.push_back(recipe::amounted_ingredient{_ingredients[2]});
    _amounted.push_back(recipe::amounted_ingredient{_ingredients[0]});
    _amounted[0].add(recipe::amounted_ingredient::amount_t::liter, 1);
    _amounted[0].add(recipe::amounted_ingredient::amount_t::grams, 100);
    _amounted[2].add(recipe::amounted_ingredient::amount_t::grams, 200);
    _amounted[2].add(recipe::amounted_ingredient::amount_t::piece, 1);
    _amounted[3].add(recipe::amounted_ingredient::amount_t::piece, 10);
    _amounted[6].add(recipe::amounted_ingredient::amount_t::bundle, 1);
    _amounted[6].add(recipe::amounted_ingredient::amount_t::can, 1);
    _amounted[6].add(recipe::amounted_ingredient::amount_t::liter, 1);
    _amounted[6].add(recipe::amounted_ingredient::amount_t::milliliter, 100);
  }

  void setupRecipe()
  {
    auto recipe1 = recipe::recipe{"Recipe 1"}
                       .g_fat(10)
                       .g_proteins(11)
                       .g_carbohydrates(12)
                       .calories(13)
                       .joules(14)
                       .source("My Cook book")
                       .servings(2)
                       .meal_type(recipe::meal_t::vegetarian)
                       .preparation_time(std::chrono::minutes{15})
                       .image_path("/tmp/recipe1.jpeg")
                       .addTag("tag1")
                       .addTag("tag2")
                       .addEater("John")
                       .addEater("Jane")
                       .instructions("my instructions 1");
    recipe1.add(_amounted[0]);
    recipe1.add(_amounted[2]);
    recipe1.add(_amounted[3]);
    auto recipe2 = recipe::recipe{"Recipe 2"}
                       .g_fat(12)
                       .g_proteins(13)
                       .g_carbohydrates(14)
                       .calories(15)
                       .joules(16)
                       .source("My Cook book")
                       .servings(4)
                       .meal_type(recipe::meal_t::fish)
                       .preparation_time(std::chrono::minutes{45})
                       .image_path("/tmp/recipe2.jpeg")
                       .addTag("tag2")
                       .addTag("tag4")
                       .addEater("Kate")
                       .addEater("William")
                       .instructions("my instructions 2");
    recipe2.add(_amounted[1]);
    recipe2.add(_amounted[4]);
    recipe2.add(_amounted[5]);
    _recipes.clear();
    _recipes.push_back(recipe1);
    _recipes.push_back(recipe2);
  }

  void setupPlan()
  {
    _plan = recipe::plan{"plan 1", 2, 2};
    auto it = _plan.begin();
    it->add(_recipes[0]);
    it->add(_recipes[1]);
    it->name("item 1");
    it->shoppingBefore(true);
    (++it)->add(_recipes[0]);
    it->name("item 2");
    it->shoppingBefore(false);
    (++it)->add(_recipes[1]);
    it->add(_recipes[1]);
    it->name("item 3");
    it->shoppingBefore(true);
    (++it)->add(_recipes[1]);
    it->name("item 4");
    it->shoppingBefore(false);
    _plan.addEater("Alice");
    _plan.addEater("Bob");
  }

  std::vector<recipe::ingredient> _ingredients;
  std::vector<recipe::amounted_ingredient> _amounted;
  std::vector<recipe::recipe> _recipes;
  recipe::plan _plan{"new plan", 1, 2};
  std::function<std::optional<recipe::ingredient>(boost::uuids::uuid const& id)> _finder_good;
  std::function<std::optional<recipe::recipe>(boost::uuids::uuid const& id)> _recipe_finder_good;
};

using category = recipe::ingredient::category_t;
using amount_type = recipe::ingredient::amount_type;
using amount = recipe::amounted_ingredient::amount_t;

class shopping_io_test : public testing::Test
{
public:
  void SetUpStuff(std::vector<boost::uuids::uuid> ingredient_ids = {})
  {
    auto ing = [](recipe::ingredient i, float v, amount a) {
      recipe::amounted_ingredient ret{i};
      ret.add(a, v);
      return ret;
    };

    _ingredients.push_back({"Butter", category::diary, amount_type::mass, true, {}});        // 0
    _ingredients.push_back({"Sugar", category::baking, amount_type::mass, false, {}});       // 1
    _ingredients.push_back({"Flour", category::baking, amount_type::mass, false, {}});       // 2
    _ingredients.push_back({"Apple", category::fruit, amount_type::mass, true, {}});         // 3
    _ingredients.push_back({"Pear", category::fruit, amount_type::mass, true, {}});          // 4
    _ingredients.push_back({"Orange", category::fruit, amount_type::mass, true, {}});        // 5
    _ingredients.push_back({"Salad", category::vegetable, amount_type::mass, true, {}});     // 6
    _ingredients.push_back({"Carrot", category::vegetable, amount_type::mass, false, {}});   // 7
    _ingredients.push_back({"Steak", category::meat, amount_type::mass, true, {}});          // 8
    _ingredients.push_back({"Bacon", category::meat, amount_type::mass, false, {}});         // 9
    _ingredients.push_back({"Frozen peas", category::frozen, amount_type::mass, false, {}}); // 10
    _ingredients.push_back({"Salmon", category::fish, amount_type::mass, true, {}});         // 11
    auto ingredient_it = std::begin(_ingredients);
    std::for_each(std::begin(ingredient_ids), std::end(ingredient_ids),
                  [this, &ingredient_it](auto id) {
                    if (ingredient_it != std::end(_ingredients)) {
                      ingredient_it->id(id);
                      ingredient_it++;
                    }
                  });

    recipe::recipe apple_tarte("apple tarte"); // 0
    apple_tarte.servings(12)
        .add(ing(_ingredients[0], 100, amount::grams))
        .add(ing(_ingredients[1], 100, amount::grams))
        .add(ing(_ingredients[2], 200, amount::grams))
        .add(ing(_ingredients[3], 2, amount::piece));
    _recipes.push_back(apple_tarte);
    recipe::recipe steak("steak"); // 1
    steak.servings(2)
        .add(ing(_ingredients[8], 200, amount::grams))
        .add(ing(_ingredients[7], 400, amount::grams))
        .add(ing(_ingredients[0], 20, amount::grams));
    _recipes.push_back(steak);
    recipe::recipe salmon{"salmon"}; // 2
    salmon.servings(4)
        .add(ing(_ingredients[11], 500, amount::grams))
        .add(ing(_ingredients[10], 300, amount::grams))
        .add(ing(_ingredients[6], 200, amount::grams));
    _recipes.push_back(salmon);
    recipe::recipe salad{"salad"}; // 3
    salad.servings(2)
        .add(ing(_ingredients[6], 1, amount::piece))
        .add(ing(_ingredients[4], 1, amount::piece))
        .add(ing(_ingredients[7], 2, amount::piece));
    _recipes.push_back(salad);
    recipe::recipe fruit_salad{"fruit salad"}; // 4
    salad.servings(2)
        .add(ing(_ingredients[3], 1, amount::piece))
        .add(ing(_ingredients[4], 1, amount::piece))
        .add(ing(_ingredients[5], 2, amount::piece))
        .add(ing(_ingredients[1], 50, amount::grams));
    _recipes.push_back(fruit_salad);
  }

  recipe::plan prepare_plan()
  {
    recipe::plan four_item_plan{"single", 2, 2};
    auto first_day1 = four_item_plan.begin();
    first_day1->name("Monday Lunch").shoppingBefore(true).add(_recipes[0]); // apple_pie
    auto first_day2 = four_item_plan.begin() + 1;
    first_day2->name("Monday Evening").shoppingBefore(false).add(_recipes[1]); // steak
    auto second_day1 = four_item_plan.begin() + 2;
    second_day1->name("Tuesday Lunch").shoppingBefore(false).add(_recipes[3]); // salad
    auto second_day2 = four_item_plan.begin() + 3;
    second_day2->name("Tuesday Dinner").shoppingBefore(true).add(_recipes[2]); // salmon

    // Cook for Mom and Dad.
    four_item_plan.addEater("Mom");
    four_item_plan.addEater("Dad");
    for (auto& meal : four_item_plan) {
      meal.add("Mom");
      meal.add("Dad");
    }

    return four_item_plan;
  }
  std::vector<recipe::ingredient> _ingredients;
  std::vector<recipe::recipe> _recipes;
};

TEST_F(io_test, provider)
{
  recipe::io::io_provider provider;

  EXPECT_TRUE(provider.installed_amounted().empty());
  EXPECT_TRUE(provider.installed_ingredient().empty());
  EXPECT_TRUE(provider.installed_recipe().empty());
  EXPECT_TRUE(provider.installed_plan().empty());
  EXPECT_TRUE(provider.installed_shopping().empty());

  auto a_object = std::make_shared<amounted_test>();
  auto a_id = boost::uuids::random_generator_mt19937{}();
  std::cout << boost::uuids::to_string(a_id) << std::endl;
  provider.install(a_id, "test", a_object);

  EXPECT_FALSE(provider.installed_amounted().empty());
  EXPECT_TRUE(provider.installed_ingredient().empty());
  EXPECT_TRUE(provider.installed_recipe().empty());
  EXPECT_TRUE(provider.installed_plan().empty());
  EXPECT_TRUE(provider.installed_shopping().empty());

  EXPECT_EQ(a_object, provider.amounted(a_id));

  auto i_object = std::make_shared<ingredient_test>();
  auto i_id = boost::uuids::random_generator_mt19937{}();
  provider.install(i_id, "test", i_object);

  EXPECT_FALSE(provider.installed_amounted().empty());
  EXPECT_FALSE(provider.installed_ingredient().empty());
  EXPECT_TRUE(provider.installed_recipe().empty());
  EXPECT_TRUE(provider.installed_plan().empty());
  EXPECT_TRUE(provider.installed_shopping().empty());

  EXPECT_EQ(i_object, provider.ingredient(i_id));

  auto r_object = std::make_shared<recipe_test>();
  auto r_id = boost::uuids::random_generator_mt19937{}();
  provider.install(r_id, "test", r_object);

  EXPECT_FALSE(provider.installed_amounted().empty());
  EXPECT_FALSE(provider.installed_ingredient().empty());
  EXPECT_FALSE(provider.installed_recipe().empty());
  EXPECT_TRUE(provider.installed_plan().empty());
  EXPECT_TRUE(provider.installed_shopping().empty());

  EXPECT_EQ(r_object, provider.recipe(r_id));

  auto p_object = std::make_shared<plan_test>();
  auto p_id = boost::uuids::random_generator_mt19937{}();
  provider.install(p_id, "test", p_object);

  EXPECT_FALSE(provider.installed_amounted().empty());
  EXPECT_FALSE(provider.installed_ingredient().empty());
  EXPECT_FALSE(provider.installed_recipe().empty());
  EXPECT_FALSE(provider.installed_plan().empty());
  EXPECT_TRUE(provider.installed_shopping().empty());

  EXPECT_EQ(p_object, provider.plan(p_id));

  auto s_object = std::make_shared<shopping_test>();
  auto s_id = boost::uuids::random_generator_mt19937{}();
  provider.install(s_id, "test", s_object);

  EXPECT_FALSE(provider.installed_amounted().empty());
  EXPECT_FALSE(provider.installed_ingredient().empty());
  EXPECT_FALSE(provider.installed_recipe().empty());
  EXPECT_FALSE(provider.installed_plan().empty());
  EXPECT_FALSE(provider.installed_shopping().empty());

  EXPECT_EQ(s_object, provider.shopping(s_id));

  EXPECT_EQ(nullptr, provider.amounted(i_id));
  EXPECT_EQ(nullptr, provider.amounted(r_id));
  EXPECT_EQ(nullptr, provider.amounted(p_id));
  EXPECT_EQ(nullptr, provider.amounted(s_id));
  EXPECT_EQ(nullptr, provider.ingredient(a_id));
  EXPECT_EQ(nullptr, provider.ingredient(r_id));
  EXPECT_EQ(nullptr, provider.ingredient(p_id));
  EXPECT_EQ(nullptr, provider.ingredient(s_id));
  EXPECT_EQ(nullptr, provider.recipe(a_id));
  EXPECT_EQ(nullptr, provider.recipe(i_id));
  EXPECT_EQ(nullptr, provider.recipe(p_id));
  EXPECT_EQ(nullptr, provider.recipe(s_id));
  EXPECT_EQ(nullptr, provider.plan(a_id));
  EXPECT_EQ(nullptr, provider.plan(i_id));
  EXPECT_EQ(nullptr, provider.plan(r_id));
  EXPECT_EQ(nullptr, provider.plan(s_id));
  EXPECT_EQ(nullptr, provider.shopping(a_id));
  EXPECT_EQ(nullptr, provider.shopping(i_id));
  EXPECT_EQ(nullptr, provider.shopping(r_id));
  EXPECT_EQ(nullptr, provider.shopping(r_id));

  provider.uninstall_amounted(a_id);
  EXPECT_TRUE(provider.installed_amounted().empty());
  EXPECT_FALSE(provider.installed_ingredient().empty());
  EXPECT_FALSE(provider.installed_recipe().empty());
  EXPECT_FALSE(provider.installed_plan().empty());
  EXPECT_FALSE(provider.installed_shopping().empty());

  provider.uninstall_ingredient(i_id);
  EXPECT_TRUE(provider.installed_amounted().empty());
  EXPECT_TRUE(provider.installed_ingredient().empty());
  EXPECT_FALSE(provider.installed_recipe().empty());
  EXPECT_FALSE(provider.installed_plan().empty());
  EXPECT_FALSE(provider.installed_shopping().empty());

  provider.uninstall_recipe(r_id);
  EXPECT_TRUE(provider.installed_amounted().empty());
  EXPECT_TRUE(provider.installed_ingredient().empty());
  EXPECT_TRUE(provider.installed_recipe().empty());
  EXPECT_FALSE(provider.installed_plan().empty());
  EXPECT_FALSE(provider.installed_shopping().empty());

  provider.uninstall_plan(p_id);
  EXPECT_TRUE(provider.installed_amounted().empty());
  EXPECT_TRUE(provider.installed_ingredient().empty());
  EXPECT_TRUE(provider.installed_recipe().empty());
  EXPECT_TRUE(provider.installed_plan().empty());
  EXPECT_FALSE(provider.installed_shopping().empty());

  provider.uninstall_shopping(s_id);
  EXPECT_TRUE(provider.installed_amounted().empty());
  EXPECT_TRUE(provider.installed_ingredient().empty());
  EXPECT_TRUE(provider.installed_recipe().empty());
  EXPECT_TRUE(provider.installed_plan().empty());
  EXPECT_TRUE(provider.installed_shopping().empty());
}

TEST_F(io_test, ingredient_rw)
{
  recipe::io::ingredient_json_io io;
  std::filesystem::path file{"/tmp/ingredients.json"};

  io.write(_ingredients, file);
  EXPECT_TRUE(std::filesystem::exists(file));
  auto read = io.read(file);

  ASSERT_TRUE(read.has_value());
  ASSERT_EQ(read->size(), _ingredients.size());
  EXPECT_EQ(*read, _ingredients);
}

TEST_F(io_test, ingredient_version)
{
  recipe::io::ingredient_json_io io;
  std::filesystem::path file{"data/ingredients_version0.json"};

  EXPECT_TRUE(std::filesystem::exists(file));
  auto read = io.read(file);

  // for the member ingredients, the ids are generated randomly with every execution, but the ids
  // in the file stay the same: set ids to allow comparison.
  boost::uuids::string_generator gen;
  _ingredients[0].id(gen("7a806cd3-3ec2-42e9-8786-8859d5211785"));
  _ingredients[1].id(gen("a5356323-4586-4dba-a3da-445e9f59668c"));
  _ingredients[2].id(gen("4b6e5242-7b2c-4876-9f7b-b75783646eb1"));

  ASSERT_TRUE(read.has_value());
  ASSERT_EQ(read->size(), _ingredients.size());
  EXPECT_EQ(*read, _ingredients);
}

TEST_F(io_test, amounted_rw)
{
  recipe::io::amounted_json_io io;
  std::filesystem::path file{"/tmp/shoppinglist.json"};

  io.write(_amounted, file);
  EXPECT_TRUE(std::filesystem::exists(file));
  auto read = io.read(file, _finder_good);

  ASSERT_TRUE(read.has_value());
  ASSERT_EQ(read->size(), _amounted.size());
  EXPECT_EQ(*read, _amounted);
}

TEST_F(io_test, amounted_version)
{
  recipe::io::amounted_json_io io;
  std::filesystem::path file{"data/shoppinglist_version0.json"};

  // for the member ingredients, the ids are generated randomly with every execution, but the ids
  // in the file stay the same: set ids to allow comparison.
  boost::uuids::string_generator gen;
  _ingredients[0].id(gen("0caca5b7-82a8-49aa-bfad-80c94c2eddad"));
  _ingredients[1].id(gen("7be59fd4-abe1-47ad-a42c-e656a29be949"));
  _ingredients[2].id(gen("c73931f0-641a-4fac-9eab-9cf7296e7832"));
  setupAmounted();

  EXPECT_TRUE(std::filesystem::exists(file));
  auto read = io.read(file, _finder_good);

  ASSERT_TRUE(read.has_value());
  ASSERT_EQ(read->size(), _amounted.size());
  EXPECT_EQ(*read, _amounted);
}

TEST_F(io_test, recipe_rw)
{
  setupPlan();
  recipe::io::recipe_json_io io;
  std::filesystem::path file{"/tmp/recipes.json"};

  io.write(_recipes, file);
  EXPECT_TRUE(std::filesystem::exists(file));
  auto read = io.read(file, _finder_good);

  ASSERT_TRUE(read.has_value());
  ASSERT_EQ(read->size(), _recipes.size());
  EXPECT_EQ(*read, _recipes);
}

TEST_F(io_test, recipe_version)
{
  recipe::io::recipe_json_io io;
  std::filesystem::path file{"data/recipes_version0.json"};

  // for the member ingredients, the ids are generated randomly with every execution, but the ids
  // in the file stay the same: set ids to allow comparison.
  boost::uuids::string_generator gen;
  _ingredients[0].id(gen("7370d976-2595-4b27-bf62-d21b2324a792"));
  _ingredients[1].id(gen("bf5987c6-5f29-4d41-bb63-31d880821baa"));
  _ingredients[2].id(gen("7b613cc3-9727-46c2-9075-2d4ed55562ea"));
  setupAmounted();
  setupRecipe();
  _recipes[0].id(gen("00000001-9845-19fe-7f00-00e8984519fe"));
  _recipes[1].id(gen("00000001-d904-4a0d-5600-00f0954519fe"));

  EXPECT_TRUE(std::filesystem::exists(file));
  auto read = io.read(file, _finder_good);

  ASSERT_TRUE(read.has_value());
  ASSERT_EQ(read->size(), _recipes.size());
  EXPECT_EQ(*read, _recipes);
}

TEST_F(io_test, plan_rw)
{
  recipe::io::plan_json_io io;
  std::filesystem::path file{"/tmp/plan.json"};

  io.write(_plan, file);
  EXPECT_TRUE(std::filesystem::exists(file));
  auto read = io.read(file, _recipe_finder_good);

  ASSERT_TRUE(read.has_value());
  EXPECT_EQ(*read, _plan);
}

TEST_F(io_test, plan_version)
{
  recipe::io::plan_json_io io;
  std::filesystem::path file{"data/plan_version0.json"};

  // for the member recipes, the ids are generated randomly with every execution, but the ids
  // in the file stay the same: set ids to allow comparison.
  boost::uuids::string_generator gen;
  setupAmounted();
  setupRecipe();
  _recipes[0].id(gen("0001ac0a-a7ff-7f00-0038-ac0aa7ff7f00"));
  _recipes[1].id(gen("0001e671-da60-5500-0040-a90aa7ff7f00"));
  setupPlan();
  _plan.removeEater("Alice");
  _plan.removeEater("Bob");

  EXPECT_TRUE(std::filesystem::exists(file));
  auto read = io.read(file, _recipe_finder_good);

  ASSERT_TRUE(read.has_value());
  EXPECT_EQ(*read, _plan);
}

TEST_F(shopping_io_test, shopping_rw)
{
  recipe::io::shopping_json_io io;
  std::filesystem::path file{"/tmp/shopping.json"};

  SetUpStuff();
  auto plan = prepare_plan();
  auto shopping = recipe::shopping_list::generate(plan);
  auto finder = [this](auto id) -> std::optional<recipe::ingredient> {
    auto item = std::find_if(std::begin(_ingredients), std::end(_ingredients),
                             [id](auto element) { return element.id() == id; });
    if (item != std::end(_ingredients)) {
      return *item;
    }
    return {};
  };

  std::get<3>(*shopping.begin()->begin()) = true;
  std::get<2>(*(shopping.begin()->begin() + 1)) = 10.;
  std::get<1>(*(shopping.begin()->begin() + 1)) = recipe::amounted_ingredient::amount_t::piece;

  io.write(shopping, file);
  EXPECT_TRUE(std::filesystem::exists(file));
  auto read = io.read(file, finder);

  ASSERT_TRUE(read.has_value());
  EXPECT_EQ(*read, shopping);
}

TEST_F(shopping_io_test, shopping_version)
{
  recipe::io::shopping_json_io io;
  std::filesystem::path file{"data/shopping_version0.json"};

  // for the member recipes, the ids are generated randomly with every execution, but the ids
  // in the file stay the same: set ids to allow comparison.
  boost::uuids::string_generator gen;

  std::vector<boost::uuids::uuid> ids{
      gen("b3d48c39-af6a-4838-bc6c-d104479e9655"), // 0
      gen("83a57c8b-a706-424b-940b-7fcb244fa987"), // 1
      gen("973b9397-045e-4a34-bb21-1141cc8a198f"), // 2
      gen("f1709ed4-5906-469c-a51a-d00dc53fe788"), // 3
      gen("5a9f6a4f-1e73-400d-ab2f-93feb3ea7cb1"), // 4
      gen("6dcc08d0-102b-451d-84c9-a607a0803fbc"), // not existing
      gen("ed8757e4-246c-420b-9c49-0a378772366d"), // 6
      gen("7a34d83e-fa15-49cc-bf27-7a3a8b334a4a"), // 7
      gen("41142173-bffc-4f7a-a146-8e4d7eaca24f"), // 8
      gen("6dcc08d0-102b-451d-84c9-a607a0803fbc"), // not existing
      gen("dcd1a5f9-9fca-4cd3-9759-f52dfb328b5b"), // 10
      gen("218066b0-295c-4021-b20d-0938c852e8e2"), // 11
      gen("6dcc08d0-102b-451d-84c9-a607a0803fbc")  // not existing
  };
  SetUpStuff(ids);
  auto plan = prepare_plan();
  plan.id(gen("902b8cd8-3f56-0000-602a-8cd83f560000"));
  auto shopping = recipe::shopping_list::generate(plan);
  auto finder = [this](auto id) -> std::optional<recipe::ingredient> {
    auto item = std::find_if(std::begin(_ingredients), std::end(_ingredients),
                             [id](auto element) { return element.id() == id; });
    if (item != std::end(_ingredients)) {
      return *item;
    }
    return {};
  };

  EXPECT_TRUE(std::filesystem::exists(file));
  auto read = io.read(file, finder);

  ASSERT_TRUE(read.has_value());
  std::cout << boost::uuids::to_string(shopping.id()) << std::endl;
  EXPECT_EQ(read->id(), shopping.id());
  EXPECT_EQ(*read, shopping);
}

TEST_F(io_test, provider_setup)
{
  recipe::io::io_provider provider;

  provider.setup();
  EXPECT_EQ(provider.installed_amounted().size(), 1);
  EXPECT_EQ(provider.installed_ingredient().size(), 1);
  // One for Json export, one for LaTeX export
  EXPECT_EQ(provider.installed_recipe().size(), 2);
  // One for Json export, three for LaTeX export
  EXPECT_EQ(provider.installed_plan().size(), 4);
  // One for Json export, one for LaTeX export
  EXPECT_EQ(provider.installed_shopping().size(), 2);
}
