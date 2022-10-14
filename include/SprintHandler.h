#pragma once

namespace SprintHandler
{
	void              StartSprinting(RE::PlayerCharacter* player);
	void              StopSprinting(RE::PlayerCharacter* player);
	bool              IsPlayerSprinting(RE::PlayerCharacter* player);
	void              HandlePlayerSprint(RE::ButtonEvent* a_event, RE::PlayerCharacter* player);
	void              HandleAlternateMountSprint(RE::ButtonEvent* a_event, RE::PlayerCharacter* player);
	void              CastMountSprintBoostSpell(RE::Actor* mount);
	void              DispelMountSprintBoostSpell(RE::Actor* actor);
	void              UpdateMountedSprintBoostHoldCostMultiplier(bool isApply = true);
	RE::ActiveEffect* GetMountSprintBoostActiveEffect(RE::Actor* mount);
}
