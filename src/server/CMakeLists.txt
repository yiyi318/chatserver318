
set(EXCUTABLE_OUTPUT_PATH${PROJECT_SOURCE_DIR}/data/ky/chat/bin)
#定义了一个SRC_LIST变量 包含该目录下所有的源文件
aux_source_directory(. SRC_List)
aux_source_directory(./model MODEL_List)
#生成可执行文件
add_executable(ChatServer ${SRC_List} ${MODEL_List})

#
target_link_libraries(ChatServer muduo_net muduo_base mysqlclient hiredis pthread)
