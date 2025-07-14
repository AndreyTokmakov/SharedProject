cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(ClientServerUIDemo)
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -pthread")
set(PROJECT_VERSION 1.0.0.0)
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION} LANGUAGES CXX)

add_compile_options(-c -Wall -Wextra -O3 -std=c++26)

# QT control flags.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# QT dependencies.
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)

set(PROTO_BUFF_COMPILER protoc)

set(COMMON ${PROJECT_SOURCE_DIR}/common)
set(CLIENT ${PROJECT_SOURCE_DIR}/client)
set(SERVER ${PROJECT_SOURCE_DIR}/server)
set(PROTO_FILES_DIR ${PROJECT_SOURCE_DIR}/schema)
set(PROTO_FILES_GEN ${PROJECT_SOURCE_DIR}/schema_out)

set(DEST_INCLUDE_FILE ${COMMON}/ProtobufMessages.h)

# Get list of "*.proto" of files in 'schema' folder
file(GLOB PROTO_BUFF_FILES ${PROTO_FILES_DIR}/**.proto)

foreach (proto_file ${PROTO_BUFF_FILES})
    get_filename_component(filename ${proto_file} NAME)
    message("Compiling ${filename} to ${PROTO_FILES_GEN}")
    execute_process(COMMAND ${PROTO_BUFF_COMPILER} --proto_path=${PROTO_FILES_DIR} --cpp_out=${PROTO_FILES_GEN} ${proto_file})
endforeach (proto_file ${PROTO_BUFF_FILES})

# Get all "*.cc" files generated:
file(GLOB PROTO_BUFF_GENERATED_SOURCES ${PROTO_FILES_GEN}/**.cc)

# Get all "*.h" files generated:
file(GLOB PROTO_BUFF_GENERATED_HEADERS ${PROTO_FILES_GEN}/**.h)

foreach (proto_header_file ${PROTO_BUFF_GENERATED_HEADERS})
    get_filename_component(filename ${proto_header_file} NAME)
    list(APPEND INCL_FILE_CONTENT "#include \"${filename}\"\n")
endforeach (proto_header_file ${PROTO_BUFF_GENERATED_HEADERS})

# Create 'ProtobufMessages.h' file with 'INCL_FILE_CONTENT' content
file(WRITE ${DEST_INCLUDE_FILE} ${INCL_FILE_CONTENT})

include_directories(${COMMON})
include_directories(${CLIENT})
include_directories(${SERVER})
include_directories(${PROTO_FILES_GEN})

# qt5_generate_moc(ui/ClientWindow.cpp ui/ClientWindow.moc)

add_executable(${PROJECT_NAME}
        main.cpp
        client/TcpClient.cpp
        server/TcpServer.cpp
        ui/ClientWindow.cpp
        common/RequestBuilder.cpp
        ${PROTO_BUFF_GENERATED_SOURCES}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        pthread
        protobuf
        ${EXTRA_LIBS}
        ${Qt5Widgets_LIBRARIES}
)