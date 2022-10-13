#include "Settings.h"

constexpr auto defaultSettingsPath = L"Data/MCM/Config/SprintTweaks/settings.ini";
constexpr auto mcmPath = L"Data/MCM/Settings/SprintTweaks.ini";

void Settings::ReadSettings()
{
	logger::info("Reading MCM .inis...");

	const auto readMCM = [&](std::filesystem::path path) {
		CSimpleIniA mcm;
		mcm.SetUnicode();
		mcm.LoadFile(path.string().c_str());

		//Sprint
		ReadFloatSetting(mcm, "Sprint", "iSprintDelay", SprintDelay);

		//Mounted Sprint
		ReadBoolSetting(mcm, "MountedSprint", "bAlternateMountedSprint", AlternateMountedSprint);
		ReadBoolSetting(mcm, "MountedSprint", "bMountedSprintBoost", MountedSprintBoost);
		ReadFloatSetting(mcm, "MountedSprint", "fMountedSprintBoostMagnitude", MountedSprintBoostMagnitude);
		ReadBoolSetting(mcm, "MountedSprint", "bMountedSprintBoostFXS", MountedSprintBoostFXS);
		ReadFloatSetting(mcm, "MountedSprint", "fMountedSprintBoostCost", MountedSprintBoostCost);
		ReadFloatSetting(mcm, "MountedSprint", "iMountedSprintBoostDuration", MountedSprintBoostDuration);
		ReadFloatSetting(mcm, "MountedSprint", "iMountedSprintBoostDelay", MountedSprintBoostDelay);
	};

	readMCM(defaultSettingsPath);  // read the default ini first
	readMCM(mcmPath);

	logger::info("...done");
}

void Settings::ReadIntSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, uint32_t& a_setting)
{
	const char* bFound = nullptr;
	bFound = a_ini.GetValue(a_sectionName, a_settingName);
	if (bFound) {
		a_setting = static_cast<int>(a_ini.GetDoubleValue(a_sectionName, a_settingName));
	}
}
void Settings::ReadFloatSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, float& a_setting)
{
	const char* bFound = nullptr;
	bFound = a_ini.GetValue(a_sectionName, a_settingName);
	if (bFound) {
		a_setting = static_cast<float>(a_ini.GetDoubleValue(a_sectionName, a_settingName));
	}
}

void Settings::ReadBoolSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, bool& a_setting)
{
	const char* bFound = nullptr;
	bFound = a_ini.GetValue(a_sectionName, a_settingName);
	if (bFound) {
		a_setting = a_ini.GetBoolValue(a_sectionName, a_settingName);
	}
}
