// Shoot Them Up Game. All Rights Reserved

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"

#include "Engine/World.h"
#include "Engine/HitResult.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(STUBaseWeaponLog, Display, All);

// Sets default values
ASTUBaseWeapon::ASTUBaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
    SetRootComponent(WeaponMesh);
}

void ASTUBaseWeapon::StartFire()
{
    MakeShot();

    if (GetWorld())
    {
        GetWorldTimerManager().SetTimer(ShootTimerHandle, this, &ASTUBaseWeapon::MakeShot, IntervalBetweenShots,
                                        true); // IntervalBetweenShots will be used as delay
    }
}

void ASTUBaseWeapon::StopFire()
{
    if (GetWorld() && ShootTimerHandle.IsValid())
    {
        GetWorldTimerManager().ClearTimer(ShootTimerHandle);
    }
}

// Called when the game starts or when spawned
void ASTUBaseWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponMesh);
}

void ASTUBaseWeapon::MakeShot()
{
    if (!GetWorld())
    {
        return;
    }

    FHitResult HitResult;
    if (!MakeHitSafeForOwner(HitResult))
    {
        return;
    }

    // (#initiative: IsPhysicallyPossibleShot)
    // Check to not shoot enemy if it stands behind main character but player aim hits the enemy.
    const FVector MuzzleLocation = GetMuzzleSocketLocation();
    const FVector ShootDirection = CalculateShootDirectionFromHit(HitResult); // should be okay, since implementation doesn't require actual hit to be
    const FVector PlayerToEnemyDirection = (HitResult.ImpactPoint - MuzzleLocation).GetSafeNormal();

    if (HitResult.bBlockingHit && IsPhysicallyPossibleShot(ShootDirection, PlayerToEnemyDirection))
    {
        DrawDebugLine(GetWorld(), MuzzleLocation, HitResult.ImpactPoint, FColor::Red, false, 3.f, 0, 3.f);
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.f, 24, FColor::Yellow, false, 4.f, 0, 3.f);

        MakeDamage(HitResult);
    }
    else
    {
        DrawDebugLine(GetWorld(), GetMuzzleSocketLocation(), HitResult.TraceEnd, FColor::Red, false, 3.f, 0, 3.f);
    }
}

APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character)
    {
        return nullptr;
    }

    return Character->GetController<APlayerController>();
}

bool ASTUBaseWeapon::GetCameraViewPoint(FVector& OutViewLocation, FRotator& OutViewRotation) const
{
    APlayerController* Controller = GetPlayerController();
    if (!Controller)
    {
        return false;
    }

    Controller->GetPlayerViewPoint(OutViewLocation, OutViewRotation);
    return true;
}

FVector ASTUBaseWeapon::GetMuzzleSocketLocation() const
{
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool ASTUBaseWeapon::MakeHitSafeForOwner(FHitResult& InHitResult)
{
    if (!GetWorld())
    {
        return false;
    }

    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetCameraViewPoint(ViewLocation, ViewRotation))
    {
        return false;
    }

    const FVector TraceStart = ViewLocation;
    FVector ShootDirection = ViewRotation.Vector();
    AddNoiseForShooting(ShootDirection);
    const FVector TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

    FCollisionQueryParams CollisionQueryParams;
    CollisionQueryParams.AddIgnoredActor(GetOwner());
    const bool _ = GetWorld()->LineTraceSingleByChannel(InHitResult, TraceStart, TraceEnd,
                                                        ECollisionChannel::ECC_Visibility, CollisionQueryParams);

    return true;
}

void ASTUBaseWeapon::MakeDamage(FHitResult& InHitResult)
{
    AActor* DamagedActor = InHitResult.GetActor();
    if (DamagedActor)
    {
        check(DamagedActor != GetOwner());
        // ShootDirection should be normalized for FPointDamageEvent ctor
        const FVector ShootDirection = CalculateShootDirectionFromHit(InHitResult);
        const FPointDamageEvent DamageEvent(DamageAmount, InHitResult, ShootDirection, nullptr);
        DamagedActor->TakeDamage(DamageAmount, DamageEvent, GetPlayerController(), /*GetOwner()*/ this);
    }
}

//(#initiative)
bool ASTUBaseWeapon::IsPhysicallyPossibleShot(const FVector& InShootDirection, const FVector& InTargetDirection) const
{
    const float DotProductValue = FVector::DotProduct(InShootDirection, InTargetDirection);
    const FVector CrossProductVector = FVector::CrossProduct(InShootDirection, InTargetDirection);

    const float SideCorrectness = FMath::Sign(CrossProductVector.Z);
    const float DegreeBetweenVectors = FMath::RadiansToDegrees(FMath::Acos(DotProductValue) * SideCorrectness);

    UE_LOG(STUBaseWeaponLog, Verbose, TEXT("Degree: %.2f"), DegreeBetweenVectors);

    const bool bIsPhysicallyPossibleShot = DegreeBetweenVectors < 90.f;
    return bIsPhysicallyPossibleShot;
}

void ASTUBaseWeapon::AddSpreadForShooting(FVector& InOutShootDirection)
{
    InOutShootDirection = FMath::VRandCone(InOutShootDirection, FMath::DegreesToRadians(WeaponSpreadAngleDegrees));
}

FVector ASTUBaseWeapon::CalculateShootDirectionFromHit(const FHitResult& InHitResult) const
{
    return (InHitResult.TraceEnd - InHitResult.TraceStart).GetSafeNormal();
}
