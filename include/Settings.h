#pragma once

class Settings
{
public:
	static inline float SprintDelay = 400.0f;
	static inline bool  AlternateMountedSprint = true;
	static inline bool  MountedSprintBoost = true;
	static inline float MountedSprintBoostMagnitude = 50.0f;
	static inline bool  MountedSprintBoostFXS = true;
	static inline float MountedSprintBoostCost = 30.0f;
	static inline float MountedSprintBoostDuration = 5.0f;
	static inline float MountedSprintBoostDelay = 3000.0f;
	static void         ReadSettings();

private:
	static void ReadBoolSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, bool& a_setting);
	static void ReadFloatSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, float& a_setting);
	static void ReadIntSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, uint32_t& a_setting);
};
