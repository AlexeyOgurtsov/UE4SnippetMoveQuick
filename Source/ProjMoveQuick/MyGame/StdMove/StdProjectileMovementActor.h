#pragma once

#include "GameFramework/Actor.h"
#include "StdProjectileMovementActor.generated.h"

class UStaticMeshComponent;
class UProjectileMovementComponent;

UCLASS(BlueprintType)
class AStdProjectileMovementActor : public AActor
{
	GENERATED_BODY()

public:
	AStdProjectileMovementActor();

	virtual void BeginPlay() override;

	/** GetMesh*/
	UFUNCTION(BlueprintPure, Category=Mesh)
	UStaticMeshComponent* GetMesh() const
	{
		return Mesh;
	}
	
	/** GetMovement*/
	UFUNCTION(BlueprintPure, Category=Movement)
	UProjectileMovementComponent* GetMovement() const
	{
		return Movement;
	}
	
protected:
	void InitMesh();
	void InitMovementComponent();
	void FireInDirection();

	/** Mesh*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta=(AllowPrivateAccess=true), Category=Mesh)
	UStaticMeshComponent* Mesh = nullptr;

	/** Movement*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta=(AllowPrivateAccess=true), Category=Movement)
	UProjectileMovementComponent* Movement = nullptr;
	
	
};
