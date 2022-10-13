#pragma once
#include "Utils.h"
namespace Hooks
{
	class SprintHandlerHook
	{
	public:
		static void Hook();

	private:
		static void ProcessButtonEx(RE::SprintHandler* a_this, RE::ButtonEvent* a_event, RE::PlayerControlsData* a_data);
		static inline REL::Relocation<decltype(ProcessButtonEx)> _ProcessButtonFn;
	};

	class MenuOpenCloseEventHandler : public RE::BSTEventSink<RE::MenuOpenCloseEvent>
	{
	public:
		virtual RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_dispatcher);
	};
}
