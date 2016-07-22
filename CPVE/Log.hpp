#pragma once

#include <iostream>
#include <stdexcept>
#include <functional>

namespace CPVE {
	class Log {
	public:
		static int FatalError(const std::runtime_error& e)
		{
			std::cerr << e.what() << "\n";
			return EXIT_FAILURE;
		}

		template <typename T>
		static void Message(T msg)
		{
			std::cout << msg << "\n";
		}
	};
}