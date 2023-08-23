cmake_minimum_required (VERSION 3.8)

project (server LANGUAGES CXX C VERSION 1.0.0)
message(Before=${CMAKE_MODULE_PATH})
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /home/salman/code/cpp/benchmarks/drogon/install_dir/lib64/cmake)
message(After=${CMAKE_MODULE_PATH})

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(/home/salman/code/cpp/benchmarks/drogon/install_dir/usr/local/include)
link_directories(/home/salman/code/cpp/benchmarks/drogon/install_dir/usr/local/lib64)
link_directories(/home/salman/usr/bin/openssl-1.1.1o/install/lib)

# Pass boost location using ENV variable or using -D
if(DEFINED ENV{BOOST_ROOT})
  set(BOOST_DIR $ENV{BOOST_ROOT})
elseif (BOOST_ROOT)
  set(BOOST_DIR  ${BOOST_ROOT})
endif()

if(BOOST_DIR)
  IF(NOT EXISTS ${BOOST_DIR})
    message(FATAL_ERROR "Invalid boost root directory. ${BOOST_DIR}")
  endif()
  message(STATUS "Using Boost installed at ${BOOST_DIR} ")
  include_directories(${BOOST_DIR})
else()
  message(STATUS "Using systems' boost ")
endif()


find_package(Threads REQUIRED)

file(GLOB SRC
	*.cc
	*.c
	*.cpp
	*.h
)

message ("-----> Src files ${SRC}")

find_package(OpenSSL REQUIRED)
find_package(Drogon CONFIG REQUIRED)

include(FetchContent)

FetchContent_Declare(
  glaze
  GIT_REPOSITORY https://github.com/stephenberry/glaze.git
  GIT_TAG main
  GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(glaze)


add_executable(${PROJECT_NAME} ${SRC})
#target_compile_options(${PROJECT_NAME} PUBLIC -g)
target_compile_options(${PROJECT_NAME} PUBLIC -O3)
target_link_libraries(${PROJECT_NAME} PRIVATE z jsoncpp uuid Drogon::Drogon Trantor::Trantor  OpenSSL::SSL glaze::glaze)
