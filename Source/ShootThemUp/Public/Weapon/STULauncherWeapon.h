// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STULauncherWeapon.generated.h"

/**
 *
 */
UCLASS()
class SHOOTTHEMUP_API ASTULauncherWeapon : public ASTUBaseWeapon
{
    GENERATED_BODY()

public:
    virtual void StartFire() override;
    virtual void StopFire() override {}

protected:
    virtual void MakeShot() override;

    // Let's explicitly show that Launcher doesn't modify shoot direction with some spread
    virtual void ModifyShootDirectionForTrace(FVector& InOutShotDirection) override {}

protected:
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<class ASTUProjectile> ProjectileClass;
};
