#pragma once

/**
* To be copied to the destination and changed.
*/

#include "Util/TestUtil/TUVisibleActor.h"
#include "MyActor.generated.h"

class UMovementComponent;

UCLASS()
class AMyActor : public ATUVisibleActor
{
	GENERATED_BODY()

public:
	AMyActor();

	virtual void BeginPlay() override;

private:
	void InitMovement();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Motion, Meta=(AllowPrivateAccess = true))
	UMovementComponent* Movement = nullptr;
};
