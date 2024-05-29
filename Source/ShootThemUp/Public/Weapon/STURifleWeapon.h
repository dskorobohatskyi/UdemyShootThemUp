// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

class USTUWeaponFXComponent;
class UNiagaraComponent;
class UNiagaraSystem;

/**
 *
 */
UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
    GENERATED_BODY()

public:
    ASTURifleWeapon();

    virtual void StartFire() override;
    virtual void StopFire() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shooting")
    float IntervalBetweenShots = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shooting")
    float WeaponSpreadAngleDegrees = 1.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shooting")
    float DamageAmount = 10.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* TraceFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FString TraceTargetName = TEXT("TraceTarget");

    UPROPERTY(VisibleAnywhere, Category = "VFX")
    USTUWeaponFXComponent* WeaponFXComponent;

    virtual void MakeShot() override;

    void MakeDamage(FHitResult& InHitResult);

    // GetTraceData impl is okay, but let's add weapon spread in ModifyShootDirectionForTrace
    virtual void ModifyShootDirectionForTrace(FVector& InOutShotDirection) override;

    virtual void BeginPlay() override;

private:
    void AddSpreadForShooting(FVector& InOutShootDirection); // #initiative

    FTimerHandle ShootTimerHandle;

    // VFX
    UPROPERTY()
    UNiagaraComponent* MuzzleFXComponent;

    void InitMuzzleFX();
    void SetMuzzleFXVisibility(bool bIsVisible);
    void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);
};
