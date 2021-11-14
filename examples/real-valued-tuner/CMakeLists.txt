cmake_minimum_required(VERSION 3.18)

add_executable(example_real_valued_tuner
        main.cpp
        Candidate.h
)

target_link_libraries(example_real_valued_tuner
    PUBLIC
        Moxie_Genetics
)
