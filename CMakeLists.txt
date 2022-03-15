cmake_minimum_required(VERSION 3.22)
project(proz_laboratories C)

set(CMAKE_C_STANDARD 23)

find_package(MPI REQUIRED)
add_executable(introduction lab-1-introduction/main.c)
add_executable(parallel-sort lab-2-parallel-sort/main.c)
add_executable(password-cracking lab-3-password-cracking/main.c)
target_link_libraries(introduction PUBLIC MPI::MPI_C)
target_link_libraries(introduction PUBLIC MPI::MPI_C)
target_link_libraries(password-cracking PUBLIC MPI::MPI_C)
target_link_libraries(password-cracking PUBLIC crypt)
