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

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup",
              meta = (ClampMin = 1, ClampMax = 100.0, EditCondition = "!bRestoreFullHP"))
    int32 HPAmount = 25;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
    bool bRestoreFullHP = false;

protected:
    virtual bool OnPickupGivenToPawn(APawn* PawnActor) override;
};
