find_package(Doxygen
             REQUIRED dot
             OPTIONAL_COMPONENTS mscgen dia)

set(directories_to_document
  ${CMAKE_CURRENT_SOURCE_DIR}/recipe_general.dox
  ${CMAKE_CURRENT_SOURCE_DIR}/recipe_desktop_ui.dox
  ${CMAKE_SOURCE_DIR}/recipe/
  ${CMAKE_SOURCE_DIR}/io/
)

doxygen_add_docs(documentation
               ${directories_to_document}
               documentation
)
