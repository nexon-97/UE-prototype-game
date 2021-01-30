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
	for (AWeatherPreset* Preset : WeatherPresetActors)
	{
		if (nullptr != Preset)
		{
			Preset->Deactivate();
		}
	}

	if (nullptr != ActiveWeatherPreset)
	{
		ActiveWeatherPreset->Activate();
	}
}

void AWeatherController::SetActiveWeatherPreset(AWeatherPreset* Preset)
{
	if (Preset != ActiveWeatherPreset)
	{
		if (nullptr != ActiveWeatherPreset)
		{
			ActiveWeatherPreset->Deactivate();
		}

		ActiveWeatherPreset = Preset;

		if (nullptr != ActiveWeatherPreset)
		{
			ActiveWeatherPreset->Activate();
		}
	}
}

AWeatherPreset* AWeatherController::GetActiveWeatherPreset() const
{
	return ActiveWeatherPreset;
}

void AWeatherController::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	RefreshPresetsActivation();
}
