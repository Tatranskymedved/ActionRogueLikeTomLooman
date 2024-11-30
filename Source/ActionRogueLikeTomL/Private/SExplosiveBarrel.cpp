// Jan Urbanec - based on the Course of Tom Looman


#include "SExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	if (StaticMeshComp)
	{
		StaticMeshComp->SetSimulatePhysics(true);
		StaticMeshComp->SetCollisionProfileName("PhysicsActor");
		RootComponent = StaticMeshComp;
	}

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	if (RadialForceComp)
	{
		RadialForceComp->Radius = 700;
		RadialForceComp->ImpulseStrength = 2000;
		RadialForceComp->bImpulseVelChange = true;
		RadialForceComp->SetAutoActivate(false);
		RadialForceComp->SetupAttachment(StaticMeshComp);
	}
}

void ASExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (StaticMeshComp)
	{
		StaticMeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnBarrelHit);
	}
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASExplosiveBarrel::OnBarrelHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

void ASExplosiveBarrel::Explode()
{
	if (RadialForceComp)
	{
		RadialForceComp->FireImpulse();
	}
}

