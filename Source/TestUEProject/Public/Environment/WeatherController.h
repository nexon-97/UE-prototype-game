#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	void SetActiveWeatherPreset(AActor* Preset);

	UFUNCTION(BlueprintCallable)
	AActor* GetActiveWeatherPreset() const;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<AActor*> WeatherPresetActors;

protected:
	void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Active Weather Preset", AllowPrivateAccess = "true"))
	AActor* ActiveWeatherPreset = nullptr;
};
