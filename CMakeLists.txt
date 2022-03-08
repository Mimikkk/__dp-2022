cmake_minimum_required(VERSION 3.22)
project(proz_laboratories C)

set(CMAKE_C_STANDARD 23)

find_package(MPI REQUIRED)
add_executable(introduction lab-1-introduction/main.c)
add_executable(parallel-sort lab-2-parallel-sort/main.c)
target_link_libraries(introduction PUBLIC MPI::MPI_C)
target_link_libraries(parallel-sort PUBLIC MPI::MPI_C)
