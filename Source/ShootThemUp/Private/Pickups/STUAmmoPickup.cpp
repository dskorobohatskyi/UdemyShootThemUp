// Shoot Them Up Game. All Rights Reserved

#include "Pickups/STUAmmoPickup.h"

DEFINE_LOG_CATEGORY_STATIC(LogAmmoPickup, All, All);

bool ASTUAmmoPickup::OnPickupGivenToPawn(APawn* PawnActor)
{
    UE_LOG(LogAmmoPickup, Display, TEXT("Ammo Pickup was taken"));
    return true;
}
