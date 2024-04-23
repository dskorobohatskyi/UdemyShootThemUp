// Shoot Them Up Game. All Rights Reserved

#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"

void ASTULauncherWeapon::StartFire()
{
    MakeShot();
}

void ASTULauncherWeapon::MakeShot()
{
    if (!GetWorld())
    {
        return;
    }

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd))
    {
        return;
    }

    FHitResult HitResult;
    if (!MakeHitSafeForOwner(TraceStart, TraceEnd, HitResult))
    {
        return;
    }

    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleSocketLocation());
    auto Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform, GetOwner());

    if (Projectile)
    {
        // where to initialize hit
        const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
        auto ShootDirection = (EndPoint - TraceStart).GetSafeNormal();

        Projectile->SetMovementDirection(ShootDirection);
        Projectile->FinishSpawning(SpawnTransform);
    }
}
