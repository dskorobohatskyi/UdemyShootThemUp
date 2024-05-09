// Shoot Them Up Game. All Rights Reserved

#include "Pickups/STUHealthPickup.h"

#include "Player/Components/STUHealthComponent.h"

#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All);

bool ASTUHealthPickup::OnPickupGivenToPawn(APawn* PawnActor)
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PawnActor);
    if (!HealthComponent /*|| HealthComponent->IsDead()*/) //spectator doesn't have health comp
    {
        return false;
    }

    if (HealthComponent->HasFullHP())
    {
        return false;
    }

    if (bRestoreFullHP)
    {
        HealthComponent->HealToFull();
    }
    else
    {
        HealthComponent->Heal(HPAmount);
    }

    return true;
}
