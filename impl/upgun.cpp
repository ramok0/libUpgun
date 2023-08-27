#include "../include/libupgun.hpp"
#include <algorithm>
#include <map>
#include <array>

std::vector<std::wstring> upgun::CosmeticSubsystem::get_default_cosmetics_names(void)
{
	std::vector<std::wstring> result;

	static upgun::UObject Function = Game::GetSingleton().GetObjects().find(L"Function /Script/UpGun.UpGunCosmeticSubsystem.GetDefaultCosmetics");

	ue4::TArray<ue4::FDataTableRowHandle> raw_data;
	ZeroMemory(&raw_data, sizeof(raw_data));

	Game::GetSingleton().get_upgun_cosmetic_subsystem().ProcessEvent(Function, &raw_data);

	result.reserve(raw_data.Count * sizeof(std::wstring));

	for (int i = 0; i < raw_data.Count; i++)
	{
		result.push_back(raw_data[i].RowName.ToString());
	}

	return result;
}

std::vector<upgun::UpGunNormalizedCosmetic> upgun::CosmeticSubsystem::get_cosmetics(void)
{
	struct MapToDataTable {
		std::wstring MapName;
		std::wstring DataTableInternalName;
		std::wstring DataTableName;
	};

	std::vector<upgun::UpGunNormalizedCosmetic> result;

	const std::vector<std::wstring> default_cosmetics_names = upgun::CosmeticSubsystem::get_default_cosmetics_names();

	ReflectedObject subsystem = Game::GetSingleton().get_upgun_cosmetic_subsystem().Cast<ReflectedObject>();

	const std::array<MapToDataTable, 4> data_sources = {
		MapToDataTable{L"EquipmentMap", L"DT_Hats", L"EquipmentsDT"},
		MapToDataTable{L"ArmorMap", L"DT_Armors", L"ArmorsDT"},
		MapToDataTable{L"ScreensMap", L"DT_Screens", L"ScreensDT"},
		MapToDataTable{L"WeaponSkinsMap", L"DTC_WeaponSkins", L"WeaponSkinsDT"},
	};

	ue4::TMap<ue4::FString, ue4::EUpGunCosmeticType> CosmeticTypeMap = subsystem.GetAs<ue4::TMap<ue4::FString, ue4::EUpGunCosmeticType>>(L"CosmeticTypeMap");

	for (auto& data_source : data_sources)
	{
		ue4::TMap<ue4::FString, ue4::FName> Map = subsystem.GetAs<ue4::TMap<ue4::FString, ue4::FName>>(data_source.MapName.c_str());

		result.reserve(sizeof(upgun::UpGunNormalizedCosmetic) * Map.Pairs.Count);

		for (int i = 0; i < Map.Pairs.Count; i++)
		{
			auto cosmetic_data = Map.Pairs[i]; //key = id, value = name

			upgun::UpGunNormalizedCosmetic cosmetic;

			cosmetic.Id = cosmetic_data.key.ToString();
			cosmetic.Name = cosmetic_data.value.ToString();
			cosmetic.InternalDTName = data_source.DataTableInternalName;
			cosmetic.Type = CosmeticTypeMap.Find(cosmetic_data.key);
			cosmetic.isDefaultCosmetic = std::find(default_cosmetics_names.begin(), default_cosmetics_names.end(), cosmetic.Name) != default_cosmetics_names.end();
			cosmetic.DataTable = subsystem.GetAs<void*>(data_source.DataTableName.c_str());

			result.push_back(cosmetic);
		}
	}

	return result;
}
