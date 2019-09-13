cmake_minimum_required(VERSION 3.0)

get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId} CXX)

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>") # Visual Studio用設定。ソースファイルの文字コードをUTF-8に変更

set( VCGLIB_DIR C:/Users/nitta/Documents/GitLab/vcglib) # ディレクトリのパスは各自の環境に合わせる
include_directories(${VCGLIB_DIR})
message(${VCGLIB_DIR})

set( EIGEN_ROOT C:/Users/nitta/Documents/GitLab/vcglib/eigenlib) # ディレクトリのパスは各自の環境に合わせる
include_directories(${EIGEN_ROOT})
message(${EIGEN_ROOT})

add_executable(${CMAKE_PROJECT_NAME} # exe name
	main.cpp	# 必要なcpp,hファイルを入れていく
)