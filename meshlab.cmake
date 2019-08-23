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
