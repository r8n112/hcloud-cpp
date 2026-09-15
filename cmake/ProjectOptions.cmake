include_guard(GLOBAL)

include(CompilerWarnings)
include(Sanitizers)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE
        Debug
        CACHE STRING "Build type" FORCE
    )
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
