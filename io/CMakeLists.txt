my_add_library(inpput_output
  HEADER
    include/amounted_io.h
    include/ingredient_io.h
    include/plan_io.h
    include/recipe_io.h
    include/io_provider.h
    src/amounted_json_io.hpp
    src/ingredient_json_io.hpp
    src/plan_json_io.hpp
    src/recipe_json_io.hpp
    src/recipe_tex_writer.hpp
    src/plan_tex_writer.hpp
    src/tex_writer.hpp
    include/recipe_tex_output.h
    src/shopping_list_tex_writer.hpp
  SOURCE
    src/io_provider.cpp
    src/amounted_json_io.cpp
    src/ingredient_json_io.cpp
    src/plan_json_io.cpp
    src/recipe_json_io.cpp
    src/recipe_tex_writer.cpp
    src/plan_tex_writer.cpp
    src/tex_writer.cpp
    src/shopping_json_io.cpp
    src/shopping_day_generator.cpp
    src/shopping_list_tex_writer.cpp
  DEPENDS
    PUBLIC recipe::Recipe
    PRIVATE Qt5::Core
  INCLUDES
    PUBLIC include/
  ALIAS
    recipe::IO
)
