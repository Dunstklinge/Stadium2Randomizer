#pragma once

#include <random>

namespace Random {

	extern std::default_random_engine Generator;

	inline int GetInt(int min, int max) { std::uniform_int_distribution d(min, max); return d(Generator); }

	void Init();
};
