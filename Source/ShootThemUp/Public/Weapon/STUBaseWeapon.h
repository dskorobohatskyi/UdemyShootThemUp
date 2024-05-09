// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "GameFramework/Actor.h"
#include "Engine/TimerHandle.h"
#include "STUCoreTypes.h"
#include "STUBaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ASTUBaseWeapon();

    virtual void StartFire();
    virtual void StopFire();

    void ChangeClip();
    bool CanReload() const;

    FWeaponUIData GetCurrentUIData() const { return UIData; }
    FAmmoData GetCurrentAmmoData() const { return CurrentAmmo; }

    bool TryToAddAmmo(int32 ClipsAmount);

public:
    FOnClipEmptyDelegate OnClipEmpty;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName MuzzleSocketName = "MuzzleFlashSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TraceMaxDistance = 1500.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoData DefaultAmmo{15, 10, false};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FWeaponUIData UIData;

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

protected:
    virtual void MakeShot();
    bool MakeHitSafeForOwner(const FVector& InTraceStart, const FVector InTraceEnd, FHitResult& OutHitResult);
    virtual bool GetTraceData(FVector& OutTraceStart, FVector& OutTraceEnd);
    virtual void ModifyShootDirectionForTrace(FVector& InOutShotDirection) {}

    APlayerController* GetPlayerController() const;
    bool GetCameraViewPoint(FVector& OutViewLocation, FRotator& ViewRotation) const;
    FVector GetMuzzleSocketLocation() const;

    //(#initiative)
    virtual bool IsPhysicallyPossibleHitFromMuzzle(const FHitResult& InHitResult) const;

    // Ammo related methods
    bool IsAmmoEmpty() const;
    bool IsAmmoFull() const;
    bool IsClipEmpty() const;
    void DecreaseAmmo();

    void LogAmmo();

private:
    FAmmoData CurrentAmmo;
};
