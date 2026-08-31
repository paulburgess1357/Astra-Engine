astra_add_library(astra_gpu
    SOURCES
        context.cpp
        context.hpp
    DEPS astra::astra_core Vulkan::Vulkan GPUOpen::VulkanMemoryAllocator)
