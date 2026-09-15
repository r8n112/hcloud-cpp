include_guard(GLOBAL)

# Comma-separated sanitizer list: address,undefined / thread / off.
set(R8N112_SANITIZE
    "address,undefined"
    CACHE STRING "Sanitizers to enable (address,undefined | thread | off)"
)

# r8n112_enable_sanitizers(<target>)
function(r8n112_enable_sanitizers target)
    if(NOT R8N112_SANITIZE OR R8N112_SANITIZE STREQUAL "off")
        return()
    endif()

    if(MSVC)
        target_compile_options(${target} PRIVATE /fsanitize=address)
        return()
    endif()

    target_compile_options(${target} PRIVATE -fsanitize=${R8N112_SANITIZE} -fno-omit-frame-pointer -g)
    target_link_options(${target} PRIVATE -fsanitize=${R8N112_SANITIZE})
endfunction()
