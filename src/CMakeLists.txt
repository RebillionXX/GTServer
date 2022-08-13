project(server LANGUAGES CXX)

file(GLOB INCLUDE_FILES
    *.h
    database/*.h
    database/item/*.h
    events/*.h
    events/text_events/*.h
    events/text_events/action/*.h
    player/*.h
    proton/*.h
    proton/utils/*.h
    server/*.h
    utils/*.h)
file(GLOB SOURCE_FILES
    *.cpp
    database/*.cpp
    database/item/*.cpp
    events/*.cpp
    events/text_events/*.cpp
    events/text_events/action/*.cpp
    player/*.cpp
    proton/*.cpp
    proton/utils/*.cpp
    server/*.cpp
    utils/*.cpp)

add_executable(${PROJECT_NAME}
    ${INCLUDE_FILES}
    ${SOURCE_FILES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    C_STANDARD 11
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    NOMINMAX
    WIN32_LEAN_AND_MEAN
    CPPHTTPLIB_OPENSSL_SUPPORT
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/cpp-httplib/
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/enet/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/fmt/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/mysql/include
)

target_link_libraries(${PROJECT_NAME} 
    enet
    fmt  
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/mysql/lib/libmysql.lib 
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/mysql/lib/mysqlclient.lib 
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/mysql/lib/mysqlcppconn.lib 
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/mysql/lib/mysqlcppconn-static.lib 
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/mysql/lib/mysqlservices.lib
)
