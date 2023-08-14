#include "libupgun.hpp"

void upgun::UMaterialInstanceDynamic::SetTextureParameterValue(ue4::FName ParameterName, void* Value)
{
	upgun::ue4::UMaterialInstanceDynamic_SetTextureParameterValue_Params params;
	params.ParameterName = ParameterName;
	params.Value = (upgun::ue4::UObject*)Value;

	static UObject Function = Game::GetSingleton().GetObjects().find(L"Function /Script/Engine.MaterialInstanceDynamic.SetTextureParameterValue");

	this->ProcessEvent(Function, &params);
}
