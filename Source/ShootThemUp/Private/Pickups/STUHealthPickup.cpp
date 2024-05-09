// Shoot Them Up Game. All Rights Reserved

#include "Pickups/STUHealthPickup.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All);

bool ASTUHealthPickup::OnPickupGivenToPawn(APawn* PawnActor)
{
    UE_LOG(LogHealthPickup, Display, TEXT("Ammo Health was taken"));
    return true;
}
