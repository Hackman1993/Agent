cmake_minimum_required(VERSION 3.21)
project(agent)
set(VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 20)
include(CMakePackageConfigHelpers)
file(GLOB_RECURSE CPP_HEADERS include/*.h include/*.hpp)
file(GLOB_RECURSE CPP_SOURCES source/*.cpp include/*.cc)
file(GLOB_RECURSE PROTOBUF_FILES ./source/packages/protos/*.proto)

find_package(Boost REQUIRED COMPONENTS url thread json fiber filesystem)
find_package(fmt CONFIG REQUIRED)
find_package(sahara REQUIRED)
find_package(obelisk REQUIRED)
find_package(Protobuf REQUIRED)

set(Protobuf_IMPORT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/source/packages/protos)
set(protobuf_generate_PROTOC_OUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/packages/protos)

add_library(${PROJECT_NAME} STATIC ${CPP_HEADERS} ${CPP_SOURCES} )
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE sahara::sahara obelisk::obelisk Boost::json Boost::filesystem)
target_link_libraries(${PROJECT_NAME} PRIVATE protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)

message(STATUS ${PROTOBUF_FILES})

add_custom_command(
        PRE_BUILD
        TARGET ${PROJECT_NAME}
        COMMAND protobuf::protoc --proto_path=${Protobuf_IMPORT_DIRS} --cpp_out=${protobuf_generate_PROTOC_OUT_DIR} ${PROTOBUF_FILES}
        VERBATIM )

configure_package_config_file(cmake/template/${PROJECT_NAME}.in.cmake
        ${PROJECT_NAME}-config.cmake
        INSTALL_DESTINATION ./)
write_basic_package_version_file(
        ${PROJECT_NAME}-config-version.cmake
        VERSION ${VERSION}
        COMPATIBILITY ExactVersion)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
        DESTINATION share/${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}-targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY include/ DESTINATION include/${PROJECT_NAME} FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
install(EXPORT ${PROJECT_NAME}-targets NAMESPACE ${PROJECT_NAME}:: DESTINATION share/${PROJECT_NAME})

#target_include_directories(${PROJECT_NAME} PRIVATE D:/Workspace/Cpp/obelisk_asio/include)
#if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#    target_link_directories(${PROJECT_NAME} PRIVATE D:/Workspace/Cpp/obelisk_asio/cmake-build-debug)
#else ()
#    target_include_directories(${PROJECT_NAME} PRIVATE D:/Workspace/Cpp/obelisk_asio/cmake-build-release)
#endif ()