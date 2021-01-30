#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "SplinePathActor.generated.h"

UCLASS()
class TESTUEPROJECT_API ASplinePathActor
    : public AActor
{
	GENERATED_BODY()

public:
	ASplinePathActor(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere)
	USplineComponent* Spline;
};
