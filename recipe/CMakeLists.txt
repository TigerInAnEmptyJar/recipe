find_package(Boost REQUIRED)
my_add_library(recipe
  HEADER
    include/amounted_ingredient.h
    include/ingredient.h
    include/plan.h
    include/plan_item.h
    include/recipe.h
    include/shopping_day.h
    include/shopping_list.h
  SOURCE
    src/amounted_ingredient.cpp
    src/ingredient.cpp
    src/plan.cpp
    src/plan_item.cpp
    src/recipe.cpp
    src/shopping_day.cpp
    src/shopping_list.cpp
  INCLUDES
    PUBLIC include/
  ALIAS
    recipe::Recipe
  DEPENDS
    Boost::headers
)
