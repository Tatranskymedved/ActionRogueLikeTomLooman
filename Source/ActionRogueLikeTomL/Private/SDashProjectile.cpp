// Jan Urbanec - based on the Course of Tom Looman


#include "SDashProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include <Kismet/GameplayStatics.h>

ASDashProjectile::ASDashProjectile()
{
	DamageAmount = 0.0f;
}

void ASDashProjectile::BeginPlay()
{
	if (MovementComp)
	{
		MovementComp->ProjectileGravityScale = 0.0f;
	}

	Super::BeginPlay();
}

void ASDashProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASDashProjectile::Explode_Implementation(FTransform LocationTM)
{
	Super::Explode_Implementation(LocationTM);

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

void ASDashProjectile::PostExplode()
{
	// We override this function from paernt: Not invoking parent's variant so we don't immediatelly destroy actor;
	// Rather we let it be destroyed from `ProjectileTeleportation_TimeElapsed`
}