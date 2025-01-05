// Jan Urbanec - based on the Course of Tom Looman

#pragma once

#include "CoreMinimal.h"
#include "SMagicProjectile.h"
#include "SDashProjectile.generated.h"

struct FTimerHandle;
class UParticleSystem;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKETOML_API ASDashProjectile : public ASMagicProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASDashProjectile();

	UPROPERTY(EditAnywhere, Category = "Attack")
	float ProjectileTeleportationSeconds = 0.2f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle TimerHandle_Teleportation;
	void ProjectileTeleportation_TimeElapsed();

	virtual void Explode_Implementation(FTransform Location) override;

	virtual void PostExplode() override;
public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
