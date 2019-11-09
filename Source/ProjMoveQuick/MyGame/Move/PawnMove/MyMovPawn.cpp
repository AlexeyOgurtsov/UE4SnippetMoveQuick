#include "MyMovPawn.h"
#include "MyGame/MyPlayerController.h"
#include "MyGame/MyConfig.h"
#include "MyPawnMovementCustom.h"

#include "Util/Core/LogUtilLib.h"

#include "GameFramework/FloatingPawnMovement.h"

/**
* TODO Fix: Movement
* 1. Floating pawn movement: does it work by default (seems like not)?
*/

namespace
{
	namespace Config
	{
		constexpr float MAX_SPEED = 100.0F;
		constexpr float ACCELERATION = 0.0F;
		constexpr float DECELERATION = 0.0F;
	} // Config
} // anonymous

using MyMovPawnType = AMyMovPawn;

MyMovPawnType::AMyMovPawn()
{
	InitMovement();
}
void MyMovPawnType::MyBeginPlay_Implementation()
{
	M_LOGFUNC();

	Super::MyBeginPlay_Implementation();
	// Custom actions here
}

void MyMovPawnType::InitMovement()
{
	M_LOGFUNC();
	UMyPawnMovementCustom* const MyMovement = CreateDefaultSubobject<UMyPawnMovementCustom>(TEXT("PawnMovementCustom"));
	Movement = MyMovement;
}

MyPCType* MyMovPawnType::GetMyPC() const
{
	return Cast<MyPCType>(GetController());
}

MyPCType* MyMovPawnType::GetMyPCLogged(ELogFlags InLogFlags) const
{
	MyPCType* const PC = GetMyPC();
	if(PC == nullptr)
	{
		M_LOG_ERROR_IF_FLAGS(InLogFlags, TEXT("GetMyPC() returned NULL"));
	}
	return PC;
}

MyPCType* MyMovPawnType::GetMyPCChecked() const
{
	MyPCType* const PC = GetMyPC();
	checkf(PC, TEXT("GetMyMovPawn must return non-NULL pawn!"));
	return PC;
}
