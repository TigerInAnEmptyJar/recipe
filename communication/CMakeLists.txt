find_package(RapidJSON REQUIRED)

my_add_library(communication
  HEADER
    include/web_serializer.h
    src/web_serializer_json.hpp
    include/webserver_access.hpp
  SOURCE
    src/web_serializer_json.cpp
    src/webserver_access.cpp
  DEPENDS
    PUBLIC recipe::Recipe
    PRIVATE Qt5::Core
    PRIVATE Qt5::Network
  INCLUDES
    PUBLIC include/
  ALIAS
    recipe::Communication
)
