cmake_minimum_required (VERSION 2.8.9)

project (html-print)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
set(CMAKE_CXX_FLAGS "-Wall -std=c++11 ${CMAKE_CXX_FLAGS}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(Cef3 REQUIRED)

set (INCLUDES ${INCLUDES} ${CEF3_INCLUDE_DIRS} )
set (LIBS ${LIBS} ${CEF3_LIBRARIES} )

include_directories(${INCLUDES})
add_executable(html-print html-print.cpp)
target_link_libraries(html-print ${LIBS})