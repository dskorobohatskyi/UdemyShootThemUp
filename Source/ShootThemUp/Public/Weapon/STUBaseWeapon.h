// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "GameFramework/Actor.h"
#include "Engine/TimerHandle.h"
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

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FName MuzzleSocketName = "MuzzleFlashSocket";

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    float TraceMaxDistance = 1500.f;

    // I guess EditAnywhere can be used for powerups in the future, but for now VisibleAnywhere is okay
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Shooting")
    float DamageAmount = 10.f;

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

    void MakeDamage(FHitResult& InHitResult);

    //(#initiative)
    virtual bool IsPhysicallyPossibleShot(const FVector& InShootDirection, const FVector& InTargetDirection) const;
    FVector CalculateShootDirectionFromHit(const FHitResult& InHitResult) const;
};
