#pragma once

class Data
{
public:
	static inline RE::SpellItem* MountSprintBoostSpell;
	static void                  LoadData();

private:
	static inline std::optional<float> originalColorKey1ColorAlpha;
	static inline std::optional<float> originalColorKey2ColorAlpha;
	static inline std::optional<float> originalColorKey3ColorAlpha;
};

namespace DataLoader
{
	class FormLoader
	{
	private:
		RE::BSFixedString   _pluginName;
		RE::TESDataHandler* _dataHandler;

	public:
		FormLoader(RE::BSFixedString pluginName)
		{
			_pluginName = pluginName;
			_dataHandler = RE::TESDataHandler::GetSingleton();
			if (!_dataHandler) {
				logger::critical("Error: TESDataHandler not found.");
			}
			if (!_dataHandler->LookupModByName(pluginName)) {
				logger::critical("Error: {} not found.", pluginName);
			}
			logger::info("Loading from plugin {}...", pluginName);
		}

		template <class FormType>
		void load(FormType*& formRet, RE::FormID formID)
		{
			formRet = _dataHandler->LookupForm<FormType>(formID, _pluginName);
			if (!formRet) {
				logger::critical("Error: null formID or wrong form type when loading {} from {}", formID, _pluginName);
			}
		}
	};
}
