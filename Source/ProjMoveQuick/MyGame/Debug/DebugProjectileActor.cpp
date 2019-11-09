#include "DebugProjectileActor.h"
#include "Util/Core/LogUtilLib.h"
#include "Util/Core/Phys/PhysUtilLib.h"

#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "UObject/ConstructorHelpers.h"

#include "Engine/CollisionProfile.h"

/**
* TODO:
* 0. Config
* 0.1. Change config
* 0.2. Init default damage type
* 1. Handle collision
* 1.0. Override collision handling method
* 1.1. Stop movement (+DONE)
* 1.2. Make damage (+DONE)
* 1.3. Begin destruction
* 2. Init projectile movement component
* 2.1. Init movement direction
*/

using namespace ProjectileConfig;

ADebugProjectileActor::ADebugProjectileActor()
{
	OnActorHit.AddDynamic(this, &ADebugProjectileActor::ActorHit);

	InitMesh(nullptr);
	RootComponent = Mesh;
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootSceneComponent->SetupAttachment(RootComponent);

	InitCameraAndSpringArm(RootSceneComponent);
	InitProxSphere(RootSceneComponent);

	InitProjectileMovementComponent(RootComponent);
}

void ADebugProjectileActor::BeginPlay()
{
	M_LOGFUNC();
	Super::BeginPlay();
	SetLifeSpan(ProjectileConfig::Default::MAX_LIFETIME_SECONDS);
	FireProjectile();
}

void ADebugProjectileActor::ActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	M_LOGFUNC();

	MakeImpact(Hit.Actor.Get(), Hit);
}

void ADebugProjectileActor::MakeDamage(AActor* const ActorToDamage, const FHitResult& InHitInfo)
{
	M_LOGFUNC();
	ULogUtilLib::LogKeyedNameClassSafeC(TEXT("ActorToDamage"), ActorToDamage);
	UPhysUtilLib::LogHitResult(InHitInfo);

	if(bPointDamage)
	{
		if(ActorToDamage)
		{
			UGameplayStatics::ApplyPointDamage(ActorToDamage, Damage, GetVelocity(), InHitInfo, GetInstigatorController(), Instigator, DamageTypeClass);
		}
	}	
	else
	{
		TArray<AActor*> IgnoreActors; 
		UGameplayStatics::ApplyRadialDamage(this, Damage, GetActorLocation(), DamageRadius, DamageTypeClass, IgnoreActors, Instigator, GetInstigatorController());
	}
}


void ADebugProjectileActor::MakeImpact(AActor* const ActorToDamage, const FHitResult& InHitInfo)
{
	M_LOGFUNC();

	MakeDamage(ActorToDamage, InHitInfo);

	ProjectileMovementComponent->StopMovementImmediately();

	// TODO: Spawn explosition

	bool bDestroyed = Destroy();
	M_LOG_WARN_IF( ! bDestroyed, TEXT("AActor::Destroy() returned false for '%s'"), *ULogUtilLib::GetNameAndClassSafe(this));
}

void ADebugProjectileActor::FireProjectile()
{
	M_LOGFUNC();

	float const INITIAL_SPEED = ProjectileMovementComponent->InitialSpeed;

	//ProjectileMovementComponent->Velocity = UpdatedComponent->GetComponentQuat().Vector() * INITIAL_SPEED;
	ProjectileMovementComponent->Velocity = GetActorQuat().Vector().GetSafeNormal() * INITIAL_SPEED;
	ProjectileMovementComponent->bInitialVelocityInLocalSpace = false;
	ProjectileMovementComponent->UpdateComponentVelocity(); // Does NOT help

	ULogUtilLib::LogFloatC(TEXT("INITIAL_SPEED"), INITIAL_SPEED);
	ULogUtilLib::LogQuatC(TEXT("GetActorQuat()"), GetActorQuat());
	ULogUtilLib::LogVectorC(TEXT("GetActorQuat().Vector()"), GetActorQuat().Vector());
	ULogUtilLib::LogVectorC(TEXT("GetActorQuat().Vector().SafeNormal()"), GetActorQuat().Vector().GetSafeNormal());
	ULogUtilLib::LogVectorC(TEXT("Velocity"), ProjectileMovementComponent->Velocity);
}

void ADebugProjectileActor::InitProjectileMovementComponent(USceneComponent* InUpdatedComponent)
{
	M_LOGFUNC();
	float const INITIAL_SPEED = ProjectileConfig::Default::SPEED;

	check(InUpdatedComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	//ProjectileMovementComponent->SetUpdatedComponent(InUpdatedComponent);
	ProjectileMovementComponent->MaxSpeed = ProjectileConfig::Default::MAX_SPEED;
	ProjectileMovementComponent->InitialSpeed = INITIAL_SPEED;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0F;
	ProjectileMovementComponent->bInitialVelocityInLocalSpace = false;
}

void ADebugProjectileActor::InitCameraAndSpringArm(USceneComponent* InAttachTo)
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = ProjectileConfig::Default::SPRINGARM_TARGET_ARM_LENGTH;
	SpringArm->RelativeRotation = ProjectileConfig::Default::SPRINGARM_RELATIVE_ROTATION;
	SpringArm->RelativeLocation = ProjectileConfig::Default::SPRINGARM_RELATIVE_LOCATION;
	SpringArm->bEnableCameraLag = ProjectileConfig::Default::SPRINGARM_ENABLE_CAMERA_LAG;
	SpringArm->CameraLagSpeed = ProjectileConfig::Default::SPRINGARM_CAMERA_LAG_SPEED;
	if(InAttachTo)
	{
		SpringArm->SetupAttachment(InAttachTo);
	}

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = true;
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
}

void ADebugProjectileActor::InitMesh(USceneComponent* InAttachTo)
{
	static ConstructorHelpers::FObjectFinderOptional<UStaticMesh> MeshFinder { ProjectileConfig::Default::MESH_ASSET_PATH };
	M_LOG_ERROR_IF( ! MeshFinder.Succeeded(), TEXT("ProjectileConfig mesh (\"%s\") NOT found"), ProjectileConfig::Default::MESH_ASSET_PATH);

	{
		Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

		if(MeshFinder.Succeeded())
		{
			M_LOG(TEXT("ProjectileConfig mesh (\"%s\") found, setting it up"), ProjectileConfig::Default::MESH_ASSET_PATH);
			Mesh->SetStaticMesh(MeshFinder.Get());
			Mesh->SetCollisionProfileName(UCollisionProfile::DefaultProjectile_ProfileName);
			Mesh->SetRelativeScale3D(FVector{ProjectileConfig::Default::Scale});
		}

		if(InAttachTo)
		{
			Mesh->SetupAttachment(InAttachTo);
		}
	}
}

void ADebugProjectileActor::InitProxSphere(USceneComponent* InAttachTo)
{
	ProxSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	ProxSphere->InitSphereRadius(ProjectileConfig::Default::PROX_SPHERE_RADIUS);
	ProxSphere->RelativeLocation = ProjectileConfig::Default::MESH_REAL_CENTER_ACTOR_SPACE_LOCATION;
	if(InAttachTo)
	{
		ProxSphere->SetupAttachment(InAttachTo);
		ProxSphere->SetRelativeScale3D(FVector{ProjectileConfig::Default::Scale});
	}
}
