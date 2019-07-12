#pragma once

#include "MyMovementTypes.generated.h"

UENUM(BlueprintType)
enum class EMyMoveComponentFlags : uint8
{
	None                = 0           UMETA(Hidden, DisplayName = "None"),

	UseSafeUpdate       = 1 << 0      UMETA(DisplayName = "UseSafeUpdate"),

	Default             = UseSafeUpdate        UMETA(DisplayName = "Default")
};
ENUM_CLASS_FLAGS(EMyMoveComponentFlags);

USTRUCT(BlueprintType)
struct FMyMoveComponentConfig
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = Config)
	EMyMoveComponentFlags Flags = EMyMoveComponentFlags::Default;
};
