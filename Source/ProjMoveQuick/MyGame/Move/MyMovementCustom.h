#pragma once

#include "GameFramework/MovementComponent.h"
#include "MyMovementCustom.generated.h"

UCLASS()
class UMyMovementCustom : public UMovementComponent
{
	GENERATED_BODY()

public:
	UMyMovementCustom();

	virtual void TickComponent(float DeltaTime, ELevelTick InLevelTick, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = Test)
	void InitMyVelocity(const FVector& InVelocity);

	/**
	* Velocity in WORLD space.
	*/
	UFUNCTION(BlueprintPure, Category = Test)
	FVector GetMyVelocity() const { return MyVelocity; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Test, Meta=(AllowPrivateAccess = true))
	FVector MyVelocity;
};
