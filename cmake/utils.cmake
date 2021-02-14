function(my_add_library _NAME)
  cmake_parse_arguments(A "INTERFACE;STATIC;AUTORCC;AUTOMOC;AUTOUIC" "ALIAS" "HEADER;SOURCE;DEPENDS;INCLUDES;DEFINES" ${ARGN})
  message(STATUS "Adding library ${_NAME} (alias: ${A_ALIAS})")
  if(${A_INTERFACE})
    add_library(${_NAME} INTERFACE ${A_HEADER})
  else()
    if(${A_STATIC})
      add_library(${_NAME} STATIC ${A_HEADER} ${A_SOURCE})
    else()
      add_library(${_NAME} ${A_HEADER} ${A_SOURCE})
    endif()
  endif()

  if (A_DEPENDS)
    target_link_libraries(${_NAME} ${A_DEPENDS})
  endif()

  if (A_INCLUDES)
    target_include_directories(${_NAME} ${A_INCLUDES})
  endif()

  if (A_DEFINES)
    target_compile_definitions(${_NAME} ${A_DEFINES})
  endif()

  if (A_AUTORCC)
    set_target_properties(${_NAME} PROPERTIES AUTORCC ON)
  endif()
  if (A_AUTOMOC)
    set_target_properties(${_NAME} PROPERTIES AUTOMOC ON)
  endif()
  if (A_AUTOUIC)
    set_target_properties(${_NAME} PROPERTIES AUTOUIC ON)
  endif()

  if (A_ALIAS)
    add_library(${A_ALIAS} ALIAS ${_NAME})
  endif()
endfunction()

function(my_add_executable _NAME)
  cmake_parse_arguments(A "AUTORCC;AUTOMOC;AUTOUIC" "" "HEADER;SOURCE;DEPENDS;INCLUDES;DEFINES" ${ARGN})
  message(STATUS "Adding executable ${_NAME}")
  add_executable(${_NAME} ${A_HEADER} ${A_SOURCE})

  if (A_DEPENDS)
    target_link_libraries(${_NAME} ${A_DEPENDS})
  endif()

  if (A_INCLUDES)
    target_include_directories(${_NAME} ${A_INCLUDES})
  endif()

  if (A_DEFINES)
    target_compile_definitions(${_NAME} ${A_DEFINES})
  endif()

  if (A_AUTORCC)
    set_target_properties(${_NAME} PROPERTIES AUTORCC ON)
  endif()
  if (A_AUTOMOC)
    set_target_properties(${_NAME} PROPERTIES AUTOMOC ON)
  endif()
  if (A_AUTOUIC)
    set_target_properties(${_NAME} PROPERTIES AUTOUIC ON)
  endif()

endfunction()

function(my_add_test _NAME)
  cmake_parse_arguments(A "AUTORCC;AUTOMOC;AUTOUIC" "" "HEADER;SOURCE;DEPENDS;INCLUDES;DEFINES" ${ARGN})
  message(STATUS "Adding tests ${_NAME}")
  add_executable(${_NAME} ${A_HEADER} ${A_SOURCE})

  if (A_DEPENDS)
    target_link_libraries(${_NAME} ${A_DEPENDS})
  endif()

  if (A_INCLUDES)
    target_include_directories(${_NAME} ${A_INCLUDES})
  endif()

  if (A_DEFINES)
    target_compile_definitions(${_NAME} ${A_DEFINES})
  endif()

  target_link_libraries(${_NAME} gtest gmock gtest_main)

  if (A_AUTORCC)
    set_target_properties(${_NAME} PROPERTIES AUTORCC ON)
  endif()
  if (A_AUTOMOC)
    set_target_properties(${_NAME} PROPERTIES AUTOMOC ON)
  endif()
  if (A_AUTOUIC)
    set_target_properties(${_NAME} PROPERTIES AUTOUIC ON)
  endif()

  gtest_add_tests(TARGET ${_NAME}
                  SOURCES ${A_SOURCE}
  )
endfunction()
