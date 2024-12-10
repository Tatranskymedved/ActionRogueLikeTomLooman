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
	float ProjectileLifetimeSeconds = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float ProjectileTeleportationSeconds = 0.2f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle TimerHandle_LifeTime;
	FTimerHandle TimerHandle_Teleportation;

	UPROPERTY(EditAnywhere)
	UParticleSystem* OnExplodeParticles = nullptr;
	void ProjectileLifetime_TimeElapsed();
	void ProjectileTeleportation_TimeElapsed();

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
	void Explode(FTransform Location);
};
