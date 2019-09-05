#include "Environment/WeatherController.h"

AWeatherController::AWeatherController()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AWeatherController::BeginPlay()
{
	Super::BeginPlay();
	
	RefreshPresetsActivation();
}

void AWeatherController::RefreshPresetsActivation()
{
	for (AActor* Preset : WeatherPresetActors)
	{
		if (nullptr != Preset)
		{
			Preset->GetRootComponent()->SetVisibility(false, true);
		}
	}

	if (nullptr != ActiveWeatherPreset)
	{
		ActiveWeatherPreset->GetRootComponent()->SetVisibility(true, true);
	}
}

void AWeatherController::SetActiveWeatherPreset(AActor* Preset)
{
	ActiveWeatherPreset = Preset;

	RefreshPresetsActivation();
}

AActor* AWeatherController::GetActiveWeatherPreset() const
{
	return ActiveWeatherPreset;
}
