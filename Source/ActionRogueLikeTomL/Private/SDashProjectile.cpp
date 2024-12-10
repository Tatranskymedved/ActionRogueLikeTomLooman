// Jan Urbanec - based on the Course of Tom Looman


#include "SDashProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include <Kismet/GameplayStatics.h>

ASDashProjectile::ASDashProjectile()
{
	OnExplodeParticles = CreateDefaultSubobject<UParticleSystem>("OnExplodeParticles");
}

void ASDashProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (SphereComp)
	{
		SphereComp->OnComponentHit.AddDynamic(this, &ASDashProjectile::OnProjectileHit);
	}
}

void ASDashProjectile::BeginPlay()
{
	if (MovementComp)
	{
		MovementComp->ProjectileGravityScale = 0.0f;
	}

	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_LifeTime, this, &ASDashProjectile::ProjectileLifetime_TimeElapsed, ProjectileLifetimeSeconds);
}

void ASDashProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASDashProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//FString CombinedString = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());
	//DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);

	FTransform HitTM;
	HitTM.SetLocation(Hit.Location);

	Explode(HitTM);
}


void ASDashProjectile::ProjectileLifetime_TimeElapsed()
{
	FTransform actorTM = GetActorTransform();
	this->Explode(actorTM);
}

void ASDashProjectile::Explode(FTransform LocationTM)
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

	GetWorldTimerManager().SetTimer(TimerHandle_Teleportation, this, &ASDashProjectile::ProjectileTeleportation_TimeElapsed, ProjectileTeleportationSeconds);
}

void ASDashProjectile::ProjectileTeleportation_TimeElapsed()
{
	APawn* ProjectileInstigator = GetInstigator();
	if (ProjectileInstigator)
	{
		ProjectileInstigator->TeleportTo(GetActorLocation(), GetActorRotation(), false, true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile instigator not found"));
	}

	Destroy();
}