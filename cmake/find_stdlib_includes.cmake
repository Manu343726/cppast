
# Original function here https://gitlab.com/Manu343726/tinyrefl/cmake/utils.cmake
# This one was modified to use the LLVM distro clang binary directly
function(clangxx_stdlib_includes stdlib INCLUDES)
    if(MSVC)
        if(NOT CLANG_BINARY)
            message(FATAL_ERROR "Clang binary not available")
        endif()

        set(compiler "${CLANG_BINARY}")
    else()
        set(compiler "${CMAKE_CXX_COMPILER}")
    endif()

    execute_process(
        COMMAND ${CMAKE_COMMAND} -E echo ""
        COMMAND ${compiler} -std=c++11 -v -x c++ -E -
        OUTPUT_VARIABLE out
        ERROR_VARIABLE err
    )

    message(STATUS "${err}")

    # Since GNU has localization in most of their tools (GCC included)
    # the diagnostics are translated to the different user languages.
    # We have to handle the different languages...
    #
    # The fun thing of this is that GNU translation to Spanish
    # seems to be done by a bad Google Translate bot or something

    set(locale_component_separator "__siplasplas_locale_component_separator__")

    set(include_search_locales
        "english
${locale_component_separator}
#include \"...\" search starts here:
#include <...> search starts here:
${locale_component_separator}
End of search list."

        "spanish
${locale_component_separator}
la búsqueda de #include \"...\" inicia aquí:
la búsqueda de #include <...> inicia aquí:
${locale_component_separator}
Fin de la lista de búsqueda"
    )

    foreach(locale ${include_search_locales})
        string(REGEX REPLACE "(.*)\n${locale_component_separator}\n.*\n${locale_component_separator}\n.*" "\\1" language "${locale}")
        string(REGEX REPLACE ".*\n${locale_component_separator}\n(.*)\n${locale_component_separator}\n.*" "\\1" header "${locale}")
        string(REGEX REPLACE ".*\n${locale_component_separator}\n.*\n${locale_component_separator}\n(.*)" "\\1" footer "${locale}")

        if(TINYREFL_VERBOSE_CONFIG)
            message(STATUS "Checking ${${compiler_exec}} output with ${language} language...")
            message(STATUS " - header: ${header}")
            message(STATUS " - footer: ${footer}")
        endif()

        string(REGEX MATCH "
${header}
(.+)
${footer}"
                includes "${err}\n STDOUT: ${out}")

        string(REGEX REPLACE "\n" ";" includes "${CMAKE_MATCH_1}")
        list(REMOVE_DUPLICATES includes)

        foreach(includedir ${includes})
            string(REGEX REPLACE "\ +$" "" includedir "${includedir}")
            string(REGEX REPLACE "\"(.*)\"" "\\1" includedir "${includedir}")
            string(REGEX REPLACE "\n" "" includedir "${includedir}")
            string(STRIP "${includedir}" includedir)

            message(STATUS "found clang include dir: ${includedir}")
            list(APPEND includedirs ${includedir})
        endforeach()
    endforeach()

    if(includedirs)
        list(REMOVE_DUPLICATES includedirs)
        set(${INCLUDES} "${includedirs}" PARENT_SCOPE)
        return()
    else()
        set(${INCLUDES} PARENT_SCOPE)
    endif()
endfunction()

function(add_clang_stdlib_includes TARGET)
    clangxx_stdlib_includes(stdlibc++ includes)

    set(variable CPPAST_CLANG_STDLIB_INCLUDES)
    set(string_literals)

    foreach(include ${includes})
        list(APPEND string_literals "\"${include}\"")
    endforeach()

    string(REGEX REPLACE ";" "," string_literals "${string_literals}")
    target_compile_definitions(${TARGET} PRIVATE ${variable}={${string_literals}})
endfunction()
