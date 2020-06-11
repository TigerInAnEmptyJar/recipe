my_add_test(recipe_test
  SOURCE
    amounted_ingredient_test.cpp
    recipe_test.cpp
  DEPENDS
    recipe::Recipe
)
