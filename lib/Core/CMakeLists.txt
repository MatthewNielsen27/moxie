cmake_minimum_required(VERSION 3.18)


add_library(Moxie_Core
        include/Core/Genome.hpp
        include/Core/Crossover.hpp
        src/Crossover.cpp

        include/Core/Selection.hpp
        include/Core/AliasTable.hpp
        src/AliasTable.cpp
)

target_include_directories(Moxie_Core PUBLIC include)

add_subdirectory(tests)
