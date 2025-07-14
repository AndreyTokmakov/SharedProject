cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(ProtoBuff_SimpleDemo)
set(PROJECT_VERSION 1.0.0.0)
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION} LANGUAGES CXX)

add_compile_options(-c -Wall -Wextra -O3 -std=c++26)

# set(PROTO_BUFF_COMPILER "/path/to/protobuf/compiler/protoc")
set(PROTO_BUFF_COMPILER protoc)

set(PROTO_FILES_DIR ${PROJECT_SOURCE_DIR}/proto)
set(PROTO_FILES_GEN ${PROJECT_SOURCE_DIR}/messages)

# Get list of "*.proto" of files in 'proto' folder
file(GLOB PROTO_BUFF_FILES ${PROTO_FILES_DIR}/**.proto)

# For each "*.proto" file in the 'proto' folder calling protobuf compiler
# to generate corresponding *.pb.h and *.pb.cc files into the 'messages' folder.
foreach (proto_file ${PROTO_BUFF_FILES})
    get_filename_component(filename ${proto_file} NAME)
    message("Compiling ${filename} to ${PROTO_FILES_GEN}")
    execute_process(COMMAND ${PROTO_BUFF_COMPILER} --proto_path=${PROTO_FILES_DIR} --cpp_out=${PROTO_FILES_GEN} ${proto_file})
endforeach (proto_file ${PROTO_BUFF_FILES})


# Get all "*.cc" files generated:
file(GLOB PROTO_BUFF_CPP_GEN ${PROTO_FILES_GEN}/**.cc)

# Add all .cc/.cpp protobuf-generated files to compilation target
add_executable(${PROJECT_NAME}
        main.cpp
        ${PROTO_BUFF_CPP_GEN}
)

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${PROTO_FILES_GEN}
)

target_link_libraries(${PROJECT_NAME}
        pthread
        protobuf
        # Or path to '/home/andtokm/DiskS/ProjectsUbuntu/third_party/protobuf/build'
)