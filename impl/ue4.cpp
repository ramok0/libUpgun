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
