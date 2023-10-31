@PACKAGE_INIT@

include(CMakeFindDependencyMacro)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

find_dependency(Boost REQUIRED COMPONENTS json)
find_dependency(sahara REQUIRED)
find_dependency(obelisk REQUIRED)

include(${CMAKE_CURRENT_LIST_DIR}/agent-targets.cmake)
if(@Boost_FOUND@)
  include_directories(${Boost_INCLUDE_DIRS})
endif()