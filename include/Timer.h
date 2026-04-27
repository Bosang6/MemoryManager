#pragma once
#include <chrono>
#include <string>
#include <iostream>

class Timer
{
public:
	Timer(const std::string& allocateType)
		: allocateType(allocateType)
		, start(std::chrono::high_resolution_clock::now())
	{
	}
	~Timer()
	{
		auto end = std::chrono::high_resolution_clock::now();

		auto duration =
			std::chrono::duration_cast<std::chrono::microseconds>(
				end - start
			).count();

		std::cout << allocateType << ": " << duration / 1000.0 << "ms" << std::endl;
	}

private:
	std::string allocateType;
	std::chrono::high_resolution_clock::time_point start;
};