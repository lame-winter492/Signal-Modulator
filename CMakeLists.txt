cmake_minimum_required(VERSION 3.21)
project(SignalModulator VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(SIGNAL_MODULATOR_BUILD_AE_PLUGIN "Build the After Effects plugin target" ON)
set(SIGNAL_MODULATOR_AE_SDK_ROOT "" CACHE PATH "Path to Adobe After Effects SDK root")

add_library(signal_modulator_core STATIC
    src/Modulation.cpp
    src/Filtering.cpp
    src/Distortion.cpp
    src/Channels.cpp
    src/Demodulation.cpp
)

target_include_directories(signal_modulator_core PUBLIC src)

if(SIGNAL_MODULATOR_BUILD_AE_PLUGIN)
    if(SIGNAL_MODULATOR_AE_SDK_ROOT STREQUAL "")
        message(WARNING "SIGNAL_MODULATOR_AE_SDK_ROOT is not set. AE plugin target will be skipped.")
    else()
        set(AE_INCLUDE_DIRS
            ${SIGNAL_MODULATOR_AE_SDK_ROOT}/Examples/Headers
            ${SIGNAL_MODULATOR_AE_SDK_ROOT}/Examples/Util
        )

        add_library(SignalModulator MODULE
            src/Plugin.cpp
            src/Params.cpp
            src/Render.cpp
        )

        target_include_directories(SignalModulator PRIVATE ${AE_INCLUDE_DIRS} src)
        target_link_libraries(SignalModulator PRIVATE signal_modulator_core)

        if(WIN32)
            target_compile_definitions(SignalModulator PRIVATE AE_OS_WIN)
            set_target_properties(SignalModulator PROPERTIES SUFFIX ".aex")
        elseif(APPLE)
            target_compile_definitions(SignalModulator PRIVATE AE_OS_MAC)
            set_target_properties(SignalModulator PROPERTIES BUNDLE TRUE SUFFIX ".plugin")
        else()
            message(WARNING "After Effects host is not available on this OS; plugin build is provided for cross-compilation setup only.")
        endif()
    endif()
endif()
