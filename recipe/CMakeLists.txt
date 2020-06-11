my_add_library(recipe
  HEADER
    include/amounted_ingredient.h
    include/ingredient.h
    include/recipe.h
  SOURCE
    src/amounted_ingredient.cpp
    src/ingredient.cpp
    src/recipe.cpp
  INCLUDES
    PUBLIC include/
  ALIAS
    recipe::Recipe
)
