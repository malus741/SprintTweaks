#include "Data.h"
#include "Settings.h"

void Data::LoadData()
{
	logger::info("Loading perks and spells...");

	if (!Data::MountSprintBoostSpell) {
		DataLoader::FormLoader loader("SprintTweaks.esp");
		loader.load(Data::MountSprintBoostSpell, 0x0D67);
	}

	if (!OriginalfSprintStaminaDrainMultValue && !fSprintStaminaDrainMult) {
		auto gameSettingCollection = RE::GameSettingCollection::GetSingleton();
		if (gameSettingCollection) {
			fSprintStaminaDrainMult = gameSettingCollection->GetSetting("fSprintStaminaDrainMult");
			if (fSprintStaminaDrainMult) {
				OriginalfSprintStaminaDrainMultValue = fSprintStaminaDrainMult->GetFloat();
			}
		}
	}

	if (Data::MountSprintBoostSpell && Data::MountSprintBoostSpell->effects.size() > 0) {
		RE::Effect* mountSprintBoostEffect = Data::MountSprintBoostSpell->effects[0];
		mountSprintBoostEffect->effectItem.duration = static_cast<std::uint32_t>(Settings::MountedSprintBoostDuration);

		RE::EffectShaderData& effectShaderData = mountSprintBoostEffect->baseEffect->data.effectShader->data;
		if (!(originalColorKey1ColorAlpha && originalColorKey2ColorAlpha && originalColorKey3ColorAlpha)) {
			originalColorKey1ColorAlpha = effectShaderData.colorKey1ColorAlpha;
			originalColorKey2ColorAlpha = effectShaderData.colorKey2ColorAlpha;
			originalColorKey3ColorAlpha = effectShaderData.colorKey3ColorAlpha;
		}

		if (!Settings::MountedSprintBoostFXS) {
			effectShaderData.colorKey1ColorAlpha = 0;
			effectShaderData.colorKey2ColorAlpha = 0;
			effectShaderData.colorKey3ColorAlpha = 0;
		} else {
			effectShaderData.colorKey1ColorAlpha = *originalColorKey1ColorAlpha;
			effectShaderData.colorKey2ColorAlpha = *originalColorKey2ColorAlpha;
			effectShaderData.colorKey3ColorAlpha = *originalColorKey3ColorAlpha;
		}

	} else {
		Settings::MountedSprintBoost = false;
	}

	logger::info("...done");
}
