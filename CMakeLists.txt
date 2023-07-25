cmake_minimum_required(VERSION 3.18)

project(RayTracer)

file(GLOB SOURCES "RayTracer/src/*.cpp" "RayTracer/src/*.h")

add_executable(PathTracer ${SOURCES})