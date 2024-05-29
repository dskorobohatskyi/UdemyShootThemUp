// Shoot Them Up Game. All Rights Reserved

#include "Weapon/STURifleWeapon.h"
#include "Weapon/Components/STUWeaponFXComponent.h"

#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#ifndef STU_RIFLE_DEBUG_DRAW
#define STU_RIFLE_DEBUG_DRAW 0
#endif // !STU_RIFLE_DEBUG_DRAW

ASTURifleWeapon::ASTURifleWeapon()
{
    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTURifleWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponFXComponent);
}

void ASTURifleWeapon::StartFire()
{
    if (!GetWorld())
    {
        return;
    }

    InitMuzzleFX();
    GetWorldTimerManager().SetTimer(ShootTimerHandle,                 //
                                    this, &ASTURifleWeapon::MakeShot, //
                                    IntervalBetweenShots,             //
                                    true);                            // IntervalBetweenShots will be used as delay
    MakeShot();
}

void ASTURifleWeapon::StopFire()
{
    if (GetWorld() && ShootTimerHandle.IsValid())
    {
        GetWorldTimerManager().ClearTimer(ShootTimerHandle);
    }
    SetMuzzleFXVisibility(false);
}

void ASTURifleWeapon::MakeShot()
{
    if (IsAmmoEmpty())
    {
        StopFire();
        return;
    }

    if (!GetWorld())
    {
        return;
    }

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd))
    {
        StopFire(); // Don't understand why we need that in the course
        return;
    }

    FHitResult HitResult;
    if (!MakeHitSafeForOwner(TraceStart, TraceEnd, HitResult))
    {
        return;
    }

    const FVector MuzzleLocation = GetMuzzleSocketLocation();
    FVector TraceFXEnd = TraceEnd;
    // Important: IsPhysicallyPossibleShot is indicator of correct shooting even if bBlockingHit is true
    if (IsPhysicallyPossibleHitFromMuzzle(HitResult))
    {
        TraceFXEnd = HitResult.ImpactPoint;
        MakeDamage(HitResult);

        WeaponFXComponent->PlayImpactFX(HitResult);

#if STU_RIFLE_DEBUG_DRAW
        // Helpers to debug
        DrawDebugLine(GetWorld(), HitResult.ImpactPoint,
                      HitResult.ImpactPoint + HitResult.ImpactNormal.Rotation().Vector() * 200.f, FColor::Blue, false,
                      4.f, 0, 3);

        DrawDebugLine(GetWorld(), MuzzleLocation, HitResult.ImpactPoint, FColor::Red, false, 3.f, 0, 3.f);
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.f, 24, FColor::Yellow, false, 4.f, 0, 3.f);
#endif
    }
    
    SpawnTraceFX(MuzzleLocation, TraceFXEnd);

    DecreaseAmmo();
}

void ASTURifleWeapon::MakeDamage(FHitResult& InHitResult)
{
    AActor* DamagedActor = InHitResult.GetActor();
    if (DamagedActor)
    {
        check(DamagedActor != GetOwner());
        check(InHitResult.bBlockingHit);

        // ShootDirection should be normalized for FPointDamageEvent ctor
        const FVector ShootDirection = (InHitResult.ImpactPoint - InHitResult.TraceStart).GetSafeNormal();
        const FPointDamageEvent DamageEvent(DamageAmount, InHitResult, ShootDirection, nullptr);
        DamagedActor->TakeDamage(DamageAmount, DamageEvent, GetPlayerController(), /*GetOwner()*/ this);
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

void ASTURifleWeapon::InitMuzzleFX()
{
    if (!MuzzleFXComponent)
    {
        MuzzleFXComponent = SpawnMuzzleFX();
    }

    SetMuzzleFXVisibility(true);
}

void ASTURifleWeapon::SetMuzzleFXVisibility(bool bIsVisible)
{
    if (MuzzleFXComponent)
    {
        MuzzleFXComponent->SetPaused(!bIsVisible);
        MuzzleFXComponent->SetVisibility(bIsVisible, true);
    }
}

void ASTURifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd) {
    const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), //
                                                                                 TraceFX,    //
                                                                                 TraceStart);

    if (TraceFXComponent)
    {
        TraceFXComponent->SetVariablePosition(FName(*TraceTargetName), TraceEnd);
    }
}

#undef STU_RIFLE_DEBUG_DRAW
