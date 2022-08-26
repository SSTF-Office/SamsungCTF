cmake_minimum_required(VERSION 3.22)
project(LuQwest C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_SKIP_BUILD_RPATH TRUE)

link_directories(src/lua)
add_executable(${PROJECT_NAME}
        src/main.c
        src/ui.c
        src/ui.h
        src/luqwest.c
        src/luqwest.h
        src/base64.c
)

target_link_libraries(${PROJECT_NAME} lua m dl)
