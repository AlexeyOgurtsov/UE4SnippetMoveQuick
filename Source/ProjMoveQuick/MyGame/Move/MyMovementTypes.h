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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config, Meta=(Bitmask, BitmaskEnum="EMyMoveComponentFlags"))
	uint8 Flags = static_cast<uint8>(EMyMoveComponentFlags::Default);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	bool bEnableCollision = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float Speed = 300.0F;
};
