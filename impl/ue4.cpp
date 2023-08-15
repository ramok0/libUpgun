#include "../include/libupgun.hpp"

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