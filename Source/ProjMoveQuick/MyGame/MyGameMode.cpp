#include "MyGameMode.h"
#include "MyPlayerController.h"
#include "MyPawn.h"
#include "Util/Core/LogUtilLib.h"

AMyGameMode::AMyGameMode()
{
	DefaultPawnClass = AMyPawn::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
}
