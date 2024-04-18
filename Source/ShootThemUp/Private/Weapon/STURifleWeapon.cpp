// Shoot Them Up Game. All Rights Reserved

#include "Weapon/STURifleWeapon.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"

void ASTURifleWeapon::StartFire()
{
    MakeShot();

    if (GetWorld())
    {
        GetWorldTimerManager().SetTimer(ShootTimerHandle, this, &ASTURifleWeapon::MakeShot, IntervalBetweenShots,
                                        true); // IntervalBetweenShots will be used as delay
    }
}

void ASTURifleWeapon::StopFire()
{
    if (GetWorld() && ShootTimerHandle.IsValid())
    {
        GetWorldTimerManager().ClearTimer(ShootTimerHandle);
    }
}

void ASTURifleWeapon::MakeShot()
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

    // (#initiative: IsPhysicallyPossibleShot)
    // Check to not shoot target if it stands behind main character but player aim hits the target.
    const FVector MuzzleLocation = GetMuzzleSocketLocation();
    const FVector ShootDirection = CalculateShootDirectionFromHit(
        HitResult); // should be okay, since implementation doesn't require actual hit to be

    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : HitResult.TraceEnd;
    const FVector PlayerToTargetDirection = (EndPoint - MuzzleLocation).GetSafeNormal();

    // Important: IsPhysicallyPossibleShot is indicator of correct shooting even if bBlockingHit is true
    if (HitResult.bBlockingHit && IsPhysicallyPossibleShot(ShootDirection, PlayerToTargetDirection))
    {
        DrawDebugLine(GetWorld(), MuzzleLocation, HitResult.ImpactPoint, FColor::Red, false, 3.f, 0, 3.f);
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.f, 24, FColor::Yellow, false, 4.f, 0, 3.f);

        MakeDamage(HitResult);
    }
    else
    {
        DrawDebugLine(GetWorld(), MuzzleLocation, HitResult.TraceEnd, FColor::Red, false, 3.f, 0, 3.f);
    }
}

void ASTURifleWeapon::ModifyShootDirectionForTrace(FVector& InOutShotDirection)
{
    AddSpreadForShooting(InOutShotDirection);
}

void ASTURifleWeapon::AddSpreadForShooting(FVector& InOutShootDirection)
{
    InOutShootDirection = FMath::VRandCone(InOutShootDirection, FMath::DegreesToRadians(WeaponSpreadAngleDegrees));
}