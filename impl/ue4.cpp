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

upgun::UClass* upgun::ue4::UStruct::StaticClass()
{
	static upgun::UClass staticClass = Game::GetSingleton().GetObjects().find([](upgun::UObject& object)
		{
			return object.get_full_name() == L"Class /Script/CoreUObject.Struct";
		}).Cast<upgun::UClass>();

		return &staticClass;
}

upgun::UClass* upgun::ue4::KismetRenderingLibrary::StaticClass()
{
	static upgun::UClass staticClass = Game::GetSingleton().GetObjects().find([](upgun::UObject& object)
		{
			return object.get_full_name() == L"Class /Script/Engine.KismetRenderingLibrary";
		}).Cast<upgun::UClass>();

		return &staticClass;
}

upgun::UClass* upgun::ue4::KismetStringLibrary::StaticClass()
{
	static upgun::UClass staticClass = Game::GetSingleton().GetObjects().find([](upgun::UObject& object)
		{
			return object.get_full_name() == L"Class /Script/Engine.KismetStringLibrary";
		}).Cast<upgun::UClass>();

		return &staticClass;
}

upgun::ue4::TArray<upgun::ue4::FUpGunInventoryItem> upgun::ue4::UpGunInventorySubsystem::GetItems(void)
{
	upgun::ue4::TArray<upgun::ue4::FUpGunInventoryItem> result;

	static upgun::UObject Function = Game::GetSingleton().GetObjects().find(L"Function /Script/UpGun.UpGunInventorySubsystem.GetItems");

	upgun::UObject subsystem = Game::GetSingleton().get_upgun_inventory_subsystem();

	subsystem.ProcessEvent(Function, &result);

	return result;
}

upgun::UClass* upgun::ue4::UpGunInventorySubsystem::StaticClass()
{
	static upgun::UClass staticClass = Game::GetSingleton().GetObjects()
		.find(L"Class /Script/UpGun.UpGunInventorySubsystem")
		.Cast<upgun::UClass>();

	return &staticClass;
}

upgun::UClass* upgun::ue4::KismetMaterialLibrary::StaticClass()
{
	static upgun::UClass staticClass = Game::GetSingleton().GetObjects()
		.find(L"Class /Script/Engine.KismetMaterialLibrary")
		.Cast<upgun::UClass>();

	return &staticClass;
}
