cmake_minimum_required( VERSION 3.6 )

# Create Project
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME) # https://stackoverflow.com/questions/12344368/automatically-use-the-directory-as-the-project-name-in-cmake
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId} CXX)
message(${ProjectId})

include(../Qt.cmake) # https://stackoverflow.com/questions/22804287/cmake-file-structure-how-to-split-a-cmakelists-txt-into-multiple-files
include(../meshlab.cmake)
include(../opt.cmake)
include(../vs.cmake)

set(SOURCE_FILES
   main.cpp
   widget.h
   widget.cpp
#   ${VCGLIB_DIR}/wrap/gui/trackball.h
#   ${VCGLIB_DIR}/wrap/gui/trackball.cpp
#   ${VCGLIB_DIR}/wrap/gui/trackmode.h
#   ${VCGLIB_DIR}/wrap/gui/trackmode.cpp
)

# set(TARGET_LINK_LIBRARIES_DEBUG
#       ${TARGET_LINK_LIBRARIES}
#       ${VCGLIB_DIR}/lib/plylibd.lib
# )
# set(TARGET_LINK_LIBRARIES_RELEASE
#       ${TARGET_LINK_LIBRARIES}
#       ${VCGLIB_DIR}/lib/plylib.lib
# )
# if(USE_QT_SYSTEM)
#    # QGLWidget Setting
#    find_package(Qt5 COMPONENTS OpenGL REQUIRED)
#    find_package(OpenGL)
#    set(TARGET_LINK_LIBRARIES
#       ${TARGET_LINK_LIBRARIES}
#       Qt5::OpenGL
#       ${OPENGL_LIBRARIES}
#       )
# endif()

# Add Executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}) # https://stackoverflow.com/questions/28893450/how-to-get-debug-postfix-in-executable-name/29023980

target_link_libraries(${PROJECT_NAME} 
   Qt5::Widgets
   Qt5::OpenGL
   ${OPENGL_LIBRARIES}
   debug ${VCGLIB_DIR}/lib/plylibd.lib
)
target_link_libraries(${PROJECT_NAME}
   Qt5::Widgets
   Qt5::OpenGL
   ${OPENGL_LIBRARIES}
   optimized ${VCGLIB_DIR}/lib/plylib.lib
)

# $<$<CONFIG:Debug>:         logger_debug.cpp>
# $<$<NOT:$<CONFIG:Debug>>:  logger_release.cpp>

# デバッグとリリースでply.cppのlibファイルのリンクを分岐処理しているが、OpenMeshみたいに自動で分ける方法に変更したい
# target_link_libraries(${PROJECT_NAME} debug ${TARGET_LINK_LIBRARIES_DEBUG}) # https://stackoverflow.com/questions/2209929/linking-different-libraries-for-debug-and-release-builds-in-cmake-on-windows
# target_link_libraries(${PROJECT_NAME} optimized ${TARGET_LINK_LIBRARIES_RELEASE})
