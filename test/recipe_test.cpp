#include "recipe.h"

#include <gtest/gtest.h>

TEST(recipe, ingredients)
{
  recipe::ingredient ing1{"Ingredient 1",
                          recipe::ingredient::category_t::diary,
                          recipe::ingredient::amount_type::mass,
                          true,
                          {}};
  recipe::ingredient ing2{"Ingredient 2",
                          recipe::ingredient::category_t::frozen,
                          recipe::ingredient::amount_type::volume,
                          false,
                          {}};

  recipe::recipe rec{"My recipe"};

  EXPECT_EQ(rec.begin(), rec.end());

  recipe::amounted_ingredient ai1{ing1};
  rec.add(ai1);
  EXPECT_EQ(std::distance(rec.begin(), rec.end()), 1);
  EXPECT_EQ(*rec.begin(), ai1);

  recipe::amounted_ingredient ai11{ing1};
  ai11.add(recipe::amounted_ingredient::amount_t::grams, 100);
  rec.add(ai11);
  EXPECT_EQ(std::distance(rec.begin(), rec.end()), 1);
  EXPECT_EQ(*rec.begin(), ai11);

  recipe::amounted_ingredient ai21{ing2};
  ai21.add(recipe::amounted_ingredient::amount_t::milliliter, 100);
  rec.add(ai21);
  EXPECT_EQ(std::distance(rec.begin(), rec.end()), 2);
  auto iter = rec.begin();
  EXPECT_EQ(*iter, ai11);
  ++iter;
  EXPECT_EQ(*iter, ai21);

  rec.remove(iter);
  EXPECT_EQ(std::distance(rec.begin(), rec.end()), 1);
  EXPECT_EQ(*rec.begin(), ai11);
}

TEST(recipe, tags)
{
  recipe::recipe rec{"My recipe"};

  EXPECT_EQ("", rec.tags());
  EXPECT_EQ(rec.tagList().size(), 0);

  rec.addTag("light");
  EXPECT_EQ("light", rec.tags());
  EXPECT_EQ(rec.tagList().size(), 1);

  rec.addTag("sushi");
  EXPECT_EQ("light, sushi", rec.tags());
  EXPECT_EQ(rec.tagList().size(), 2);

  rec.addTag("foo");
  EXPECT_EQ("light, sushi, foo", rec.tags());
  EXPECT_EQ(rec.tagList().size(), 3);

  rec.addTag("sushi");
  EXPECT_EQ("light, sushi, foo", rec.tags());
  EXPECT_EQ(rec.tagList().size(), 3);

  rec.addTag("foo");
  EXPECT_EQ("light, sushi, foo", rec.tags());
  EXPECT_EQ(rec.tagList().size(), 3);

  rec.removeTag("sushi");
  EXPECT_EQ("light, foo", rec.tags());
  EXPECT_EQ(rec.tagList().size(), 2);
}

TEST(recipe, eaters)
{
  recipe::recipe rec{"My recipe"};

  EXPECT_EQ("", rec.eaters());
  EXPECT_EQ(rec.eaterList().size(), 0);

  rec.addEater("John");
  EXPECT_EQ("John", rec.eaters());
  EXPECT_EQ(rec.eaterList().size(), 1);

  rec.addEater("Lisa");
  EXPECT_EQ("John, Lisa", rec.eaters());
  EXPECT_EQ(rec.eaterList().size(), 2);

  rec.addEater("Frida");
  EXPECT_EQ("John, Lisa, Frida", rec.eaters());
  EXPECT_EQ(rec.eaterList().size(), 3);

  rec.addEater("Frida");
  EXPECT_EQ("John, Lisa, Frida", rec.eaters());
  EXPECT_EQ(rec.eaterList().size(), 3);

  rec.addEater("Lisa");
  EXPECT_EQ("John, Lisa, Frida", rec.eaters());
  EXPECT_EQ(rec.eaterList().size(), 3);

  rec.removeEater("Lisa");
  EXPECT_EQ("John, Frida", rec.eaters());
  EXPECT_EQ(rec.eaterList().size(), 2);
}
