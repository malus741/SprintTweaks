#include "SprintHandler.h"
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

		} else if (a_event->IsUp() && isPlayerSprinting) {
			StopSprinting(player);
		}
	}
}
