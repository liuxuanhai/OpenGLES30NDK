# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections -fvisibility=hidden")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections -fvisibility=hidden")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--gc-sections")

##########################################################
set(freetype2_path ${CMAKE_SOURCE_DIR}/../../../../freetype2)
set(freetype2_include ${freetype2_path}/include)
set(freetype2_a_path ${freetype2_path}/alibs/${ANDROID_ABI})
include_directories(${freetype2_include})

##########################################################
set(freetype2_a ${freetype2_a_path}/libfreetype.a)
add_library(freetype2lib STATIC IMPORTED)
set_target_properties(freetype2lib PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "C"
        IMPORTED_LINK_INTERFACE_LIBRARIES "z"
        IMPORTED_LOCATION ${freetype2_a}
        )

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

##########################################################
include_directories(./)

include_directories(./image)
aux_source_directory(./image image)

include_directories(./java)
aux_source_directory(./java java)

include_directories(./glm)

add_library( # Sets the name of the library.
        native-renderer

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${image}
        ${java}
        base_Log.cpp
        native-lib.cpp
        esUtils.cpp

#        example1.cpp
#        example2.cpp
#        example3.cpp
#        example4.cpp
#        example5.cpp
#        example6.cpp
#        example7.cpp
#        example8.cpp
#        example9.cpp
#        example10.cpp
        example11.cpp
        )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        native-renderer

        # Links the target library to the log library
        # included in the NDK.
        android
        jnigraphics
        ${log-lib}
        GLESv3
        #EGL
        freetype2lib
        )