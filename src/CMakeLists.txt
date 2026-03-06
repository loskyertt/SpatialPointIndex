# for all "src/x.cpp", generate target "x"
message("CMakeLists.txt for src directory")

set(ALL_SRCS
  main.cpp
)

add_executable(my_program ${ALL_SRCS})

target_include_directories(my_program PUBLIC ${PROJECT_SOURCE_DIR}/include)

# 链接构建好的动态库
target_link_libraries(my_program PUBLIC utils)

# MSVC UTF-8 支持
if(MSVC)
  target_compile_options(my_program PRIVATE /utf-8)
endif()

message("\n--------------------------------------------")
