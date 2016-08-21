VULKAN_SDK_PATH = ../VulkanSDK/x.x.x.x/x86_64

CFLAGS = -std=c++14 -I$(VULKAN_SDK_PATH)/include
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan

LD_LIBRARY_PATH = $(VULKAN_SDK_PATH)/lib
VK_LAYER_PATH = $(VULKAN_SDK_PATH)/etc/explicit_layer.d

BIN_FOLDER = ./CPVE_build

CPVE_build: *.cpp
	mkdir ./CPVE_build
	g++ $(CFLAGS) -o $(BIN_FOLDER)/CPVE *.cpp $(LDFLAGS)

.PHONY: build clean

build: CPVE_build
	./CPVE_build/CPVE

clean:
	rm -rf ./CPVE_build
