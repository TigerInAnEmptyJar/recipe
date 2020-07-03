my_add_library(recipe
  HEADER
    include/amounted_ingredient.h
    include/ingredient.h
    include/plan.h
    include/plan_item.h
    include/recipe.h
  SOURCE
    src/amounted_ingredient.cpp
    src/ingredient.cpp
    src/plan.cpp
    src/plan_item.cpp
    src/recipe.cpp
  INCLUDES
    PUBLIC include/
  ALIAS
    recipe::Recipe
)
