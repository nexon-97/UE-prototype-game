#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Environment/WeatherPreset.h"
#include "WeatherController.generated.h"

UCLASS()
class TESTUEPROJECT_API AWeatherController
	: public AActor
{
	GENERATED_BODY()
	
public:	
	AWeatherController();

	UFUNCTION(BlueprintCallable)
	void RefreshPresetsActivation();

	UFUNCTION(BlueprintCallable)
	void SetActiveWeatherPreset(AWeatherPreset* Preset);

	UFUNCTION(BlueprintCallable)
	AWeatherPreset* GetActiveWeatherPreset() const;

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<AWeatherPreset*> WeatherPresetActors;

protected:
	void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Active Weather Preset", AllowPrivateAccess = "true"))
	AWeatherPreset* ActiveWeatherPreset = nullptr;
};
