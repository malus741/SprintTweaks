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
}
