#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeatherPreset.generated.h"

UCLASS()
class TESTUEPROJECT_API AWeatherPreset
	: public AActor
{
	GENERATED_BODY()
	
public:	
	AWeatherPreset();

	void Activate();
	void Deactivate();

	virtual void OnActivate();
	virtual void OnDeactivate();

	UFUNCTION(BlueprintImplementableEvent)
	void OnActivateBP();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDeactivateBP();

private:
	bool bIsActivated = false;
};
