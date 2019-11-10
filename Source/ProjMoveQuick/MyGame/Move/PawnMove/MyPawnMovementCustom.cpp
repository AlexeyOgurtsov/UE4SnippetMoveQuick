#include "MyPawnMovementCustom.h"
#include "Util/Core/Phys/PhysUtilLib.h"
#include "Util/Core/LogUtilLib.h"

namespace MyPawnMovementCustom
{
	namespace Config
	{
	} // Config
} // MyPawnMovementCustom

using namespace MyPawnMovementCustom;

UMyPawnMovementCustom::UMyPawnMovementCustom()
{
}

void UMyPawnMovementCustom::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if(ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!PawnOwner || !UpdatedComponent)
	{
		return;
	}

	AController* const Controller = PawnOwner->GetController();
	if(!Controller)
	{
		return;
	}

	ApplyControlInputToVelocity(DeltaTime);

	// MoveActor
	FVector const Delta = Velocity * DeltaTime;
	FQuat const NewQuat = UpdatedComponent->GetComponentQuat();
	if( ! Delta.IsNearlyZero(1e-6f) )
	{
		FHitResult Hit;
		MoveUpdatedComponent_ByFlags(Delta, NewQuat, Config.bEnableCollision, /**Out**/Hit);
		if(Hit.bBlockingHit)
		{
			HandleImpact(Hit, Hit.Time, Delta);
		}
	}

	// Finalize
	UpdateComponentVelocity();
}

void UMyPawnMovementCustom::ApplyControlInputToVelocity(float DeltaTime)
{
	FVector const ControlAcceleration = GetPendingInputVector();

	// @TODO
	Velocity = Velocity + (DeltaTime * ControlAcceleration * Config.Speed);

	ConsumeInputVector();
}

void UMyPawnMovementCustom::HandleImpact(const FHitResult& Hit, float TimeSlice, const FVector& MoveDelta)
{
	// WARNING! Super::HandleImpact is empty (at the time of writing this code), but who knows...
	Super::HandleImpact(Hit, TimeSlice, MoveDelta);	

	Velocity = FVector::ZeroVector;
	UpdateComponentVelocity();
}

void UMyPawnMovementCustom::MoveUpdatedComponent_ByFlags(const FVector& InDelta, const FQuat& InNewRotation, bool bInSweep, FHitResult& OutHit, ETeleportType InTeleport)
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
