#include "shopping_list.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using day_iterator = recipe::shopping_day::iterator;

MATCHER_P3(IsIngredient1, id, am, val, "Ingredient is id %(id): %(val) %(am)")
{
  auto object = static_cast<day_iterator>(arg);
  recipe::amounted_ingredient& ing = std::get<0>(*object);
  EXPECT_EQ(std::get<0>(*object).base_ingredient().id(), id) << "Bad id of ingredient";
  if (std::distance(std::begin(ing), std::end(ing)) < 1) {
    return false;
  }
  EXPECT_EQ(std::distance(std::begin(ing), std::end(ing)), 1)
      << "Number of amounts in ingredient must be 1: actual:"
      << std::distance(std::begin(ing), std::end(ing));
  EXPECT_EQ(std::begin(ing)->first, am) << "Amount of ingredient is wrong";
  EXPECT_FLOAT_EQ(std::begin(ing)->second, val)
      << "Expected value of ingredient (" << ing.base_ingredient().name() << ") is " << val
      << ", actual: " << std::begin(ing)->second;
  return std::begin(ing)->first == am && fabs(std::begin(ing)->second - val) < 0.01;
};
MATCHER_P5(IsIngredient2, id, am1, val1, am2, val2, "Ingredient is id %(id): %(val) %(am)")
{
  auto object = static_cast<day_iterator>(arg);
  recipe::amounted_ingredient& ing = std::get<0>(*object);
  EXPECT_EQ(std::get<0>(*object).base_ingredient().id(), id) << "Bad id of ingredient";
  if (std::distance(std::begin(ing), std::end(ing)) < 2) {
    return false;
  }
  EXPECT_EQ(std::distance(std::begin(ing), std::end(ing)), 2)
      << "Number of amounts in ingredient must be 1: actual:"
      << std::distance(std::begin(ing), std::end(ing));
  auto a1 = std::begin(ing);
  auto a2 = std::begin(ing);
  std::advance(a2, 1);
  EXPECT_EQ(a1->first, am1) << "Amount1 of ingredient is wrong";
  EXPECT_FLOAT_EQ(a1->second, val1)
      << "Expected value1 of ingredient (" << ing.base_ingredient().name() << ") is " << val1
      << ", actual: " << a1->second;
  EXPECT_EQ(a2->first, am2) << "Amount2 of ingredient is wrong";
  EXPECT_FLOAT_EQ(a2->second, val2)
      << "Expected value2 of ingredient (" << ing.base_ingredient().name() << ") is " << val2
      << ", actual: " << a2->second;
  return a1->first == am1 && fabs(a1->second - val1) < 0.01 && a2->first == am2 &&
         fabs(a2->second - val2) < 0.01;
};

using category = recipe::ingredient::category_t;
using amount_type = recipe::ingredient::amount_type;
using amount = recipe::amounted_ingredient::amount_t;

class shoppingTest : public testing::Test
{
public:
  void SetUp()
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

  std::vector<recipe::ingredient> _ingredients;
  std::vector<recipe::recipe> _recipes;
};

TEST_F(shoppingTest, generation_singleItem)
{
  {
    recipe::plan single_item_plan{"single", 1, 1};
    auto item = single_item_plan.begin();
    item->name("Monday").shoppingBefore(true).add(_recipes[0]);

    {
      auto no_eater = recipe::shopping_list::generate(single_item_plan);

      ASSERT_EQ(std::distance(std::begin(no_eater), std::end(no_eater)), 0);
    }

    {
      single_item_plan.addEater("Mom");
      single_item_plan.addEater("Dad");
      std::begin(single_item_plan)->begin()->add("Mom");
      std::begin(single_item_plan)->begin()->add("Dad");
      auto two_eater = recipe::shopping_list::generate(single_item_plan);

      ASSERT_EQ(std::distance(std::begin(two_eater), std::end(two_eater)), 1);
      auto day = std::begin(two_eater);
      EXPECT_EQ(day->name(), "Monday");
      auto day_it = std::begin(*day);
      ASSERT_EQ(std::distance(day_it, std::end(*day)), 4);
      EXPECT_THAT(day_it, IsIngredient1(_ingredients[0].id(), amount::grams, 100 / 6.));
      EXPECT_THAT(day_it + 1, IsIngredient1(_ingredients[1].id(), amount::grams, 100 / 6.));
      EXPECT_THAT(day_it + 2, IsIngredient1(_ingredients[2].id(), amount::grams, 100 / 3.));
      EXPECT_THAT(day_it + 3, IsIngredient1(_ingredients[3].id(), amount::piece, 1 / 3.));
    }

    {
      std::begin(single_item_plan)->add(_recipes[1]);
      (std::begin(single_item_plan)->begin() + 1)->add("Mom");
      (std::begin(single_item_plan)->begin() + 1)->add("Dad");
      auto two_recipes = recipe::shopping_list::generate(single_item_plan);

      ASSERT_EQ(std::distance(std::begin(two_recipes), std::end(two_recipes)), 1);
      auto day = std::begin(two_recipes);
      EXPECT_EQ(day->name(), "Monday");
      auto day_it = std::begin(*day);
      ASSERT_EQ(std::distance(day_it, std::end(*day)), 6);
      EXPECT_THAT(day_it, IsIngredient1(_ingredients[0].id(), amount::grams, 100 / 6. + 20));
      EXPECT_THAT(day_it + 1, IsIngredient1(_ingredients[1].id(), amount::grams, 100 / 6.));
      EXPECT_THAT(day_it + 2, IsIngredient1(_ingredients[2].id(), amount::grams, 100 / 3.));
      EXPECT_THAT(day_it + 3, IsIngredient1(_ingredients[3].id(), amount::piece, 1 / 3.));
      EXPECT_THAT(day_it + 4, IsIngredient1(_ingredients[8].id(), amount::grams, 200.));
      EXPECT_THAT(day_it + 5, IsIngredient1(_ingredients[7].id(), amount::grams, 400.));
    }
  }
}

TEST_F(shoppingTest, generation_singleItem_nonSubscribable)
{
  {
    recipe::plan single_item_plan{"single", 1, 1};
    auto item = single_item_plan.begin();
    item->name("Monday").shoppingBefore(true).add(_recipes[1]);
    item->addFullRecipe(_recipes[0]);

    {
      auto no_eater = recipe::shopping_list::generate(single_item_plan);

      ASSERT_EQ(std::distance(std::begin(no_eater), std::end(no_eater)), 1);
      auto day = std::begin(no_eater);
      EXPECT_EQ(day->name(), "Monday");
      auto day_it = std::begin(*day);
      ASSERT_EQ(std::distance(day_it, std::end(*day)), 4);
      EXPECT_THAT(day_it, IsIngredient1(_ingredients[0].id(), amount::grams, 100));
      EXPECT_THAT(day_it + 1, IsIngredient1(_ingredients[1].id(), amount::grams, 100));
      EXPECT_THAT(day_it + 2, IsIngredient1(_ingredients[2].id(), amount::grams, 200));
      EXPECT_THAT(day_it + 3, IsIngredient1(_ingredients[3].id(), amount::piece, 2));
    }

    {
      single_item_plan.addEater("Mom");
      single_item_plan.addEater("Dad");
      std::begin(single_item_plan)->begin()->add("Mom");
      std::begin(single_item_plan)->begin()->add("Dad");
      auto two_eater = recipe::shopping_list::generate(single_item_plan);

      ASSERT_EQ(std::distance(std::begin(two_eater), std::end(two_eater)), 1);
      auto day = std::begin(two_eater);
      EXPECT_EQ(day->name(), "Monday");
      auto day_it = std::begin(*day);
      ASSERT_EQ(std::distance(day_it, std::end(*day)), 6);
      EXPECT_THAT(day_it + 0, IsIngredient1(_ingredients[8].id(), amount::grams, 200.));
      EXPECT_THAT(day_it + 1, IsIngredient1(_ingredients[7].id(), amount::grams, 400.));
      EXPECT_THAT(day_it + 2, IsIngredient1(_ingredients[0].id(), amount::grams, 100 + 20));
      EXPECT_THAT(day_it + 3, IsIngredient1(_ingredients[1].id(), amount::grams, 100));
      EXPECT_THAT(day_it + 4, IsIngredient1(_ingredients[2].id(), amount::grams, 200));
      EXPECT_THAT(day_it + 5, IsIngredient1(_ingredients[3].id(), amount::piece, 2));
    }

    {
      std::begin(single_item_plan)->add(_recipes[2]);
      (std::begin(single_item_plan)->begin() + 1)->add("Mom");
      (std::begin(single_item_plan)->begin() + 1)->add("Dad");
      auto two_recipes = recipe::shopping_list::generate(single_item_plan);

      ASSERT_EQ(std::distance(std::begin(two_recipes), std::end(two_recipes)), 1);
      auto day = std::begin(two_recipes);
      EXPECT_EQ(day->name(), "Monday");
      auto day_it = std::begin(*day);
      ASSERT_EQ(std::distance(day_it, std::end(*day)), 9);
      EXPECT_THAT(day_it + 0, IsIngredient1(_ingredients[8].id(), amount::grams, 200.));
      EXPECT_THAT(day_it + 1, IsIngredient1(_ingredients[7].id(), amount::grams, 400.));
      EXPECT_THAT(day_it + 2, IsIngredient1(_ingredients[0].id(), amount::grams, 100 + 20));
      EXPECT_THAT(day_it + 3, IsIngredient1(_ingredients[11].id(), amount::grams, 250));
      EXPECT_THAT(day_it + 4, IsIngredient1(_ingredients[10].id(), amount::grams, 150));
      EXPECT_THAT(day_it + 5, IsIngredient1(_ingredients[6].id(), amount::grams, 100));
      EXPECT_THAT(day_it + 6, IsIngredient1(_ingredients[1].id(), amount::grams, 100));
      EXPECT_THAT(day_it + 7, IsIngredient1(_ingredients[2].id(), amount::grams, 200));
      EXPECT_THAT(day_it + 8, IsIngredient1(_ingredients[3].id(), amount::piece, 2));
    }
  }
}

TEST_F(shoppingTest, generation_twoDays_oneShopping)
{
  recipe::plan two_item_plan{"single", 2, 1};
  auto first_day = two_item_plan.begin();
  first_day->name("Monday").shoppingBefore(true).add(_recipes[0]); // apple_pie
  auto second_day = two_item_plan.begin() + 1;
  second_day->name("Tuesday").shoppingBefore(false).add(_recipes[3]); // salad

  two_item_plan.addEater("Mom");
  two_item_plan.addEater("Dad");
  std::for_each(std::begin(two_item_plan), std::end(two_item_plan), [](auto& day) {
    std::for_each(std::begin(day), std::end(day), [](auto& meal) {
      meal.add("Mom");
      meal.add("Dad");
    });
  });

  {
    auto two_days = recipe::shopping_list::generate(two_item_plan);

    ASSERT_EQ(std::distance(std::begin(two_days), std::end(two_days)), 1);
    auto day = std::begin(two_days);
    EXPECT_EQ(day->name(), "Monday");
    auto day_it = std::begin(*day);
    ASSERT_EQ(std::distance(day_it, std::end(*day)), 7);

    EXPECT_THAT(day_it, IsIngredient1(_ingredients[0].id(), amount::grams, 100 / 6.));
    EXPECT_THAT(day_it + 1, IsIngredient1(_ingredients[1].id(), amount::grams, 100 / 6.));
    EXPECT_THAT(day_it + 2, IsIngredient1(_ingredients[2].id(), amount::grams, 100 / 3.));
    EXPECT_THAT(day_it + 3, IsIngredient1(_ingredients[3].id(), amount::piece, 1 / 3.));
    EXPECT_THAT(day_it + 4, IsIngredient1(_ingredients[6].id(), amount::piece, 1.));
    EXPECT_THAT(day_it + 5, IsIngredient1(_ingredients[4].id(), amount::piece, 1.));
    EXPECT_THAT(day_it + 6, IsIngredient1(_ingredients[7].id(), amount::piece, 2.));
  }

  {
    // It should not make any difference, if the very first item has shopping flag set or not.
    first_day->shoppingBefore(false);
    auto two_days = recipe::shopping_list::generate(two_item_plan);

    ASSERT_EQ(std::distance(std::begin(two_days), std::end(two_days)), 1);
    auto day = std::begin(two_days);
    EXPECT_EQ(day->name(), "Monday");
    auto day_it = std::begin(*day);
    ASSERT_EQ(std::distance(day_it, std::end(*day)), 7);

    EXPECT_THAT(day_it, IsIngredient1(_ingredients[0].id(), amount::grams, 100 / 6.));
    EXPECT_THAT(day_it + 1, IsIngredient1(_ingredients[1].id(), amount::grams, 100 / 6.));
    EXPECT_THAT(day_it + 2, IsIngredient1(_ingredients[2].id(), amount::grams, 100 / 3.));
    EXPECT_THAT(day_it + 3, IsIngredient1(_ingredients[3].id(), amount::piece, 1 / 3.));
    EXPECT_THAT(day_it + 4, IsIngredient1(_ingredients[6].id(), amount::piece, 1.));
    EXPECT_THAT(day_it + 5, IsIngredient1(_ingredients[4].id(), amount::piece, 1.));
    EXPECT_THAT(day_it + 6, IsIngredient1(_ingredients[7].id(), amount::piece, 2.));
  }
}

TEST_F(shoppingTest, generation_twoDays_twoShopping)
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
  for (auto& day : four_item_plan) {
    for (auto& meal : day) {
      meal.add("Mom");
      meal.add("Dad");
    }
  }

  auto two_days = recipe::shopping_list::generate(four_item_plan);

  ASSERT_EQ(std::distance(std::begin(two_days), std::end(two_days)), 2);
  auto day1 = std::begin(two_days);
  EXPECT_EQ(day1->name(), "Monday Lunch");
  auto day_it = std::begin(*day1);

  auto day2 = std::begin(two_days) + 1;
  EXPECT_EQ(day2->name(), "Tuesday Dinner");
  auto day2_it = std::begin(*day2);

  ASSERT_EQ(std::distance(day2_it, std::end(*day2)), 2);
  ASSERT_EQ(std::distance(day_it, std::end(*day1)), 9);

  EXPECT_THAT(day_it, IsIngredient1(_ingredients[0].id(), amount::grams, 100 / 6. + 20.));
  EXPECT_THAT(day_it + 1, IsIngredient1(_ingredients[1].id(), amount::grams, 100 / 6.));
  EXPECT_THAT(day_it + 2, IsIngredient1(_ingredients[2].id(), amount::grams, 100 / 3.));
  EXPECT_THAT(day_it + 3, IsIngredient1(_ingredients[3].id(), amount::piece, 1 / 3.));
  EXPECT_THAT(day_it + 4, IsIngredient1(_ingredients[8].id(), amount::grams, 200.));
  EXPECT_THAT(day_it + 5,
              IsIngredient2(_ingredients[7].id(), amount::grams, 400., amount::piece, 2.));
  EXPECT_THAT(day_it + 6, IsIngredient1(_ingredients[6].id(), amount::piece, 1.));
  EXPECT_THAT(day_it + 7, IsIngredient1(_ingredients[4].id(), amount::piece, 1.));
  EXPECT_THAT(day_it + 8, IsIngredient1(_ingredients[10].id(), amount::grams, 150.));

  EXPECT_THAT(day2_it, IsIngredient1(_ingredients[11].id(), amount::grams, 250.));
  EXPECT_THAT(day2_it + 1, IsIngredient1(_ingredients[6].id(), amount::grams, 100.));
}
