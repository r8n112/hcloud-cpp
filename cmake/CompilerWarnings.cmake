include_guard(GLOBAL)

# r8n112_enable_warnings(<target>)
# Curated, strict warning set promoted to errors. Tune only with justification.
function(r8n112_enable_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /permissive- /WX /utf-8)
    else()
        target_compile_options(
            ${target}
            PRIVATE -Wall
                    -Wextra
                    -Wpedantic
                    -Werror
                    -Wshadow
                    -Wconversion
                    -Wsign-conversion
                    -Wcast-qual
                    -Wformat=2
                    -Wundef
                    -Wnull-dereference
                    -Wdouble-promotion
                    -Wnon-virtual-dtor
                    -Wold-style-cast
                    -Woverloaded-virtual
                    -Wimplicit-fallthrough
                    -Wswitch-enum
        )
    endif()
endfunction()
