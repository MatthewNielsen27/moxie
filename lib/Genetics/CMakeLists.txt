cmake_minimum_required(VERSION 3.18)


add_library(Moxie_Genetics
        include/Genetics/Genome.hpp
        include/Genetics/Crossover.hpp
        include/Genetics/Selection.hpp
        src/Crossover.cpp
        src/Selection.cpp
)

target_include_directories(Moxie_Genetics PUBLIC include)

target_link_libraries(Moxie_Genetics
    PUBLIC
        Moxie_Util
)


add_subdirectory(tests)
