// Shoot Them Up Game. All Rights Reserved

#include "Weapon/STUProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ASTUProjectile::ASTUProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    CollisionComponent->InitSphereRadius(5.f);
    SetRootComponent(CollisionComponent);

    // logical component
    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
    MovementComponent->ProjectileGravityScale = 0.f;
    MovementComponent->InitialSpeed = 2000.f;
}

void ASTUProjectile::SetMovementDirection(const FVector& InDirection)
{
    ShotDirection = InDirection.GetSafeNormal();
}

void ASTUProjectile::BeginPlay()
{
    Super::BeginPlay();

    check(MovementComponent);
    check(!MovementComponent->Velocity.IsNearlyZero());
    check(!FMath::IsNearlyZero(MovementComponent->InitialSpeed));

    MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
    SetLifeSpan(5.f);
}
