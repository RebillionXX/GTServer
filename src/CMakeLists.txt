project(server LANGUAGES CXX)

file(GLOB INCLUDE_FILES
    *.h
    player/*.h
    proton/*.h
    server/*.h)
file(GLOB SOURCE_FILES
    *.cpp
    player/*.cpp
    proton/*.cpp
    server/*.cpp)

add_executable(${PROJECT_NAME}
    ${INCLUDE_FILES}
    ${SOURCE_FILES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    C_STANDARD 11
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    NOMINMAX
    WIN32_LEAN_AND_MEAN
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/enet/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/fmt/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/proton/shared
)

target_link_libraries(${PROJECT_NAME} 
    enet
    fmt
)
