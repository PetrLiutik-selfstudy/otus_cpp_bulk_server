cmake_minimum_required(VERSION 3.2)

if (DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(bulk_server VERSION 1.1.$ENV{TRAVIS_BUILD_NUMBER})
else ()
    project(bulk_server VERSION 1.1.1)
endif ()

find_package(Threads REQUIRED)
find_package(Boost REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)

file(GLOB_RECURSE SOURCES *.cpp *.h)

# Setup application
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/inc ${CMAKE_SOURCE_DIR}/src)

configure_file(version.h.in ${PROJECT_SOURCE_DIR}/version.h)

if (MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES
                                          CXX_STANDARD 14
                                          CXX_STANDARD_REQUIRED ON
    )
else ()
    set_target_properties(${PROJECT_NAME} PROPERTIES
                                          CXX_STANDARD 14
                                          CXX_STANDARD_REQUIRED ON
                                          COMPILE_OPTIONS -Wpedantic -Wall -Wextra
    )
endif ()

target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_CONTACT petr_lutik@mail.ru)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

include (CPack)
