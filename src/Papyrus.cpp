#include "Papyrus.h"
#include "Data.h"
#include "Hooks.h"
#include "Settings.h"

namespace Papyrus
{
	void SprintTweaks_MCM::OnConfigClose(RE::TESQuest*)
	{
		Settings::ReadSettings();
		Data::LoadData();
	}

	bool SprintTweaks_MCM::Register(RE::BSScript::IVirtualMachine* a_vm)
	{
		a_vm->RegisterFunction("OnConfigClose", "SprintTweaks_MCM", OnConfigClose);

		logger::info("Registered SprintTweaks_MCM class");
		return true;
	}

	void Register()
	{
		auto papyrus = SKSE::GetPapyrusInterface();
		papyrus->Register(SprintTweaks_MCM::Register);
		logger::info("Registered papyrus functions");
	}
}
