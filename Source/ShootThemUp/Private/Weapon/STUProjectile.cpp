// Shoot Them Up Game. All Rights Reserved

#include "Weapon/STUProjectile.h"
#include "Weapon/Components/STUWeaponFXComponent.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASTUProjectile::ASTUProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    CollisionComponent->InitSphereRadius(5.f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    SetRootComponent(CollisionComponent);

    // logical component
    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
    MovementComponent->ProjectileGravityScale = 0.f;
    MovementComponent->InitialSpeed = 2000.f;

    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
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
    check(WeaponFXComponent);

    MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
    CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
    SetLifeSpan(LifeSeconds);

    CollisionComponent->OnComponentHit.AddDynamic(this, &ASTUProjectile::OnProjectileHit);
}

void ASTUProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    check(Hit.bBlockingHit);

    if (!GetWorld())
    {
        return;
    }

    MovementComponent->StopMovementImmediately();

    UGameplayStatics::ApplyRadialDamage(GetWorld(),                 //
                                        DamageAmount,               //
                                        GetActorLocation(),         //
                                        DamageRadius,               //
                                        UDamageType::StaticClass(), //
                                        {GetOwner()},               //
                                        this,                       //
                                        GetController(),            //
                                        bDoFullDamage);

    WeaponFXComponent->PlayImpactFX(Hit);
    DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, 2.f, 0, 3.f);

    Destroy();
}

AController* ASTUProjectile::GetController() const
{
    APawn* Pawn = Cast<APawn>(GetOwner());
    if (!Pawn)
    {
        return nullptr;
    }

    return Pawn->GetController();
}
