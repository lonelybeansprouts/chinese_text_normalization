#递归获取目录下所有的C文件
#file(GLOB c_files ./*.cc)

# 递归获取目录下所有的h文件
#file(GLOB h_files ./*.h)

include_directories(/opt/kaldi/tools/openfst/include)

link_directories(/opt/kaldi/tools/openfst/lib)

#add_definitions(-D KENLM_MAX_ORDER=6)


# 设置编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3 -Wall -fpermissive -Wno-sign-compare -Wno-unused-local-typedefs -Wno-deprecated-declarations -Winit-self -std=c++14")

# 生成执行文件
add_executable(test test.cc)

# 链接库文件
target_link_libraries(test fst)
