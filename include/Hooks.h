#pragma once
#include "Utils.h"
namespace Hooks
{
	class SprintHandlerHook
	{
	public:
		static void Hook();
		void        ProcessButtonEx(RE::ButtonEvent* a_event, RE::PlayerControlsData* a_data);

	private:
		using ProcessButtonFn = decltype(&RE::SprintHandler::ProcessButton);
		inline static REL::Relocation<ProcessButtonFn> _ProcessButtonFn;
	};

	class ActorStateHook
	{
	public:
		static void Hook()
		{
			//called when actor stops sprinting
			auto     hook_addr = REL::RelocationID{ 36994, 38022 }.address() + 0x22A;
			uint32_t write_call_size = 14;
			SKSE::AllocTrampoline(Utils::GetTrampolineAllocSize(write_call_size));
			_RemoveMovementFlags = SKSE::GetTrampoline().write_call<5>(hook_addr, (uintptr_t)&RemoveMovementFlags);
		}

		static char RemoveMovementFlags(RE::ActorState* actorState, uint16_t a2);

	private:
		inline static REL::Relocation<decltype(&RemoveMovementFlags)> _RemoveMovementFlags;
	};

	class MenuOpenCloseEventHandler : public RE::BSTEventSink<RE::MenuOpenCloseEvent>
	{
	public:
		virtual RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_dispatcher);
	};

	/*
	* Hook for the general PlayerCharacter update method, replaced by the ActorStateHook hook.
	class PlayerCharacterHook
	{
	public:
		static void Hook()
		{
			REL::Relocation<std::uintptr_t> CharacterVtbl{ RE::VTABLE_PlayerCharacter[0] };
			_Update = CharacterVtbl.write_vfunc(0xAD, Update);
		}

	private:
		static void                                     Update(RE::PlayerCharacter* player, float a_delta);
		static inline REL::Relocation<decltype(Update)> _Update;
	};
	*/

}
