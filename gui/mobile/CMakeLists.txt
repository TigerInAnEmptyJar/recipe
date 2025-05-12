my_add_executable(recipe-mobile
  SOURCE
    src/main.cpp
    qml/qml.qrc
  DEPENDS
    Qt6::Core
    Qt6::Qml
    Qt6::Quick
    Qt6::Gui
    Qt6::DBus
  AUTORCC
  AUTOMOC
)
file(COPY ${qt_PACKAGE_FOLDER_DEBUG}/plugins/platforms DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/plugins/)
