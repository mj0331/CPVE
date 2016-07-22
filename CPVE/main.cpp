#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm.hpp>

#include <iostream>


#include "App.h"


int main() {
	
	CPVE::App app;
	
	try {
		app.Run();
	}
	catch (const std::runtime_error& e) {
		return CPVE::Log::FatalError(e);
	}

	return 0;
}