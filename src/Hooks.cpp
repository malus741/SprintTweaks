#include "Hooks.h"
#include "Settings.h"
#include "SprintHandler.h"

namespace Hooks
{
	void SprintHandlerHook::ProcessButtonEx(RE::ButtonEvent* a_event, RE::PlayerControlsData* a_data)
	{
		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();

		if (Settings::AlternateMountedSprint && player->IsOnMount()) {
			SprintHandler::HandleAlternateMountSprint(a_event, player);
		} else {
			SprintHandler::HandlePlayerSprint(a_event, player);
		}
	}

	char ActorStateHook::RemoveMovementFlags(RE::ActorState* a_state, uint16_t a2)
	{
		char result = _RemoveMovementFlags(a_state, a2);

		if (Settings::MountedSprintBoost && !a_state->IsSprinting()) {
			auto actor = SKSE::stl::adjust_pointer<RE::Character>(a_state, -0x0B8);
			if (actor && actor->IsAMount()) {
				RE::ActiveEffect* mountSprintBoostActiveEffect = SprintHandler::GetMountSprintBoostActiveEffect(actor);
				if (mountSprintBoostActiveEffect)
					mountSprintBoostActiveEffect->Dispel(true);
			}
		}

		return result;
	}

	RE::BSEventNotifyControl MenuOpenCloseEventHandler::ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_dispatcher)
	{
		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();

		if (a_event && a_event->opening && a_event->menuName != "LootMenu" && SprintHandler::IsPlayerSprinting(player)) {
			// Menu was opened, stop sprinting
			SprintHandler::StopSprinting(player);
		}

		return RE::BSEventNotifyControl::kContinue;
	};
	MenuOpenCloseEventHandler g_menuOpenCloseEventHandler;

	//All of these came from the original Classic Sprinting Redone
	void SprintHandlerHook::Hook()
	{
		REL::Relocation<std::uintptr_t> ObjRefVtbl{ RE::VTABLE_SprintHandler[0] };
		_ProcessButtonFn = ObjRefVtbl.write_vfunc(0x4, &SprintHandlerHook::ProcessButtonEx);

		// Force sprint state to sync in every frame
		REL::safe_write(REL::RelocationID{ 39673, 40760 }.address() + REL::VariantOffset(0x15A, 0x159, 0).offset(), std::uint16_t(0x9090));

		// Skip HUD meter flashing when out of stamina - we handle it ourselves
		REL::safe_write(REL::RelocationID{ 41271, 42350 }.address() + REL::VariantOffset(0x30C, 0x350, 0).offset(), std::uint8_t(0xEB));

		RE::UI* ui = RE::UI::GetSingleton();
		ui->GetEventSource<RE::MenuOpenCloseEvent>()->AddEventSink(&g_menuOpenCloseEventHandler);
	}

	/*
		General PlayerCharacter update method, called every frame (?), delta is in-game time passed since last call.
		Replaced by the ActorStateHook hook for performance, kept here for future reference

	void PlayerCharacterHook::Update(RE::PlayerCharacter* player, float a_delta)
	{
		_Update(player, a_delta);
	}

	*/
}
