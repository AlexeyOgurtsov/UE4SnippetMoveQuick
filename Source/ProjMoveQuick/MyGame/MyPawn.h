#pragma once

/**
* To be copied to the destination and changed.
*/

#include "Util/TestUtil/TUPawn.h"
#include "Util/Core/Log/MyLoggingTypes.h"
#include "MyPawn.generated.h"

UCLASS()
class AMyPawn : public ATUPawn
{
	GENERATED_BODY()

	// ~Framework Begin
	virtual void MyBeginPlay_Implementation() override;
	// ~Framework End

	// ~Access helpers Begin
	UFUNCTION(BlueprintPure, Category = Controller)
	AMyPlayerController* GetMyPC() const;

	UFUNCTION(BlueprintPure, Category = Controller)
	AMyPlayerController* GetMyPCLogged(ELogFlags InLogFlags = ELogFlags::None) const;

	/**
	* Checks that the given pawn NOT null.
	*/
	UFUNCTION(BlueprintPure, Category = Controller)
	AMyPlayerController* GetMyPCChecked() const;
	// ~Access helpers End

public:
	AMyPawn();

private:
	// ~Motion Begin
	void InitMovement();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Motion, Meta=(AllowPrivateAccess = true))
	UPawnMovementComponent *Movement = nullptr;
	// ~Motion End
};
