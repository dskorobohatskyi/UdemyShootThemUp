// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Pickups/STUBasePickup.h"
#include "STUHealthPickup.generated.h"

/**
 *
 */
UCLASS()
class SHOOTTHEMUP_API ASTUHealthPickup : public ASTUBasePickup
{
    GENERATED_BODY()

protected:
    virtual bool OnPickupGivenToPawn(APawn* PawnActor) override;
};
