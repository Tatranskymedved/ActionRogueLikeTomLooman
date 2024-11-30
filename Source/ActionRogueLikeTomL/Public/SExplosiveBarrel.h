// Jan Urbanec - based on the Course of Tom Looman

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosiveBarrel.generated.h"

class UStaticMeshComponent;
class URadialForceComponent;

UCLASS()
class ACTIONROGUELIKETOML_API ASExplosiveBarrel : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASExplosiveBarrel();

protected:

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(VisibleAnywhere)
	URadialForceComponent* RadialForceComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnBarrelHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
	void Explode();
};
