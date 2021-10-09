#include "td_pch.h"
#include "GUID.h"

namespace Thunder
{
	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_Engine(s_RandomDevice());
	static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

	GUID GUID::Generate()
	{
		return s_UniformDistribution(s_Engine);
	}
}