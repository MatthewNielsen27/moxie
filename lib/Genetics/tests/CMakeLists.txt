cmake_minimum_required(VERSION 3.18)


find_package(Catch2 3 REQUIRED)


add_executable(catch_Genetics
        catch_Crossover.cpp
        catch_Genome.cpp
        catch_Selection.cpp
)

target_link_libraries(catch_Genetics
    PUBLIC
        Moxie_Genetics
    PRIVATE
        Catch2::Catch2WithMain
)
