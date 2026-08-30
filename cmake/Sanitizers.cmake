option(ASTRA_ENABLE_ADDRESS_SANITIZER "Enable AddressSanitizer" OFF)
option(ASTRA_ENABLE_UNDEFINED_BEHAVIOR_SANITIZER "Enable UndefinedBehaviorSanitizer" OFF)
option(ASTRA_ENABLE_THREAD_SANITIZER "Enable ThreadSanitizer" OFF)

function(astra_enable_sanitizers target)
    if(ASTRA_ENABLE_ADDRESS_SANITIZER AND ASTRA_ENABLE_THREAD_SANITIZER)
        message(FATAL_ERROR "AddressSanitizer and ThreadSanitizer cannot be enabled together.")
    endif()

    if(MSVC)
        if(ASTRA_ENABLE_UNDEFINED_BEHAVIOR_SANITIZER OR ASTRA_ENABLE_THREAD_SANITIZER)
            message(FATAL_ERROR "MSVC supports AddressSanitizer only; use Clang or GCC for UBSan and TSan.")
        endif()

        if(ASTRA_ENABLE_ADDRESS_SANITIZER)
            # MSVC Debug defaults enable /RTC1 and incremental linking; both are
            # incompatible with AddressSanitizer.
            target_compile_options(${target} PRIVATE /fsanitize=address /RTC-)
            target_link_options(${target} PRIVATE /INCREMENTAL:NO)

            # The MSVC ASan runtime is DLL-only and lives next to cl.exe, which
            # is not on PATH outside a Developer Command Prompt. Copy the DLL
            # next to the executable; without it the loader's missing-DLL error
            # dialog blocks forever on headless CI runners.
            cmake_path(GET CMAKE_CXX_COMPILER PARENT_PATH astra_msvc_tools_dir)
            add_custom_command(TARGET ${target} POST_BUILD
                COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${astra_msvc_tools_dir}/clang_rt.asan_dynamic-x86_64.dll"
                    "$<TARGET_FILE_DIR:${target}>"
                VERBATIM)
        endif()
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
        set(astra_sanitizers)

        if(ASTRA_ENABLE_ADDRESS_SANITIZER)
            list(APPEND astra_sanitizers address)
        endif()

        if(ASTRA_ENABLE_UNDEFINED_BEHAVIOR_SANITIZER)
            list(APPEND astra_sanitizers undefined)
        endif()

        if(ASTRA_ENABLE_THREAD_SANITIZER)
            list(APPEND astra_sanitizers thread)
        endif()

        if(astra_sanitizers)
            list(JOIN astra_sanitizers "," astra_sanitizer_flags)
            target_compile_options(${target} PRIVATE
                "-fsanitize=${astra_sanitizer_flags}"
                -fno-omit-frame-pointer)
            target_link_options(${target} PRIVATE "-fsanitize=${astra_sanitizer_flags}")
        endif()

        if(ASTRA_ENABLE_UNDEFINED_BEHAVIOR_SANITIZER)
            # UBSan reports and continues by default; abort instead so CTest runs fail.
            target_compile_options(${target} PRIVATE -fno-sanitize-recover=undefined)
        endif()
    elseif(ASTRA_ENABLE_ADDRESS_SANITIZER OR ASTRA_ENABLE_UNDEFINED_BEHAVIOR_SANITIZER
           OR ASTRA_ENABLE_THREAD_SANITIZER)
        message(FATAL_ERROR "Sanitizers are not configured for ${CMAKE_CXX_COMPILER_ID}.")
    endif()
endfunction()
