DEBUG_MODE = -DDEBUG
CFLAGS = -std=c++17 -I$(VULKAN_SDK)/include -I$(STB_INCLUDE_PATH) \
		 -I$(TINYOBJ_INCLUDE_PATH) -O3 
LDFLAGS = -L$(VULKAN_SDK)/lib `pkg-config --static --libs glfw3` -lvulkan

VulkanTest: main.cpp
	./compile.sh
	clang++ $(CFLAGS) -o VulkanTest main.cpp $(LDFLAGS)

.PHONY: test clean

test: VulkanTest
	./VulkanTest

clean:
	rm -f VulkanTest
