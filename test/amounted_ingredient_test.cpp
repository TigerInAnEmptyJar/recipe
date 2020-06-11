#include "amounted_ingredient.h"
#include <gtest/gtest.h>

TEST(amounted_ingredient, create)
{
  recipe::ingredient base{"Ingredient1",
                          recipe::ingredient::category_t::fruit,
                          recipe::ingredient::amount_type::mass,
                          true,
                          {}};
  recipe::amounted_ingredient ingredient1{base};
  EXPECT_EQ(base.id(), ingredient1.base_ingredient().id());
  EXPECT_EQ(ingredient1.begin(), ingredient1.end());

  ingredient1.add(recipe::amounted_ingredient::amount_t::grams, 5);
  EXPECT_NE(ingredient1.begin(), ingredient1.end());
  EXPECT_EQ(ingredient1.begin()->first, recipe::amounted_ingredient::amount_t::grams);
  EXPECT_EQ(ingredient1.begin()->second, 5);

  ingredient1.remove(ingredient1.begin());
  EXPECT_EQ(ingredient1.begin(), ingredient1.end());
}

TEST(amounted_ingredient, add_mass)
{
  recipe::ingredient base{"Ingredient1",
                          recipe::ingredient::category_t::meat,
                          recipe::ingredient::amount_type::mass,
                          true,
                          {}};
  recipe::amounted_ingredient ingredient{base};

  // Add first amount in grams
  ingredient.add(recipe::amounted_ingredient::amount_t::grams, 5);
  EXPECT_NE(ingredient.begin(), ingredient.end());
  EXPECT_EQ(++ingredient.begin(), ingredient.end());
  EXPECT_EQ(ingredient.begin()->first, recipe::amounted_ingredient::amount_t::grams);
  EXPECT_EQ(ingredient.begin()->second, 5);

  // Add some more grams: adds to same
  ingredient.add(recipe::amounted_ingredient::amount_t::grams, 10);
  EXPECT_NE(ingredient.begin(), ingredient.end());
  EXPECT_EQ(++ingredient.begin(), ingredient.end());
  EXPECT_EQ(ingredient.begin()->first, recipe::amounted_ingredient::amount_t::grams);
  EXPECT_EQ(ingredient.begin()->second, 15);

  // Add some kgs: adds to same
  ingredient.add(recipe::amounted_ingredient::amount_t::kg, 0.02);
  EXPECT_NE(ingredient.begin(), ingredient.end());
  EXPECT_EQ(++ingredient.begin(), ingredient.end());
  EXPECT_EQ(ingredient.begin()->first, recipe::amounted_ingredient::amount_t::grams);
  EXPECT_EQ(ingredient.begin()->second, 35);

  // Add other type: add new item
  ingredient.add(recipe::amounted_ingredient::amount_t::bundle, 1);
  EXPECT_NE(ingredient.begin(), ingredient.end());
  EXPECT_NE(++ingredient.begin(), ingredient.end());
  EXPECT_EQ(ingredient.begin()->first, recipe::amounted_ingredient::amount_t::grams);
  EXPECT_EQ(ingredient.begin()->second, 35);
  EXPECT_EQ((++ingredient.begin())->first, recipe::amounted_ingredient::amount_t::bundle);
  EXPECT_EQ((++ingredient.begin())->second, 1);

  // Add volume: add new item
  ingredient.add(recipe::amounted_ingredient::amount_t::milliliter, 100);
  EXPECT_EQ(std::distance(ingredient.begin(), ingredient.end()), 3);
  auto iter = ingredient.begin();
  EXPECT_EQ(iter->first, recipe::amounted_ingredient::amount_t::milliliter);
  EXPECT_EQ(iter->second, 100);
  ++iter;
  EXPECT_EQ(iter->first, recipe::amounted_ingredient::amount_t::grams);
  EXPECT_EQ(iter->second, 35);
  ++iter;
  EXPECT_EQ(iter->first, recipe::amounted_ingredient::amount_t::bundle);
  EXPECT_EQ(iter->second, 1);
}

TEST(amounted_ingredient, add_volume)
{
  recipe::ingredient base{"Ingredient1",
                          recipe::ingredient::category_t::meat,
                          recipe::ingredient::amount_type::mass,
                          true,
                          {}};
  recipe::amounted_ingredient ingredient{base};

  // Initialize with a volume
  ingredient.add(recipe::amounted_ingredient::amount_t::milliliter, 500);
  EXPECT_NE(ingredient.begin(), ingredient.end());
  EXPECT_EQ(++ingredient.begin(), ingredient.end());
  EXPECT_EQ(ingredient.begin()->first, recipe::amounted_ingredient::amount_t::milliliter);
  EXPECT_EQ(ingredient.begin()->second, 500);

  // Add more volume: adds to same
  ingredient.add(recipe::amounted_ingredient::amount_t::milliliter, 10);
  EXPECT_NE(ingredient.begin(), ingredient.end());
  EXPECT_EQ(++ingredient.begin(), ingredient.end());
  EXPECT_EQ(ingredient.begin()->first, recipe::amounted_ingredient::amount_t::milliliter);
  EXPECT_EQ(ingredient.begin()->second, 510);

  // Add more volume: adds to same
  ingredient.add(recipe::amounted_ingredient::amount_t::liter, 0.02);
  EXPECT_NE(ingredient.begin(), ingredient.end());
  EXPECT_EQ(++ingredient.begin(), ingredient.end());
  EXPECT_EQ(ingredient.begin()->first, recipe::amounted_ingredient::amount_t::milliliter);
  EXPECT_EQ(ingredient.begin()->second, 530);

  // Add some other: add new item
  ingredient.add(recipe::amounted_ingredient::amount_t::bundle, 1);
  EXPECT_NE(ingredient.begin(), ingredient.end());
  EXPECT_NE(++ingredient.begin(), ingredient.end());
  EXPECT_EQ(ingredient.begin()->first, recipe::amounted_ingredient::amount_t::milliliter);
  EXPECT_EQ(ingredient.begin()->second, 530);
  EXPECT_EQ((++ingredient.begin())->first, recipe::amounted_ingredient::amount_t::bundle);
  EXPECT_EQ((++ingredient.begin())->second, 1);

  // Add some grams: add new item
  ingredient.add(recipe::amounted_ingredient::amount_t::grams, 100);
  EXPECT_EQ(std::distance(ingredient.begin(), ingredient.end()), 3);
  auto iter = ingredient.begin();
  EXPECT_EQ(iter->first, recipe::amounted_ingredient::amount_t::milliliter);
  EXPECT_EQ(iter->second, 530);
  ++iter;
  EXPECT_EQ(iter->first, recipe::amounted_ingredient::amount_t::grams);
  EXPECT_EQ(iter->second, 100);
  ++iter;
  EXPECT_EQ(iter->first, recipe::amounted_ingredient::amount_t::bundle);
  EXPECT_EQ(iter->second, 1);
}

TEST(amounted_ingredient, add_other)
{
  recipe::ingredient base{"Ingredient1",
                          recipe::ingredient::category_t::meat,
                          recipe::ingredient::amount_type::mass,
                          true,
                          {}};
  recipe::amounted_ingredient ingredient{base};

  // Initialize with other
  ingredient.add(recipe::amounted_ingredient::amount_t::bundle, 5);
  EXPECT_NE(ingredient.begin(), ingredient.end());
  EXPECT_EQ(++ingredient.begin(), ingredient.end());
  EXPECT_EQ(ingredient.begin()->first, recipe::amounted_ingredient::amount_t::bundle);
  EXPECT_EQ(ingredient.begin()->second, 5);

  // add same other type
  ingredient.add(recipe::amounted_ingredient::amount_t::bundle, 10);
  EXPECT_NE(ingredient.begin(), ingredient.end());
  EXPECT_EQ(++ingredient.begin(), ingredient.end());
  EXPECT_EQ(ingredient.begin()->first, recipe::amounted_ingredient::amount_t::bundle);
  EXPECT_EQ(ingredient.begin()->second, 15);

  // add other other type
  ingredient.add(recipe::amounted_ingredient::amount_t::can, 1);
  EXPECT_NE(ingredient.begin(), ingredient.end());
  EXPECT_NE(++ingredient.begin(), ingredient.end());
  EXPECT_EQ(ingredient.begin()->first, recipe::amounted_ingredient::amount_t::bundle);
  EXPECT_EQ(ingredient.begin()->second, 15);
  EXPECT_EQ((++ingredient.begin())->first, recipe::amounted_ingredient::amount_t::can);
  EXPECT_EQ((++ingredient.begin())->second, 1);

  // add again other other type
  ingredient.add(recipe::amounted_ingredient::amount_t::piece, 100);
  EXPECT_EQ(std::distance(ingredient.begin(), ingredient.end()), 3);
  auto iter = ingredient.begin();
  EXPECT_EQ(iter->first, recipe::amounted_ingredient::amount_t::piece);
  EXPECT_EQ(iter->second, 100);
  ++iter;
  EXPECT_EQ(iter->first, recipe::amounted_ingredient::amount_t::bundle);
  EXPECT_EQ(iter->second, 15);
  ++iter;
  EXPECT_EQ(iter->first, recipe::amounted_ingredient::amount_t::can);
  EXPECT_EQ(iter->second, 1);
}

TEST(amounted_ingredient, add_ingredient)
{
  recipe::ingredient base{"Ingredient1",
                          recipe::ingredient::category_t::meat,
                          recipe::ingredient::amount_type::mass,
                          true,
                          {}};
  recipe::amounted_ingredient ingredient{base};
  recipe::amounted_ingredient ingredient1{base};

  recipe::ingredient base2{"Ingredient1",
                           recipe::ingredient::category_t::meat,
                           recipe::ingredient::amount_type::mass,
                           true,
                           {}};
  recipe::amounted_ingredient ingredient2{base2};

  // initialize ingredient 1
  ingredient.add(recipe::amounted_ingredient::amount_t::bundle, 5);
  EXPECT_NE(ingredient.begin(), ingredient.end());
  EXPECT_EQ(++ingredient.begin(), ingredient.end());
  EXPECT_EQ(ingredient.begin()->first, recipe::amounted_ingredient::amount_t::bundle);
  EXPECT_EQ(ingredient.begin()->second, 5);

  // add empty ingredient
  ingredient.add(ingredient1);
  EXPECT_NE(ingredient.begin(), ingredient.end());
  EXPECT_EQ(++ingredient.begin(), ingredient.end());
  EXPECT_EQ(ingredient.begin()->first, recipe::amounted_ingredient::amount_t::bundle);
  EXPECT_EQ(ingredient.begin()->second, 5);

  // add ingredient with same type
  ingredient1.add(recipe::amounted_ingredient::amount_t::bundle, 10);
  ingredient.add(ingredient1);
  EXPECT_NE(ingredient.begin(), ingredient.end());
  EXPECT_EQ(++ingredient.begin(), ingredient.end());
  EXPECT_EQ(ingredient.begin()->first, recipe::amounted_ingredient::amount_t::bundle);
  EXPECT_EQ(ingredient.begin()->second, 15);

  ingredient1.remove(ingredient1.begin());

  // add ingredient with different type
  ingredient1.add(recipe::amounted_ingredient::amount_t::can, 1);
  ingredient.add(ingredient1);
  EXPECT_NE(ingredient.begin(), ingredient.end());
  EXPECT_EQ(std::distance(ingredient.begin(), ingredient.end()), 2);
  EXPECT_NE(++ingredient.begin(), ingredient.end());
  EXPECT_EQ(ingredient.begin()->first, recipe::amounted_ingredient::amount_t::bundle);
  EXPECT_EQ(ingredient.begin()->second, 15);
  EXPECT_EQ((++ingredient.begin())->first, recipe::amounted_ingredient::amount_t::can);
  EXPECT_EQ((++ingredient.begin())->second, 1);

  // add ingredient with other id
  ingredient2.add(recipe::amounted_ingredient::amount_t::bundle, 10);
  ingredient.add(ingredient2);
  EXPECT_EQ(std::distance(ingredient.begin(), ingredient.end()), 2);
  EXPECT_NE(ingredient.begin(), ingredient.end());
  EXPECT_NE(++ingredient.begin(), ingredient.end());
  EXPECT_EQ(ingredient.begin()->first, recipe::amounted_ingredient::amount_t::bundle);
  EXPECT_EQ(ingredient.begin()->second, 15);
  EXPECT_EQ((++ingredient.begin())->first, recipe::amounted_ingredient::amount_t::can);
  EXPECT_EQ((++ingredient.begin())->second, 1);
}
