// Jan Urbanec - based on the Course of Tom Looman


#include "STargetDummy.h"

#include "Components/StaticMeshComponent.h"
#include "SAttributeComponent.h"

// Sets default values
ASTargetDummy::ASTargetDummy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
}

void ASTargetDummy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Trigger when health is changed (damage/healing)
	if (AttributeComp)
	{
		AttributeComp->OnHealthChanged.AddDynamic(this, &ASTargetDummy::OnHealthChanged);
	}
}

void ASTargetDummy::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
}
