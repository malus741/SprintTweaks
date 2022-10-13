#include "Utils.h"

namespace Utils
{
	void Utils::FlashStaminaMeter()
	{
		FlashHudMenuMeter(26);
	}

	void Utils::FlashHudMenuMeter(std::uint32_t a_meter)
	{
		using func_t = decltype(&FlashHudMenuMeter);
		REL::Relocation<func_t> func(REL::RelocationID{ 51907, 52845 });
		return func(a_meter);
	}

	uint32_t Utils::GetTrampolineAllocSize(uint32_t size)
	{
		uint32_t nextPowerOf2 = pow(2, ceil(log(size) / log(2)));
		return nextPowerOf2;
	}
}
