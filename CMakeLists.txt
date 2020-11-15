cmake_minimum_required(VERSION 3.16)
project(EpitechGameEngine)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Wno-unknown-argument -Wno-c++98-compat -Wno-c++17-extensions -Wno-c++98-compat-pedantic -Wno-global-constructors -Wno-exit-time-destructors -Wno-c99-extensions")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-everything")
endif ()

if (WIN32)
  include(C:/vcpkg/scripts/buildsystems/vcpkg.cmake)

  find_package(glfw3 CONFIG REQUIRED)
  find_package(glm CONFIG REQUIRED)
  find_package(Vulkan REQUIRED)
endif ()

#find_package(portaudio CONFIG REQUIRED)
#find_package(LibSndFile CONFIG REQUIRED)

# find engine/source -name "*.*pp" | sed 's/engine/\$\{CMAKE_CURRENT_LIST_DIR\}\/engine/g'
add_library(
  engine SHARED

  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Log/Log.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Audio/AudioGroup.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Audio/AudioListener.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Audio/AudioListener.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Audio/AudioManager.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Audio/AudioManager.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Audio/AudioSource.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Audio/AudioSource.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Audio/AudioStream.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Audio/AudioStream.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Audio/main.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Camera/Camera.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Camera/Camera.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Components/Camera.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Components/Light.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Components/Model.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Components/Movement.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Components/Physics.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Components/Physics.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Components/Transform.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Components/Gravity.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/ECS/Admin.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/ECS/ClassId.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/ECS/ComponentIdRegister.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/ECS/ComponentIdRegister.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/ECS/Entity.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/ECS/ISystem.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/ECS/System.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/ECS/SystemIdRegister.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/ECS/SystemIdRegister.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Extension/Language/Keywords.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Extension/Language/Library.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Extension/Language/Platform.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Extension/Libraries/glm.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Extension/Libraries/stb_image.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Extension/Libraries/tiny_obj_loader.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Extension/Libraries/Vulkan.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Extension/Libraries/WinApi.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Game.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Game.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Global/ApplicationBase.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Global/ApplicationBase.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Input/Key.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Input/MouseButton.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Input/State.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Maths/Math.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Maths/Matrix.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Maths/Quaternion.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Maths/Quaternion.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Maths/Vectors.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Model/IModel.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Model/OBJ.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Model/OBJ.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/ICollisionShape.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/Shapes/AABB.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/Shapes/AABB.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/Shapes/Capsule.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/Shapes/Capsule.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/Shapes/OBB.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/Shapes/OBB.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/Shapes/Raycasting.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/Shapes/Raycasting.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/Shapes/Sphere.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/Shapes/Sphere.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/ShapeType.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Systems/Gravity.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Systems/Gravity.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Systems/Physics.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Systems/Physics.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Vulkan/Buffer.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Vulkan/Buffer.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Vulkan/Drawable.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Vulkan/Drawable.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Vulkan/Image.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Vulkan/Image.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Vulkan/Model.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Vulkan/Model.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Vulkan/QueueFamilies.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Vulkan/QueueFamilies.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Vulkan/Renderer.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Vulkan/Renderer.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Vulkan/Swapchain.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Vulkan/Swapchain.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Vulkan/SwapchainDetails.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Vulkan/SwapchainDetails.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Vulkan/UniformBufferData.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Vulkan/Vertex.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Vulkan/Vertex.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Windows/Window.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Windows/Window.hpp
)

target_compile_definitions(
  engine PRIVATE

  SHARED_LIBRARY_EXPORT
)

target_include_directories(
  engine PRIVATE

  ${CMAKE_CURRENT_LIST_DIR}/engine/source
  ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(
  engine PRIVATE

  portaudio
  portaudio_static
  sndfile-shared
  glfw
  glm
  ${Vulkan_LIBRARIES}
)

add_executable(
  demo

  ${CMAKE_CURRENT_LIST_DIR}/demo/source/Demo.cpp
)

target_compile_definitions(
  demo PRIVATE

  SHARED_LIBRARY_IMPORT
)

target_include_directories(
  demo PRIVATE

  ${CMAKE_CURRENT_LIST_DIR}/engine/source
  ${CMAKE_CURRENT_LIST_DIR}/demo/source
  ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(
  demo PRIVATE

  engine
  portaudio
  portaudio_static
  sndfile-shared
  glfw
  glm
  ${Vulkan_LIBRARIES}
)

add_executable(
  demo_physics

  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Components/Model.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Components/Movement.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Components/Physics.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Components/Physics.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Components/Gravity.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Components/Transform.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/ECS/Admin.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/ECS/SystemIdRegister.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/ECS/SystemIdRegister.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/ECS/Entity.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/ECS/ISystem.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/ECS/System.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/ECS/ComponentIdRegister.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/ECS/ComponentIdRegister.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Extension/Language/Keywords.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Extension/Language/Platform.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Maths/Math.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Maths/Matrix.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Maths/Quaternion.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Maths/Quaternion.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Maths/Vectors.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/ICollisionShape.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/Shapes/AABB.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/Shapes/AABB.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/Shapes/OBB.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/Shapes/OBB.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/Shapes/Capsule.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/Shapes/Capsule.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/Shapes/Raycasting.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/Shapes/Sphere.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/Shapes/Sphere.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/ShapeType.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Systems/Physics.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Systems/Physics.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Systems/Gravity.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Systems/Gravity.hpp
  ${CMAKE_CURRENT_LIST_DIR}/mainTestCollision.cpp
)

target_include_directories(
  demo_physics PRIVATE

  ${CMAKE_CURRENT_LIST_DIR}/engine/source
)

add_executable(
  demo_physics2

  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Components/Model.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Components/Movement.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Components/Gravity.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Components/Physics.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Components/Physics.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Components/Transform.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/ECS/Admin.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/ECS/SystemIdRegister.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/ECS/SystemIdRegister.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/ECS/Entity.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/ECS/ISystem.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/ECS/System.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/ECS/ComponentIdRegister.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/ECS/ComponentIdRegister.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Extension/Language/Keywords.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Extension/Language/Platform.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Maths/Math.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Maths/Matrix.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Maths/Quaternion.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Maths/Quaternion.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Maths/Vectors.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/ICollisionShape.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/Shapes/AABB.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/Shapes/AABB.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/Shapes/OBB.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/Shapes/OBB.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/Shapes/Capsule.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/Shapes/Capsule.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/Shapes/Raycasting.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/Shapes/Sphere.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/Shapes/Sphere.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Physics/ShapeType.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Systems/Physics.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Systems/Physics.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Systems/Gravity.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Systems/Gravity.hpp
  ${CMAKE_CURRENT_LIST_DIR}/mainTestCollision2.cpp
)

target_include_directories(
  demo_physics2 PRIVATE

  ${CMAKE_CURRENT_LIST_DIR}/engine/source
)

add_executable(
  demo_audio

  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Audio/main.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Audio/AudioGroup.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Audio/AudioManager.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Audio/AudioManager.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Audio/AudioListener.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Audio/AudioListener.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Audio/AudioSource.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Audio/AudioSource.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Audio/AudioStream.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/source/Audio/AudioStream.hpp
)

target_include_directories(
  demo_audio PRIVATE

  ${CMAKE_CURRENT_LIST_DIR}/engine/source
)

target_link_libraries(
  demo_audio PRIVATE

  portaudio
  portaudio_static
  sndfile-shared
)

if (WIN32)
find_package(GLUT REQUIRED)

target_link_libraries(
  demo_physics PRIVATE

  OpenGL32
  GLUT::GLUT
  GLU32
)

target_link_libraries(
  demo_physics2 PRIVATE

  OpenGL32
  GLUT::GLUT
  GLU32
)
endif ()

if (UNIX)
target_link_libraries(
  demo_physics PRIVATE

  GL
  glut
  GLU
)

target_link_libraries(
  demo_physics2 PRIVATE

  GL
  glut
  GLU
)
endif ()

# if (EXISTS "${CMAKE_BINARY_DIR}/compile_commands.json")
#   file(STRINGS ${CMAKE_BINARY_DIR}/compile_commands.json COMPILE_COMMANDS_FLAGS REGEX "^.*command.*main\.cpp.*$")
#   string(REGEX REPLACE "^.*clang-cl\.exe" "" COMPILE_COMMANDS_FLAGS "${COMPILE_COMMANDS_FLAGS}")
#   string(REGEX REPLACE "-std:.*$" "" COMPILE_COMMANDS_FLAGS "${COMPILE_COMMANDS_FLAGS}")
#   string(REGEX REPLACE "(\/[^ ]+)" " " COMPILE_COMMANDS_FLAGS "${COMPILE_COMMANDS_FLAGS}")
#   string(REGEX REPLACE "  *" " " COMPILE_COMMANDS_FLAGS "${COMPILE_COMMANDS_FLAGS}")
#   string(REGEX REPLACE " " "\n" COMPILE_COMMANDS_FLAGS "${COMPILE_COMMANDS_FLAGS}")
#   string(REGEX REPLACE "^\n" "" COMPILE_COMMANDS_FLAGS "${COMPILE_COMMANDS_FLAGS}")
#   string(REPLACE "-I.." "-I." COMPILE_COMMANDS_FLAGS "${COMPILE_COMMANDS_FLAGS}")
#   file(WRITE "${CMAKE_CURRENT_LIST_DIR}/compile_flags.txt" "${COMPILE_COMMANDS_FLAGS}")
# endif ()
