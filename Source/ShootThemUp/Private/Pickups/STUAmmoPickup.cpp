// Shoot Them Up Game. All Rights Reserved

#include "Pickups/STUAmmoPickup.h"
#include "Player/Components/STUWeaponComponent.h"
#include "Player/Components/STUHealthComponent.h"

#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogAmmoPickup, All, All);

bool ASTUAmmoPickup::OnPickupGivenToPawn(APawn* PawnActor)
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PawnActor);
    if (!HealthComponent || HealthComponent->IsDead())
    {
        return false;
    }
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(PawnActor);
    if (!WeaponComponent)
    {
        return false;
    }

    return WeaponComponent->TryToAddAmmo(WeaponType, ClipsAmount);
}
