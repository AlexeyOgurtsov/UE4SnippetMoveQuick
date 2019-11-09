#include "MyMovementCustom.h"
#include "Util/Core/Phys/PhysUtilLib.h"
#include "Util/Core/LogUtilLib.h"

/**
* TODO Editable in blueprint:
* 1) Make that it's possible to edit the component inside the Blueprint!
* 2) Make EMyMoveComponentFlags selectable
*
* TODO Collision Test: 
* 1) Sweep test
*/

UMyMovementCustom::UMyMovementCustom() 
: MyVelocity { FVector::ForwardVector }
{
	bEditableWhenInherited = true; // Must be true by default (in ActorComponent !), however unable to edit in UE4 blueprint by default!
}
void UMyMovementCustom::TickComponent(float DeltaTime, ELevelTick InLevelTick, FActorComponentTickFunction* ThisTickFunction)
{
	if( ShouldSkipUpdate(DeltaTime) )
	{
		return;
	}

	Super::TickComponent(DeltaTime, InLevelTick, ThisTickFunction);

	{
		checkf(UpdatedComponent, TEXT("UpdatedComponent DO must be set"));
		const FVector Delta = DeltaTime * MyVelocity;

		// New rotation is in WORLD coord space passed to MoveUpdatedComponent
		const FQuat NewQuat = UpdatedComponent->GetComponentQuat();
	
		FHitResult Hit;
		MoveUpdatedComponent_ByFlags(Delta, NewQuat, Config.bEnableCollision, Hit);
	}
}

void UMyMovementCustom::MoveUpdatedComponent_ByFlags(const FVector& InDelta, const FQuat& InNewRotation, bool bInSweep, FHitResult& OutHit, ETeleportType InTeleport)
{
	bool const bUseSafeUpdate = (Config.Flags & static_cast<uint8>(EMyMoveComponentFlags::UseSafeUpdate)) != static_cast<uint8>(EMyMoveComponentFlags::None);
	if(bUseSafeUpdate)
	{
		SafeMoveUpdatedComponent(InDelta, InNewRotation, bInSweep, OutHit, InTeleport);
	}
	else
	{
		MoveUpdatedComponent(InDelta, InNewRotation, bInSweep, &OutHit, InTeleport);
	}
	UPhysUtilLib::LogHitResult(OutHit);
}

void UMyMovementCustom::InitMyVelocity(const FVector& InVelocity)
{
	MyVelocity = InVelocity;
}
