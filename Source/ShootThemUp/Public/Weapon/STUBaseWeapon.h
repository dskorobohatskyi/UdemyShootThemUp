// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ASTUBaseWeapon();

    virtual void Fire();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FName MuzzleSocketName = "MuzzleFlashSocket";

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    float TraceMaxDistance = 1500.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
    float DamageAmount = 10.f;

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

protected:
    void MakeShot();

    APlayerController* GetPlayerController() const;
    bool GetCameraViewPoint(FVector& OutViewLocation, FRotator& ViewRotation) const;
    FVector GetMuzzleSocketLocation() const;

    bool MakeHitSafeForOwner(FHitResult& InHitResult);
    void MakeDamage(FHitResult& InHitResult);

private:
    //(#initiative)
    bool IsPhysicallyPossibleShot(const FVector& InShootDirection, const FVector& InTargetDirection) const;
    FVector CalculateShootDirectionFromHit(const FHitResult& InHitResult) const;
};
