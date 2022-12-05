cmake_minimum_required(VERSION 3.10)
project(DLMS_Client_Server)

set(CMAKE_CXX_STANDARD 14)


include_directories(HEADER_FILES PRIVATE headerFiles )
include_directories(SOURCE_FILES PRIVATE sourceFiles )


# Add all *.cpp files in src as source files
FILE (GLOB_RECURSE CLIENT_SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/Client/*.cpp ${PROJECT_SOURCE_DIR}/src/Common/*.cpp)
FILE (GLOB_RECURSE SERVER_SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/Server/*.cpp ${PROJECT_SOURCE_DIR}/src/Common/*.cpp)

add_executable(Client ${CLIENT_SOURCE_FILES})
add_executable(Server ${SERVER_SOURCE_FILES})

target_include_directories(Client
    PUBLIC
        include/Common
        include/Client
)

target_include_directories(Server
    PUBLIC
        include/Common
        include/Server
)

target_link_libraries(Client
        gurux_dlms_cpp)

target_link_libraries(Server
        gurux_dlms_cpp
        pugixml)

target_compile_definitions(Client
        PUBLIC
        -DBOOST_NO_CXX11_SCOPED_ENUMS)

target_compile_definitions(Server
        PUBLIC
        -DBOOST_NO_CXX11_SCOPED_ENUMS)

# directory with other CMakeLists.txt
add_subdirectory(lib_GURUX)
add_subdirectory(lib_PugiXML)

IF (WIN32)
target_link_libraries(Client ws2_32.lib)
target_link_libraries(Server ws2_32.lib)
ELSE()
target_link_libraries(Client boost_system boost_filesystem)
target_link_libraries(Server pthread rt)
    add_compile_options(-g -rdynamic)
ENDIF (WIN32)