#include "MyMovementCustom.h"
#include "Util/Core/LogUtilLib.h"

UMyMovementCustom::UMyMovementCustom() 
: MyVelocity { FVector::ForwardVector }
{
}
void UMyMovementCustom::TickComponent(float DeltaTime, ELevelTick InLevelTick, FActorComponentTickFunction* ThisTickFunction)
{
	if( ShouldSkipUpdate(DeltaTime) )
	{
		return;
	}

	Super::TickComponent(DeltaTime, InLevelTick, ThisTickFunction);

	M_TO_BE_IMPL(TEXT("Update according to my velocity"));
}

void UMyMovementCustom::InitMyVelocity(const FVector& InVelocity)
{
	MyVelocity = InVelocity;
}