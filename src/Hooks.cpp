#include "Hooks.h"
#include "Settings.h"
#include "SprintHandler.h"

namespace Hooks
{
	void SprintHandlerHook::ProcessButtonEx(RE::SprintHandler* a_this, RE::ButtonEvent* a_event, RE::PlayerControlsData* a_data)
	{
		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();

		if (Settings::AlternateMountedSprint && player->IsOnMount()) {
			_ProcessButtonFn(a_this, a_event, a_data);
		} else {
			SprintHandler::HandlePlayerSprint(a_event, player);
		}
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
}
