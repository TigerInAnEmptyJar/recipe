# Recipe

Manage your recipes, create weekly meal-plans, offer the meal-plan to your family for subscription, create a shopping list, print recipes, display all on mobile device.

This repository comes with 3 applications:
* a desktop application for maintaining the recipe database
* a mobile application for meal-plan subscription and display of recipes
* a webserver for communication between desktop and mobile application

## Build requirements

* CMake 3.27 or higher
* conan 2.0 or higher 
* Qt 5.15.2

### Build on Linux

Download the cmake wrapper for conan and the repository

```
git clone https://github.com/tigerInAnEmptyJar/recipe.git
git clone https://github.com/conan-io/cmake-conan.git -b develop2
```

If you have both repositories next to each other, you can directly use the provided CMake preset:

```
cd recipe
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
```

## Start the webserver

See webserver/README.md