project(server LANGUAGES CXX)

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

if (MSVC)
    add_definitions(/EHsc)
else ()
    add_definitions(-fexceptions)
endif ()

if (CONAN_EXPORTED)
    include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else ()
    include(${CMAKE_CURRENT_SOURCE_DIR}/conan.cmake)
    conan_cmake_configure(REQUIRES
            fmt/9.0.0
            openssl/3.0.5
            libcurl/7.80.0
            mariadb-connector-c/3.1.12
            sqlpp11/0.61
        GENERATORS cmake_find_package
        IMPORTS "bin, *.dll -> ./bin"
        OPTIONS
            fmt:shared=True
            openssl:shared=True
            libcurl:shared=True
            mariadb-connector-c:shared=True)

    conan_cmake_autodetect(settings)
    conan_cmake_install(PATH_OR_REFERENCE .
        BUILD missing
        REMOTE conancenter
        SETTINGS ${settings})
endif ()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_BINARY_DIR}")

find_package(fmt REQUIRED)
if (fmt_FOUND)
    target_include_directories(${PROJECT_NAME} PUBLIC ${fmt_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${fmt_LIBRARIES})
endif ()

find_package(mariadb-connector-c REQUIRED)
if (mariadb-connector-c_FOUND)
    target_include_directories(${PROJECT_NAME} PUBLIC ${mariadb-connector-c_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${mariadb-connector-c_LIBRARIES})
endif ()

find_package(Sqlpp11 REQUIRED)
if (Sqlpp11_FOUND)
    target_include_directories(${PROJECT_NAME} PUBLIC ${sqlpp11_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${sqlpp11_LIBRARIES})
endif ()

set_target_properties(${PROJECT_NAME} PROPERTIES
    C_STANDARD 11
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    NOMINMAX
    WIN32_LEAN_AND_MEAN
    CPPHTTPLIB_OPENSSL_SUPPORT
)
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/cpp-httplib
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/enet/include
)
target_link_libraries(${PROJECT_NAME} 
    enet
)