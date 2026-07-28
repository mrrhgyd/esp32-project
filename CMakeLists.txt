cmake_minimum_required(VERSION 3.16.0)
# 核心补丁：加上判断，只有当系统存在 IDF_PATH 环境变量时，才去包含 project.cmake
if(DEFINED ENV{IDF_PATH})
    include($ENV{IDF_PATH}/tools/cmake/project.cmake)
    project(ESP_BASIC)
else()
    # 核心防御：在 Native 模拟器下，强制声明本项目不需要任何底层 C/C++ 硬件编译链
    # 这能直接阻止 VS Code 的 CMake 插件去瞎找 "nmake" 或 "Visual Studio"
    project(ESP_BASIC_SIM LANGUAGES NONE)
endif()