cmake_minimum_required(VERSION 3.13)
project(factd)

set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE ssrc "src/daemon/*.c")
file(GLOB_RECURSE csrc "src/client/*.c")

add_executable(factd ${ssrc})
add_executable(factc ${csrc})

target_compile_options(factd PUBLIC -Wall)
target_include_directories(factd PUBLIC
	"src/"
)
target_link_libraries(factd
	rt pthread
)

target_compile_options(factc PUBLIC -Wall)
target_include_directories(factc PUBLIC
	"src/"
)
target_link_libraries(factc
	rt pthread
)
