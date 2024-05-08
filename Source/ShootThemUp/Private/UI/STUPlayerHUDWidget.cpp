// Shoot Them Up Game. All Rights Reserved

#include "UI/STUPlayerHUDWidget.h"
#include "Player/Components/STUHealthComponent.h"

float USTUPlayerHUDWidget::GetHealthPercent() const
{
    APawn* OwningPlayer = GetOwningPlayerPawn();
    if (!OwningPlayer)
    {
        return 0.0f;
    }

    auto HealthComponent = OwningPlayer->GetComponentByClass<USTUHealthComponent>();
    if (!HealthComponent)
    {
        return 0.0f;
    }

    return HealthComponent->GetHealthPercent();
}
