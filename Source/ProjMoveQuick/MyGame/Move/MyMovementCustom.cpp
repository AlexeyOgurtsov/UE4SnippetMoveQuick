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

	M_TO_BE_IMPL(TEXT("Change velocity according to flags"));

	{
		checkf(UpdatedComponent, TEXT("UpdatedComponent DO must be set"));
		const bool bEnableCollision = false;
		const FVector Delta = DeltaTime * MyVelocity;

		// New rotation is in WORLD coord space passed to MoveUpdatedComponent
		const FQuat NewQuat = UpdatedComponent->GetComponentQuat();
	
		FHitResult Hit;
		MoveUpdatedComponent_ByFlags(Delta, NewQuat, bEnableCollision, Hit);
	}
}

void UMyMovementCustom::MoveUpdatedComponent_ByFlags(const FVector& InDelta, const FQuat& InNewRotation, bool bInSweep, FHitResult& OutHit, ETeleportType InTeleport)
{
	bool bUseSafeUpdate = (Config.Flags & EMyMoveComponentFlags::UseSafeUpdate) != EMyMoveComponentFlags::None;
	if(bUseSafeUpdate)
	{
		SafeMoveUpdatedComponent(InDelta, InNewRotation, bInSweep, OutHit, InTeleport);
	}
	else
	{
		MoveUpdatedComponent(InDelta, InNewRotation, bInSweep, &OutHit, InTeleport);
	}
}

void UMyMovementCustom::InitMyVelocity(const FVector& InVelocity)
{
	MyVelocity = InVelocity;
}
