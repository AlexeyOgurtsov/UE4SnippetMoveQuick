#include "MyActor.h"
#include "Move/MyMovementCustom.h"
#include "Util/Core/LogUtilLib.h"

AMyActor::AMyActor()
{
	InitMovement();
}

void AMyActor::BeginPlay()
{
	M_LOGFUNC();
	Super::BeginPlay();
}

void AMyActor::InitMovement()
{
	M_LOGFUNC();
	Movement = CreateDefaultSubobject<UMyMovementCustom>(TEXT("Movement"));
}
