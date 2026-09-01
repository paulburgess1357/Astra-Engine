astra_add_library(astra_gpu
    SOURCES
        context.cpp
        context.hpp
        instance.cpp
        instance.hpp
    DEPS astra::astra_core Vulkan::Vulkan GPUOpen::VulkanMemoryAllocator)
