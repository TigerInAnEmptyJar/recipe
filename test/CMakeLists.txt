my_add_test(recipe_test
  SOURCE
    amounted_ingredient_test.cpp
    recipe_test.cpp
    shopping_tests.cpp
  DEPENDS
    recipe::Recipe
)

my_add_test(io_test
  SOURCE
    io_tests.cpp
  DEPENDS
    recipe::IO
  INCLUDES
    PRIVATE ${CMAKE_SOURCE_DIR}/io/src/
)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/ingredients_version0.json ${CMAKE_CURRENT_BINARY_DIR}/data/ingredients_version0.json COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/shoppinglist_version0.json ${CMAKE_CURRENT_BINARY_DIR}/data/shoppinglist_version0.json COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/recipes_version0.json ${CMAKE_CURRENT_BINARY_DIR}/data/recipes_version0.json COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/plan_version0.json ${CMAKE_CURRENT_BINARY_DIR}/data/plan_version0.json COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/shopping_version0.json ${CMAKE_CURRENT_BINARY_DIR}/data/shopping_version0.json COPYONLY)
