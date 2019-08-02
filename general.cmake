# https://stackoverflow.com/questions/22804287/cmake-file-structure-how-to-split-a-cmakelists-txt-into-multiple-files
# OpenMesh,Qt(必要ならば)の読み込み
# 実行ファイルの出力ディレクトリとpostfix設定

cmake_minimum_required( VERSION 3.6 )
option(USE_QT_SYSTEM "Use Qt system" OFF) # https://www.wagavulin.jp/entry/2011/11/27/222650
# set(USE_QT_SYSTEM OFF)

message("-----load general setting-----")

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>") # 
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/W2>") # warningレベルの変更 3,4だとnoteまで出るのでテンプレート展開で大量にログが出る。https://foonathan.net/blog/2018/10/17/cmake-warnings.html
if(MSVC)
   # add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>") # これ単体でも同様
   # add_compile_options("/utf-8")
   # project()の後でないとコンパイラの判定ができない
   # target設定の前に実行する必要がある https://stackoverflow.com/questions/40516794/cmake-not-applying-compile-option-using-add-compile-options
endif()

if(POLICY CMP0074) # https://github.com/PointCloudLibrary/pcl/issues/2425
   cmake_policy(SET CMP0074 NEW)
endif()
# Find OpenMesh Package
set( VCGLIB_DIR $ENV{VCGLIB_DIR})
include_directories(${VCGLIB_DIR})
message(${VCGLIB_DIR})

set( EIGEN_ROOT $ENV{EIGEN_ROOT})
include_directories(${EIGEN_ROOT})
message(${EIGEN_ROOT})

set( MESHLAB_DIR $ENV{MESHLAB_DIR})
include_directories(${MESHLAB_DIR})
message(${MESHLAB_DIR})

# find_package( OpenMesh REQUIRED )
# if(OpenMesh_FOUND)
#    message("OpenMesh Found")
#    get_target_property(V OpenMeshCore INTERFACE_INCLUDE_DIRECTORIES) 
#    message(${V})
# else()
#    message("OpenMesh not Found")
# endif()

if(USE_QT_SYSTEM)
   message("---USE_QT_SYSTEM---")
endif()

if(USE_QT_SYSTEM)
   # set(Qt5_DIR $ENV{QTDIR})
   set(Qt5_DIR C:/Qt2/5.12.3/msvc2017_64)
   list(APPEND CMAKE_PREFIX_PATH ${Qt5_DIR})
   find_package(Qt5 COMPONENTS Widgets REQUIRED)

   # Set Automatic MOC
   set(CMAKE_AUTOMOC ON)
   set(CMAKE_AUTORCC ON)
   set(CMAKE_AUTOUIC ON)
   if(CMAKE_VERSION VERSION_LESS "3.7.0")
      set(CMAKE_INCLUDE_CURRENT_DIR ON)
   endif()
endif()

set(TARGET_LINK_LIBRARIES
   # ${VCGLIB_DIR}/lib/plylib.lib
   # ${VCGLIB_DIR}/lib/plylibd.lib
)
if(USE_QT_SYSTEM)
   set(TARGET_LINK_LIBRARIES
      ${TARGET_LINK_LIBRARIES}
      Qt5::Widgets
      Qt5::Core
   )
endif()

set(TARGET_LINK_LIBRARIES_DEBUG
      ${TARGET_LINK_LIBRARIES}
      ${VCGLIB_DIR}/lib/plylibd.lib
)
set(TARGET_LINK_LIBRARIES_RELEASE
      ${TARGET_LINK_LIBRARIES}
      ${VCGLIB_DIR}/lib/plylib.lib
)


# https://stackoverflow.com/questions/6594796/how-do-i-make-cmake-output-into-a-bin-dir
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../bin)

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

# Preprocessor Definitions
# add_definitions(-D_USE_MATH_DEFINES) # Core/System/compiler.hhで定義が無いとエラーになる
# add_definitions(-DOPENMESHDLL)
   # tutorial_009_IO_Optionsdでリンクエラーのため、対策
   # C:\Program Files\OpenMesh\include\OpenMesh\Core\System\OpenMeshDLLMacros.hh でこれを定義しないとlibでリンクしようとしてエラー出る
   # 本来はOpenMeshのCMakeLists.txtで定義されていてほしい
# add_compile_definitions(-D_USE_MATH_DEFINES) # Core/System/compiler.hhで定義が無いとエラーになる
# Additional Library Directories

# Additional Dependencies
add_definitions(-DMESHLAB_SCALAR=float)

