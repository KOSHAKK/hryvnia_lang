cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

set(PROJECT_NAME hryvnia_lang)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} 
	src/main.cpp
	src/Lexer.cpp
	
	src/Lexer.hpp
	src/Lexeme.hpp
	src/common.hpp
	
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)


