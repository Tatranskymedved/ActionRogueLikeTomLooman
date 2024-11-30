// Jan Urbanec - based on the Course of Tom Looman


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	QueryInteractDistance = 1000.0f;
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USInteractionComponent::PrimaryInteract()
{
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector End = EyeLocation + (EyeRotation.Vector() * QueryInteractDistance);

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

		// This is one option: Single Line trace, depending on ObjectType
		//FHitResult HitResult;
		//bool IsBlockingHit = GetWorld()->LineTraceSingleByObjectType(HitResult, EyeLocation, End, ObjectQueryParams);

		// This is other option: SweepMultiple results around within radius
		FCollisionShape Shape;
		Shape.SetSphere(QueryInteractDistance);

		TArray<FHitResult> HitResultArray;
		bool HasFoundAnyObject = GetWorld()->SweepMultiByObjectType(HitResultArray, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

		AActor* HitActor = NULL;
		//HitActor = HitResult.GetActor();

		// Other approach that checks last elementy in array (should be the closests) and we use it
		//if (HitResultArray.IsEmpty() == false &&
		//	HitResultArray.Top().HasValidHitObjectHandle())
		//{
		//	HitActor = HitResultArray.Top().GetActor();
		//}

		// Iterate over all objects who implements GameplayInterface and execute on each that is within range
		for (FHitResult HitResult : HitResultArray)
		{
			HitActor = HitResult.GetActor();


			if (HitActor)
			{
				if (HitActor->Implements<USGameplayInterface>())
				{
					APawn* MyOwnerAsPawn = Cast<APawn>(MyOwner);

					ISGameplayInterface::Execute_Interact(HitActor, MyOwnerAsPawn);
				}
			}
			//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, QueryInteractDistance, 20, HasFoundAnyObject ? FColor::Green : FColor::Red, false, 2.0f, 0, 2.0f);
		}

		//DrawDebugLine(GetWorld(), EyeLocation, End, HasFoundAnyObject ? FColor::Green : FColor::Red, false, 2.0f, 0, 2.0f);
		//DrawDebugSphere(GetWorld(), EyeLocation, QueryInteractDistance, 20, HasFoundAnyObject ? FColor::Green : FColor::Red, false, 2.0f, 0, 2.0f);
	}

}

