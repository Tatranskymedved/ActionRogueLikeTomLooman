// Jan Urbanec - based on the Course of Tom Looman


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
}

bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	return true;
}
