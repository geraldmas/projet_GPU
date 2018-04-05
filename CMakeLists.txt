cmake_minimum_required(VERSION 2.6)

                

#Déclaration du projet

project(GPU_Project)



#Génération de la liste des fichiers sources

file(

        GLOB_RECURSE

        source_files

        src/*

)



#Déclaration de l'exécutable

add_executable(BS_Monte_Carlo ${source_files})

SET(GCC_COVERAGE_COMPILE_FLAGS "-std=c++11 -fopenmp")

add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()