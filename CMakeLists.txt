
cmake_minimum_required(VERSION 3.1)

set(project_name recognizer-bot)

project(${project_name})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

add_library(${project_name}-lib
    src/modules/Logger.cpp
    src/service/bot/BotSync.cpp
    src/service/bot/BotAsync.cpp
    src/service/bot/Bot.cpp
    src/service/bot/handler/EventHandler.cpp
    src/service/Users.cpp
)

## link libs

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,/usr/local/lib")

find_package(httplib REQUIRED)
find_package(spdlog REQUIRED)
find_package(jsoncpp REQUIRED)
find_package(SQLiteCpp REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

target_link_libraries(${project_name}-lib
    PUBLIC httplib::httplib
    PUBLIC spdlog
    PUBLIC jsoncpp
    PUBLIC SQLiteCpp
    PUBLIC stdc++fs
    PUBLIC TgBot
    PUBLIC ${CMAKE_THREAD_LIBS_INIT} 
    PUBLIC ${OPENSSL_LIBRARIES}
)
target_link_directories(${project_name}-lib PUBLIC "/usr/local/lib")

## add executables


include_directories(${LIBHTTPSERVER_INCLUDE_DIRS})

add_definitions(
    ## httplib
    -DCPPHTTPLIB_OPENSSL_SUPPORT
    -DCPPHTTPLIB_ZLIB_SUPPORT

    ## Bot will send message to user with predefined text
    -DNOTIFY_USER_ON_ERROR
)

add_executable(${project_name}-exe
    src/Entry.cpp
)
include_directories(${project_name}-lib ${project_name}-exe
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/src/modules"
)

target_link_libraries(${project_name}-exe ${project_name}-lib)
add_dependencies(${project_name}-exe ${project_name}-lib)


set_target_properties(${project_name}-exe PROPERTIES
    CXX_STANDARD 11
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
)

