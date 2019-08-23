message("---general setting---")
if(POLICY CMP0074) # https://github.com/PointCloudLibrary/pcl/issues/2425
   cmake_policy(SET CMP0074 NEW)
endif()
cmake_policy(SET CMP0076 NEW) # CMakeが自動的に相対パスを絶対パスへ変換する
# 実行ファイルの出力ディレクトリとpostfix設定 https://stackoverflow.com/questions/6594796/how-do-i-make-cmake-output-into-a-bin-dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../bin)

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()
