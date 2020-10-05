cmake_minimum_required( VERSION 3.6 )

# Create Project
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId} CXX)

message(${ProjectId})

# If you use visual studio, set the character code to UTF8
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# In debug mode, add 'd' to the end of the executable file name
if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

# set output file directory
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../bin)

# define path of Eigen
file(TO_CMAKE_PATH $ENV{EIGEN3_INCLUDE_DIR} EIGEN3_INCLUDE_DIR)
# define path of vcglib
file(TO_CMAKE_PATH $ENV{VCGLIB_DIR} VCGLIB_DIR)
# define path of meshlab source
file(TO_CMAKE_PATH $ENV{MESHLAB_DIR} MESHLAB_DIR)
add_definitions(-DMESHLAB_SCALAR=float) # you should add this definition if you use meshlab source

# set(VCGLIB_DIR C:/Users/Public/Documents/GitHub/vcglib)
# set(MESHLAB_DIR C:/Users/Public/Documents/GitHub/meshlab_latest/src)

# Add Executable
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
  PRIVATE
    main.cpp
	${VCGLIB_DIR}/wrap/ply/plylib.h
	${VCGLIB_DIR}/wrap/ply/plylib.cpp
)
target_include_directories(${PROJECT_NAME}
  PUBLIC
  ${EIGEN3_INCLUDE_DIR}
  ${VCGLIB_DIR}
  ${MESHLAB_DIR}
)

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
