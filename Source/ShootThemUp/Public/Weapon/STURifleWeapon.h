// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

/**
 *
 */
UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
    GENERATED_BODY()

public:
    virtual void StartFire() override;
    virtual void StopFire() override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Shooting")
    float IntervalBetweenShots = 0.1f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Shooting")
    float WeaponSpreadAngleDegrees = 1.5f;

    virtual void MakeShot() override;

    // GetTraceData impl is okay, but let's add weapon spread in ModifyShootDirectionForTrace
    virtual void ModifyShootDirectionForTrace(FVector& InOutShotDirection) override;

private:
    void AddSpreadForShooting(FVector& InOutShootDirection); // #initiative

    FTimerHandle ShootTimerHandle;
};
