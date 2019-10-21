cmake_minimum_required( VERSION 3.6 )

# Create Project
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId} CXX)
message(${ProjectId})

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

message("---meshlab setting---")
set( VCGLIB_DIR C:/Users/nitta/Documents/GitLab/vcglib)
# set( VCGLIB_DIR $ENV{VCGLIB_DIR})
include_directories(${VCGLIB_DIR})
message(${VCGLIB_DIR})

# set( MESHLAB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../library)
set( MESHLAB_DIR $ENV{MESHLAB_DIR})
include_directories(${MESHLAB_DIR})
message(${MESHLAB_DIR})

set( EIGEN_ROOT $ENV{EIGEN_ROOT})
include_directories(${EIGEN_ROOT})
message(${EIGEN_ROOT})

# Additional Dependencies
add_definitions(-DMESHLAB_SCALAR=float)

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../bin)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE
	main.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

# target_link_libraries(${PROJECT_NAME} 
# )