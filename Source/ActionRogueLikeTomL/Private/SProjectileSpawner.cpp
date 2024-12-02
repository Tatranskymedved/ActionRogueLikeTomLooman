// Jan Urbanec - based on the Course of Tom Looman


#include "SProjectileSpawner.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/StaticMeshComponent.h"
#include <Kismet/KismetMathLibrary.h>

// Sets default values
ASProjectileSpawner::ASProjectileSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASProjectileSpawner::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASProjectileSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AActor* ASProjectileSpawner::SpawnProjectile(UClass* ProjectileClass, UWorld* World, APawn* InstigatorPawn, FTransform SpawnTM)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = InstigatorPawn;

	AActor* ProjectileActor = World->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);

	return ProjectileActor;
}

AActor* ASProjectileSpawner::SpawnProjectileWithLineTrace(UClass* ProjectileClass, UWorld* World, APawn* InstigatorPawn, FVector SpawnWorldLocation, FVector CameraWorldLocation, FVector CameraControlRotationVector)
{
	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	FCollisionResponseParams ResponseParams;

	FVector TraceEnd = CameraWorldLocation + (CameraControlRotationVector * SpawnTraceDistance);

	bool bTraceSuccess = GetWorld()->LineTraceSingleByChannel(Hit, CameraWorldLocation, TraceEnd, ECC_WorldDynamic, QueryParams, ResponseParams);
	//DrawDebugLine(GetWorld(), CameraWorldLocation, Hit.Location, FColor::Purple, false, 3.0f, 0U, 2.0f);

	FRotator ProjectileRotation;
	if (bTraceSuccess)
	{
		// We usually "hit" something by the trace, we set rotation from hand to the hit location
		ProjectileRotation = UKismetMathLibrary::FindLookAtRotation(SpawnWorldLocation, Hit.Location);

		// Debugging line from camera to where we hit:
		//FVector ImpactLocation = ImpactLocation = CameraWorldLocation + (ProjectileRotation.Vector() * Hit.Distance);
		//DrawDebugLine(GetWorld(), CameraWorldLocation, Hit.Location, FColor::Green, false, 3.0f, 0U, 2.0f);
	}
	else
	{
		ProjectileRotation = UKismetMathLibrary::FindLookAtRotation(SpawnWorldLocation, TraceEnd);
	}

	FTransform SpawnTM = FTransform(ProjectileRotation, SpawnWorldLocation);
	AActor* ProjectileActor = ASProjectileSpawner::SpawnProjectile(ProjectileClass, World, InstigatorPawn, SpawnTM);

	return ProjectileActor;
}

