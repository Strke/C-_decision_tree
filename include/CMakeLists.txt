# 把该文件加的所有文件 用变量DIR_LIB_SRCS表示
AUX_SOURCE_DIRECTORY(. DIR_LIB_SRCS)
# 设置编译后输出的库文件路径
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
# 将mylib文件，生成Mylib.a库文件
ADD_LIBRARY(Mylib STATIC ${DIR_LIB_SRCS})
aux_source_directory(. DIR_LIB_SRCS)
