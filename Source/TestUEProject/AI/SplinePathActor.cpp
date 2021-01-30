#include "AI/SplinePathActor.h"

ASplinePathActor::ASplinePathActor(const FObjectInitializer& ObjectInitializer)
{
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("SplinePath"));
	Spline->bEditableWhenInherited = true;

	RootComponent = Spline;
}