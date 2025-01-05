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

	UPROPERTY(EditAnywhere, Category = "Explosion effects")
	UParticleSystem* OnExplodeParticles = nullptr;
	UPROPERTY(EditAnywhere, Category = "Explosion effects")
	USoundBase* OnExplodeAudio = nullptr;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle TimerHandle_LifeTime;
	virtual void ProjectileLifetime_TimeElapsed();

	UFUNCTION()
	virtual void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode(FTransform Location);

	// Function that will be called at the end of Explode; by default handles Actor lifetime by destroying it
	virtual void PostExplode();

public:

	virtual void PostInitializeComponents() override;
};
