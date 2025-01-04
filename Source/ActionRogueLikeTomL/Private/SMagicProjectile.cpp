// Jan Urbanec - based on the Course of Tom Looman


#include "SMagicProjectile.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "SAttributeComponent.h"
#include <Kismet/GameplayStatics.h>


ASMagicProjectile::ASMagicProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	//SphereComp->SetCollisionObjectType(ECC_WorldDynamic);
	//SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	//SphereComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap);
	SphereComp->SetCollisionProfileName("Projectile"); //Used Project-wide profile rather than defining per object
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorBeginOverlap);
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	//MovementComp->SetupAttachment(SphereComp);

	OnExplodeParticles = CreateDefaultSubobject<UParticleSystem>("OnExplodeParticles");

	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
}

void ASMagicProjectile::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(-1.0f * DamageAmount);
		}
	}
}

void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (SphereComp)
	{
		SphereComp->OnComponentHit.AddDynamic(this, &ASMagicProjectile::OnProjectileHit);
	}
}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
	GetWorldTimerManager().SetTimer(TimerHandle_LifeTime, this, &ASMagicProjectile::ProjectileLifetime_TimeElapsed, ProjectileLifetimeSeconds);
}

void ASMagicProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//FString CombinedString = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());
	//DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);

	FTransform HitTM;
	HitTM.SetLocation(Hit.Location);

	Explode(HitTM);
}

// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASMagicProjectile::ProjectileLifetime_TimeElapsed()
{
	FTransform actorTM = GetActorTransform();
	this->Explode(actorTM);
}

void ASMagicProjectile::Explode(FTransform LocationTM)
{
	if (MovementComp) { //Stop projectile on the spot
		MovementComp->Deactivate();
	}
	if (SphereComp) { //Stop further collisions
		SphereComp->Deactivate();
	}
	if (EffectComp) { //Disable particles
		EffectComp->Deactivate();
	}
	if (TimerHandle_LifeTime.IsValid()) {
		GetWorldTimerManager().ClearTimer(TimerHandle_LifeTime);
	}
	if (OnExplodeParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OnExplodeParticles, LocationTM, true);
	}
}