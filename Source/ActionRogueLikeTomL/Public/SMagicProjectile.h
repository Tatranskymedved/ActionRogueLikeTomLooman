// Jan Urbanec - based on the Course of Tom Looman

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SMagicProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS()
class ACTIONROGUELIKETOML_API ASMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ASMagicProjectile();

	/// <summary>
	/// Positive value of how much HP should be taken from target. If negative, it will be healing target instead.
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Attack")
	float DamageAmount = 20.0f;

	/// <summary>
	/// Amount of seconds after which projectile will `Explode` on its own.
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Attack")
	float ProjectileLifetimeSeconds = 2.0f;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* EffectComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* AudioComp;

	UPROPERTY(EditAnywhere)
	UParticleSystem* OnExplodeParticles = nullptr;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle TimerHandle_LifeTime;
	virtual void ProjectileLifetime_TimeElapsed();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	virtual void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
	virtual void Explode(FTransform Location);
};
