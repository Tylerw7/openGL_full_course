cmake_minimum_required(VERSION 3.20)

project(OpenGL_Lessons LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_SHALLOW TRUE
)

# stb_image
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(stb)

# GLAD
add_library(glad STATIC
    external/glad/src/gl.c
)

target_include_directories(glad PUBLIC
    external/glad/include
)

# macOS OpenGL framework
if(APPLE)
    find_library(OpenGL_FRAMEWORK OpenGL REQUIRED)
endif()

# Find all example main.cpp files
file(GLOB_RECURSE EXAMPLE_FILES
    CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/lessons/*/main.cpp"
)

foreach(EXAMPLE_FILE ${EXAMPLE_FILES})

    get_filename_component(EXAMPLE_DIR
        "${EXAMPLE_FILE}" DIRECTORY
    )

    get_filename_component(EXAMPLE_NAME
        "${EXAMPLE_DIR}" NAME
    )

    # Create separate executable
    add_executable(${EXAMPLE_NAME}
        "${EXAMPLE_FILE}"
        "${PROJECT_SOURCE_DIR}/include/Shader.cpp"
    )

    target_link_libraries(${EXAMPLE_NAME}
        PRIVATE
            glfw
            glad
    )

    if(APPLE)
        target_link_libraries(${EXAMPLE_NAME}
            PRIVATE ${OpenGL_FRAMEWORK}
        )
    endif()

    target_include_directories(${EXAMPLE_NAME}
        PRIVATE ${PROJECT_SOURCE_DIR}/include
        ${stb_SOURCE_DIR}
    )

endforeach()