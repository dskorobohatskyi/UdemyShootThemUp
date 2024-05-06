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
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shooting")
    float IntervalBetweenShots = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shooting")
    float WeaponSpreadAngleDegrees = 1.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shooting")
    float DamageAmount = 10.f;

    virtual void MakeShot() override;

    void MakeDamage(FHitResult& InHitResult);

    // GetTraceData impl is okay, but let's add weapon spread in ModifyShootDirectionForTrace
    virtual void ModifyShootDirectionForTrace(FVector& InOutShotDirection) override;

private:
    void AddSpreadForShooting(FVector& InOutShootDirection); // #initiative

    FTimerHandle ShootTimerHandle;
};
