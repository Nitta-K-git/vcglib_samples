message("---visual studio setting---")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>") # 
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/w>") # warningレベルの変更 3,4だとnoteまで出るのでテンプレート展開で大量にログが出る。https://foonathan.net/blog/2018/10/17/cmake-warnings.html
