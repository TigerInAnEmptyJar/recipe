find_package(Qt5 REQUIRED Core)

my_add_library(inpput_output
  HEADER
    include/amounted_io.h
    include/ingredient_io.h
    include/recipe_io.h
    include/io_provider.h
    src/amounted_json_io.hpp
    src/ingredient_json_io.hpp
    src/recipe_json_io.hpp
  SOURCE
    src/io_provider.cpp
    src/amounted_json_io.cpp
    src/ingredient_json_io.cpp
    src/recipe_json_io.cpp
  DEPENDS
    PUBLIC recipe::Recipe
    PRIVATE Qt5::Core
  INCLUDES
    PUBLIC include/
  ALIAS
    recipe::IO
)
