// Jan Urbanec - based on the Course of Tom Looman

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKETOML_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditAnywhere)
	float TargetLidPitch;

	// Sets default values for this actor's properties
	ASItemChest();

protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Interact_Implementation(APawn* InstigatorPawn);
};
