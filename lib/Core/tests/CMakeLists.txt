cmake_minimum_required(VERSION 3.18)


find_package(Catch2 3 REQUIRED)


add_executable(catch_Core
        catch_Crossover.cpp
        catch_Genome.cpp
)

target_link_libraries(catch_Core
    PUBLIC
        Moxie_Core
    PRIVATE
        Catch2::Catch2WithMain
)
