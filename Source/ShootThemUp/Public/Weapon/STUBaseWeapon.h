// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "GameFramework/Actor.h"
#include "Engine/TimerHandle.h"
#include "STUBaseWeapon.generated.h"

class USkeletalMeshComponent;

USTRUCT(Blueprintable)
struct FAmmoData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    int32 Bullets;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "!bIsInfinite"), Category = "Weapon")
    int32 Clips;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool bIsInfinite;
};

DECLARE_MULTICAST_DELEGATE(FOnClipEmptyDelegate);

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

public:
    FOnClipEmptyDelegate OnClipEmpty;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FName MuzzleSocketName = "MuzzleFlashSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float TraceMaxDistance = 1500.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FAmmoData DefaultAmmo{15, 10, false};

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
    bool IsClipEmpty() const;
    void DecreaseAmmo();

    void LogAmmo();

private:
    FAmmoData CurrentAmmo;
};
