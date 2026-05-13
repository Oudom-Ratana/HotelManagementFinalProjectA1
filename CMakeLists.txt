cmake_minimum_required(VERSION 3.14)
project(HotelManagementApp)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# Fetch xlnt ( library used for working excel )
FetchContent_Declare(
  xlnt
  GIT_REPOSITORY https://github.com/tfussell/xlnt.git
  GIT_TAG v1.5.0
)
FetchContent_MakeAvailable(xlnt)

# Fetch tabulate
FetchContent_Declare(
  tabulate
  GIT_REPOSITORY https://github.com/p-ranav/tabulate.git
  GIT_TAG master
)
FetchContent_MakeAvailable(tabulate)

# Add your main.cpp or other source files
add_executable(HotelManagementApp 
    main.cpp
    src/Room.cpp
    src/ExcelUtil.cpp
    src/MenuUtils.cpp
    
    )

# Link libraries
target_link_libraries(HotelManagementApp PRIVATE xlnt tabulate)

add_custom_command(TARGET HotelManagementApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/build/_deps/xlnt-build/source/Release/xlnt.dll"
        $<TARGET_FILE_DIR:HotelManagementApp>)
