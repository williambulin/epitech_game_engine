cmake_minimum_required(VERSION 3.16)
project(epitech_game_engine)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Wno-unknown-argument -Wno-c++98-compat -Wno-c++17-extensions -Wno-c++98-compat-pedantic -Wno-global-constructors -Wno-exit-time-destructors -Wno-c99-extensions")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")
endif ()

add_executable(
  epitech_game_engine

  ${CMAKE_CURRENT_LIST_DIR}/source/Core.cpp
  ${CMAKE_CURRENT_LIST_DIR}/source/Core.hpp
  ${CMAKE_CURRENT_LIST_DIR}/source/main.cpp
  ${CMAKE_CURRENT_LIST_DIR}/source/Extension/Language/Keywords.hpp
  ${CMAKE_CURRENT_LIST_DIR}/source/Extension/Language/Platform.hpp
  ${CMAKE_CURRENT_LIST_DIR}/source/Global/ApplicationBase.cpp
  ${CMAKE_CURRENT_LIST_DIR}/source/Global/ApplicationBase.hpp
  ${CMAKE_CURRENT_LIST_DIR}/source/Maths/Matrix.hpp
  ${CMAKE_CURRENT_LIST_DIR}/source/Maths/Quaternion.cpp
  ${CMAKE_CURRENT_LIST_DIR}/source/Maths/Quaternion.hpp
  ${CMAKE_CURRENT_LIST_DIR}/source/Maths/Vectors.hpp
  ${CMAKE_CURRENT_LIST_DIR}/source/Model/Converter/Assimp.hpp
  ${CMAKE_CURRENT_LIST_DIR}/source/Model/AnimatedModel.cpp
  ${CMAKE_CURRENT_LIST_DIR}/source/Model/AnimatedModel.hpp
  ${CMAKE_CURRENT_LIST_DIR}/source/Model/IAnimatedModel.hpp
  ${CMAKE_CURRENT_LIST_DIR}/source/Model/IModel.hpp
  ${CMAKE_CURRENT_LIST_DIR}/source/Model/Model.cpp
  ${CMAKE_CURRENT_LIST_DIR}/source/Model/Model.hpp
  ${CMAKE_CURRENT_LIST_DIR}/source/Model/OBJ.cpp
  ${CMAKE_CURRENT_LIST_DIR}/source/Model/OBJ.hpp
)

target_include_directories(
  epitech_game_engine PRIVATE

  ${CMAKE_CURRENT_LIST_DIR}/source
)

find_package(
  Assimp CONFIG REQUIRED
)

target_link_libraries(
  epitech_game_engine PRIVATE
	
  assimp::assimp
)

if(EXISTS "${CMAKE_BINARY_DIR}/compile_commands.json")
  file(STRINGS ${CMAKE_BINARY_DIR}/compile_commands.json COMPILE_COMMANDS_FLAGS REGEX "^.*command.*main\.cpp.*$")
  string(REGEX REPLACE "^.*clang-cl\.exe" "" COMPILE_COMMANDS_FLAGS "${COMPILE_COMMANDS_FLAGS}")
  string(REGEX REPLACE "-std:.*$" "" COMPILE_COMMANDS_FLAGS "${COMPILE_COMMANDS_FLAGS}")
  string(REGEX REPLACE "(\/[^ ]+)" " " COMPILE_COMMANDS_FLAGS "${COMPILE_COMMANDS_FLAGS}")
  string(REGEX REPLACE "  *" " " COMPILE_COMMANDS_FLAGS "${COMPILE_COMMANDS_FLAGS}")
  string(REGEX REPLACE " " "\n" COMPILE_COMMANDS_FLAGS "${COMPILE_COMMANDS_FLAGS}")
  string(REGEX REPLACE "^\n" "" COMPILE_COMMANDS_FLAGS "${COMPILE_COMMANDS_FLAGS}")
  string(REPLACE "-I.." "-I." COMPILE_COMMANDS_FLAGS "${COMPILE_COMMANDS_FLAGS}")
  file(WRITE "${CMAKE_CURRENT_LIST_DIR}/compile_flags.txt" "${COMPILE_COMMANDS_FLAGS}")
endif()
