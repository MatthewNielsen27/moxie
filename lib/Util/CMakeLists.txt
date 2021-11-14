cmake_minimum_required(VERSION 3.18)

add_library(Moxie_Util
        include/Util/AliasTable.hpp
        src/AliasTable.cpp
)

target_include_directories(Moxie_Util PUBLIC include)
