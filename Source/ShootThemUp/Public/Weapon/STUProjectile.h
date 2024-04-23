// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUProjectile : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ASTUProjectile();

    void SetMovementDirection(const FVector& InDirection);

    // TODO optional implement self-aiming projectile

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    USphereComponent* CollisionComponent;

    // UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    // float ProjectileSpeed = 1000.f; // << Breaks encapsulation!! Use Movement component

    UPROPERTY()
    UProjectileMovementComponent* MovementComponent;

private:
    FVector ShotDirection;
};
