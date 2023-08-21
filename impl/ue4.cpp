#include "../include/libupgun.hpp"
#include <map>

upgun::ue4::FUObjectItem* upgun::ue4::TUObjectArray::GetObjectPtr(int32 Index)
{
	const uint32_t ChunkIndex = (uint32_t)Index / NumElementsPerChunk;
	const uint32_t WithinChunkIndex = (uint32_t)Index % NumElementsPerChunk;
	FUObjectItem* Chunk = Objects[ChunkIndex];
	return Chunk + WithinChunkIndex;
}

const std::wstring upgun::ue4::FString::ToString(void)
{
	//this is a shitty implementation
	if (!Data) return std::wstring();
	return std::wstring(Data);
}

upgun::ue4::TArray<upgun::ue4::FUpGunInventoryItem> upgun::ue4::UpGunInventorySubsystem::GetItems(void)
{
	upgun::ue4::TArray<upgun::ue4::FUpGunInventoryItem> result;

	static upgun::UObject Function = Game::GetSingleton().GetObjects().find(L"Function /Script/UpGun.UpGunInventorySubsystem.GetItems");

	upgun::UObject subsystem = Game::GetSingleton().get_upgun_inventory_subsystem();

	subsystem.ProcessEvent(Function, &result);

	return result;
}

upgun::ue4::FUpGunDeathmatchGameStateTags upgun::ue4::StringToGameStateTags(const std::wstring in)
{
	const std::map<FUpGunDeathmatchGameStateTags, const wchar_t*> EnumToString {
		{ FUpGunDeathmatchGameStateTags::WaitingPlayersToJoinTeam, L"GameState.Deathmatch.WaitingPlayersToJoinTeam" },
		{ FUpGunDeathmatchGameStateTags::PostWaitingPlayersToJoinTeam, L"GameState.Deathmatch.PostWaitingPlayersToJoinTeam" },
		{ FUpGunDeathmatchGameStateTags::TeamSelection, L"GameState.Deathmatch.TeamSelection" },
		{ FUpGunDeathmatchGameStateTags::PickingSkill, L"GameState.Deathmatch.PickingSkill" },
		{ FUpGunDeathmatchGameStateTags::InProgress, L"GameState.Deathmatch.InProgress" },
		{ FUpGunDeathmatchGameStateTags::RoundFinished, L"GameState.Deathmatch.RoundFinished" },
	};

	auto it = std::find_if(EnumToString.begin(), EnumToString.end(), [in](std::pair<FUpGunDeathmatchGameStateTags, const wchar_t*> item)
		{
			return item.second == in;
		});
	
	if (it != EnumToString.end())
	{
		return it->first;
	}

	return FUpGunDeathmatchGameStateTags::UNKNOWN;
}
