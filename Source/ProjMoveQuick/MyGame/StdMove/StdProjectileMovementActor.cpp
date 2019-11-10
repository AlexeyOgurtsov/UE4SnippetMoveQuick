#include "StdProjectileMovementActor.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Util/Core/LogUtilLib.h"

#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

#include "Util/TestUtil/VisibleActorConfig.h"

namespace StdProjectileMovementActorImpl
{
	namespace Config
	{
		constexpr float SPEED = 1000;
		constexpr float MAX_SPEED = SPEED;
	} // Config
} // StdProjectileMovementActorImpl
using namespace StdProjectileMovementActorImpl;

AStdProjectileMovementActor::AStdProjectileMovementActor()
{
	InitMesh();
	InitMovementComponent();
}

void AStdProjectileMovementActor::InitMesh()
{
	M_LOGFUNC();

	static ConstructorHelpers::FObjectFinderOptional<UStaticMesh> MeshFinder { VisibleActorConfig::Default::MESH_ASSET_PATH };
	M_LOG_ERROR_IF( ! MeshFinder.Succeeded(), TEXT("Default mesh (\"%s\") NOT found"), VisibleActorConfig::Default::MESH_ASSET_PATH);

	{
		Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

		if(MeshFinder.Succeeded())
		{
			M_LOG(TEXT("Default mesh (\"%s\") found, setting it up"), VisibleActorConfig::Default::MESH_ASSET_PATH);
			Mesh->SetStaticMesh(MeshFinder.Get());
		}

		RootComponent = Mesh;
	}
}

void AStdProjectileMovementActor::InitMovementComponent()
{
	M_LOGFUNC();
	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	Movement->InitialSpeed = Config::SPEED;
	Movement->MaxSpeed = Config::MAX_SPEED;
	Movement->ProjectileGravityScale = 0.0F;
}

void AStdProjectileMovementActor::FireInDirection()
{
	M_LOGFUNC();
	checkf(Movement, TEXT("Movement component must be created and initialized"));
	Movement->bInitialVelocityInLocalSpace = true;
	Movement->Velocity = Movement->InitialSpeed * FVector::ForwardVector;
	ULogUtilLib::LogVectorC(TEXT("Movement->Velocity"), Movement->Velocity);
}

void AStdProjectileMovementActor::BeginPlay()
{
	M_LOGFUNC();
	Super::BeginPlay();
	FireInDirection();
}
