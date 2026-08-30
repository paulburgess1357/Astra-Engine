option(ASTRA_ENABLE_ADDRESS_SANITIZER "Enable AddressSanitizer" OFF)
option(ASTRA_ENABLE_UNDEFINED_BEHAVIOR_SANITIZER "Enable UndefinedBehaviorSanitizer" OFF)
option(ASTRA_ENABLE_THREAD_SANITIZER "Enable ThreadSanitizer" OFF)

if(MSVC AND ASTRA_ENABLE_ADDRESS_SANITIZER)
    # /RTC1 is part of CMake's default Debug flags but is incompatible with /fsanitize=address.
    string(REGEX REPLACE "/RTC[1csu]+" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
endif()

function(astra_enable_sanitizers target)
    if(ASTRA_ENABLE_ADDRESS_SANITIZER AND ASTRA_ENABLE_THREAD_SANITIZER)
        message(FATAL_ERROR "AddressSanitizer and ThreadSanitizer cannot be enabled together.")
    endif()

    if(MSVC)
        if(ASTRA_ENABLE_UNDEFINED_BEHAVIOR_SANITIZER OR ASTRA_ENABLE_THREAD_SANITIZER)
            message(FATAL_ERROR "MSVC supports AddressSanitizer only; use Clang or GCC for UBSan and TSan.")
        endif()

        if(ASTRA_ENABLE_ADDRESS_SANITIZER)
            target_compile_options(${target} PRIVATE /fsanitize=address)
            # Incremental linking is not supported with ASan instrumentation.
            target_link_options(${target} PRIVATE /INCREMENTAL:NO)
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
