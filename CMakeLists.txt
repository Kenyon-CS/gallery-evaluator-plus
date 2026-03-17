cmake_minimum_required(VERSION 3.10)
project(gallery_evaluator_plus)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

add_executable(gallery_eval_plus
    src/main.cpp
    src/parser.cpp
    src/evaluator.cpp
    src/generators.cpp
    src/csv_writer.cpp
)
