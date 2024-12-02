// Jan Urbanec - based on the Course of Tom Looman

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileSpawner.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS()
class ACTIONROGUELIKETOML_API ASProjectileSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASProjectileSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintReadWrite, Category = "Attack")
	float SpawnTraceDistance = 1000000.0f;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AActor* SpawnProjectile(UClass* ProjectileClass, UWorld* World, APawn* InstigatorPawn, FTransform SpawnTM);
	AActor* SpawnProjectileWithLineTrace(UClass* ProjectileClass, UWorld* World, APawn* InstigatorPawn, FVector SpawnWorldLocation, FVector CameraWorldLocation, FVector CameraControlRotationVector);
};
