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

SET(GCC_COVERAGE_COMPILE_FLAGS "-std=c++11")

add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})