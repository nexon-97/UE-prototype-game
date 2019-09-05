#include "Environment/WeatherPreset.h"

AWeatherPreset::AWeatherPreset()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AWeatherPreset::Activate()
{
	bIsActivated = true;
	RootComponent->SetVisibility(bIsActivated, true);

	OnActivate();
	OnActivateBP();
}

void AWeatherPreset::Deactivate()
{
	bIsActivated = false;
	RootComponent->SetVisibility(bIsActivated, true);

	OnDeactivate();
	OnDeactivateBP();
}

void AWeatherPreset::OnActivate()
{
	
}

void AWeatherPreset::OnDeactivate()
{
	
}
