cmake_minimum_required (VERSION 3.8)

project (server LANGUAGES CXX VERSION 1.0.0)

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

file(GLOB SRC
	*.cpp
	*.h*
)

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME}
	Threads::Threads
)
