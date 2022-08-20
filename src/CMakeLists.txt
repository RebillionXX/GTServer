project(server LANGUAGES CXX)

find_package(OpenSSL REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(unofficial-enet CONFIG REQUIRED)

find_path(CPP_HTTPLIB_INCLUDE_DIRS "httplib.h")

file(GLOB INCLUDE_FILES
    *.h
    **/*.h
    **/**/*.h
    **/**/**/*.h)
file(GLOB SOURCE_FILES
    *.cpp
    **/*.cpp
    **/**/*.cpp
    **/**/**/*.cpp)

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
    ${CPP_HTTPLIB_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/mariadb/include
)

target_link_libraries(${PROJECT_NAME}
    unofficial::enet::enet
    fmt::fmt
    OpenSSL::SSL OpenSSL::Crypto
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/mariadb/mariadbcpp.lib
)


