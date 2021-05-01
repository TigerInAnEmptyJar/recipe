find_package(RapidJSON REQUIRED)

my_add_library(communication
  HEADER
    include/web_serializer.h
    src/web_serializer_json.hpp
  SOURCE
    src/web_serializer_json.cpp
  DEPENDS
    PUBLIC recipe::Recipe
    PRIVATE Qt5::Core
  INCLUDES
    PUBLIC include/
  ALIAS
    recipe::Communication
)
