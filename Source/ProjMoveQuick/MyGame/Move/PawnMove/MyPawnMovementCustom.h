#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "MyGame/Move/MyMovementTypes.h"
#include "MyPawnMovementCustom.generated.h"

UCLASS(BlueprintType, ClassGroup=(Custom), Meta=(BlueprintSpawnableComponent))
class UMyPawnMovementCustom : public UPawnMovementComponent
{
	GENERATED_BODY()

public:
	UMyPawnMovementCustom();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void ApplyControlInputToVelocity(float DeltaTime);
	
	/** GetConfig*/
	UFUNCTION(BlueprintPure, Category=Config)
	FMyMoveComponentConfig GetConfig() const
	{
		return Config;
	}

private:	
	void MoveUpdatedComponent_ByFlags(const FVector& InDelta, const FQuat& InNewRotation, bool bInSweep, FHitResult& OutHit, ETeleportType InTeleport = ETeleportType::None);

	/** MoveConfig*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta=(AllowPrivateAccess=true), Category=Config)
	FMyMoveComponentConfig Config;
};
