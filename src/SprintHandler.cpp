#include "SprintHandler.h"
#include "Data.h"
#include "Settings.h"
#include "Utils.h"

namespace SprintHandler
{
	using FlagBDD = RE::PlayerCharacter::FlagBDD;

	void SprintHandler::StartSprinting(RE::PlayerCharacter* player)
	{
		player->GetPlayerRuntimeData().unkBDD |= FlagBDD::kSprinting;
	}

	void SprintHandler::StopSprinting(RE::PlayerCharacter* player)
	{
		player->GetPlayerRuntimeData().unkBDD &= static_cast<FlagBDD>(~std::underlying_type_t<FlagBDD>(FlagBDD::kSprinting));
	}

	bool SprintHandler::IsPlayerSprinting(RE::PlayerCharacter* player)
	{
		return (player->GetPlayerRuntimeData().unkBDD & FlagBDD::kSprinting) != FlagBDD::kNone;
	}

	void SprintHandler::HandlePlayerSprint(RE::ButtonEvent* a_event, RE::PlayerCharacter* player)
	{
		float sprintDelaySeconds = Settings::SprintDelay / 1000;
		float currentStamina = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kStamina);
		bool  isPlayerSprinting = IsPlayerSprinting(player);

		if (a_event->IsDown()) {
			if (currentStamina <= 0.0f)
				return Utils::FlashStaminaMeter();

			if (sprintDelaySeconds > 0.0f)
				return;

			if (!isPlayerSprinting)
				StartSprinting(player);

		} else if (a_event->IsHeld()) {
			if (a_event->HeldDuration() < sprintDelaySeconds)
				return;

			if (currentStamina > 0.0f && !isPlayerSprinting)
				return StartSprinting(player);

			float mountSprintDelaySeconds = Settings::MountedSprintBoostDelay / 1000;
			if (Settings::MountedSprintBoost && player->IsOnMount() && a_event->HeldDuration() >= mountSprintDelaySeconds + sprintDelaySeconds) {
				RE::NiPointer<RE::Actor> currentMount;
				if (player->GetMount(currentMount)) {
					CastMountSprintBoostSpell(currentMount.get());
				}
			}

		} else if (a_event->IsUp() && isPlayerSprinting) {
			StopSprinting(player);
		}
	}

	bool wasPlayerSprinting;
	void SprintHandler::HandleAlternateMountSprint(RE::ButtonEvent* a_event, RE::PlayerCharacter* player)
	{
		RE::NiPointer<RE::Actor> currentMount;
		bool                     isPlayerSprinting = IsPlayerSprinting(player);
		float                    mountSprintDelaySeconds = Settings::MountedSprintBoostDelay / 1000;

		if (player->GetMount(currentMount)) {
			if (a_event->IsDown()) {
				wasPlayerSprinting = isPlayerSprinting;

				if (!isPlayerSprinting)
					return StartSprinting(player);

				if (!Settings::MountedSprintBoost)
					return StopSprinting(player);

				if (!Settings::MountedSprintBoostHold) {
					if (currentMount->AsActorValueOwner()->GetActorValue(RE::ActorValue::kStamina) < Settings::MountedSprintBoostCost)
						return Utils::FlashStaminaMeter();

					CastMountSprintBoostSpell(currentMount.get());
					currentMount->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, RE::ActorValue::kStamina, -Settings::MountedSprintBoostCost);
				}
			} else if (a_event->IsHeld() && Settings::MountedSprintBoostHold && isPlayerSprinting && a_event->HeldDuration() >= mountSprintDelaySeconds) {
				wasPlayerSprinting = isPlayerSprinting;
				CastMountSprintBoostSpell(currentMount.get());
			} else if (a_event->IsUp() && Settings::MountedSprintBoostHold && wasPlayerSprinting) {
				if (a_event->HeldDuration() <= 0.2f) {
					StopSprinting(player);
				} else if (a_event->HeldDuration() >= mountSprintDelaySeconds) {
					SprintHandler::DispelMountSprintBoostSpell(currentMount.get());
				}
			}
		}
	}

	void SprintHandler::CastMountSprintBoostSpell(RE::Actor* mount)
	{
		RE::ActiveEffect* mountSprintBoostActiveEffect = GetMountSprintBoostActiveEffect(mount);

		if (mountSprintBoostActiveEffect) {
			mountSprintBoostActiveEffect->elapsedSeconds = 0;
		} else {
			float currentSpeedMult = mount->AsActorValueOwner()->GetActorValue(RE::ActorValue::kSpeedMult);

			mount->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
				->CastSpellImmediate(Data::MountSprintBoostSpell,
					false, mount, 1, false,
					currentSpeedMult * Settings::MountedSprintBoostMagnitude / 100,
					mount);
		}
	}

	void SprintHandler::DispelMountSprintBoostSpell(RE::Actor* mount)
	{
		RE::ActiveEffect* mountSprintBoostActiveEffect = SprintHandler::GetMountSprintBoostActiveEffect(mount);
		if (mountSprintBoostActiveEffect)
			mountSprintBoostActiveEffect->Dispel(true);
	}

	RE::ActiveEffect* SprintHandler::GetMountSprintBoostActiveEffect(RE::Actor* mount)
	{
		RE::BSSimpleList<RE::ActiveEffect*>* activeEffects = mount->AsMagicTarget()->GetActiveEffectList();
		if (activeEffects) {
			for (RE::ActiveEffect* activeEffect : *activeEffects) {
				if (activeEffect->effect == Data::MountSprintBoostSpell->effects[0])
					return activeEffect;
			}
		}
		return nullptr;
	}

}
