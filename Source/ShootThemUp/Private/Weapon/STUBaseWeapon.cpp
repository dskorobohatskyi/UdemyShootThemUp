// Shoot Them Up Game. All Rights Reserved

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"

#include "Engine/World.h"
#include "Engine/HitResult.h"

#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

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
}

void ASTUBaseWeapon::StopFire() {}

// Called when the game starts or when spawned
void ASTUBaseWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponMesh);
    checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets count couldn't be less or equal zero"));
    checkf(DefaultAmmo.Clips > 0, TEXT("Clips count couldn't be less or equal zero"));

    CurrentAmmo = DefaultAmmo;
}

void ASTUBaseWeapon::MakeShot()
{
    UE_LOG(STUBaseWeaponLog, Warning, TEXT("ASTUBaseWeapon::MakeShot should be overriden in child class"));
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

bool ASTUBaseWeapon::MakeHitSafeForOwner(const FVector& InTraceStart, const FVector InTraceEnd,
                                         FHitResult& OutHitResult)
{
    if (!GetWorld())
    {
        return false;
    }

    FCollisionQueryParams CollisionQueryParams;
    CollisionQueryParams.AddIgnoredActor(GetOwner());
    const bool _ = GetWorld()->LineTraceSingleByChannel(OutHitResult, InTraceStart, InTraceEnd,
                                                        ECollisionChannel::ECC_Visibility, CollisionQueryParams);

    return true;
}

bool ASTUBaseWeapon::GetTraceData(FVector& OutTraceStart, FVector& OutTraceEnd)
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetCameraViewPoint(ViewLocation, ViewRotation))
    {
        return false;
    }

    OutTraceStart = ViewLocation;
    FVector ShootDirection = ViewRotation.Vector();
    ModifyShootDirectionForTrace(ShootDirection);
    OutTraceEnd = OutTraceStart + ShootDirection * TraceMaxDistance;

    return true;
}

//(#initiative)
bool ASTUBaseWeapon::IsPhysicallyPossibleHitFromMuzzle(const FHitResult& InHitResult) const
{
    if (!InHitResult.bBlockingHit)
    {
        return false;
    }

    // Check to not shoot target if it stands behind main character but player aim hits the target.
    const FTransform MuzzleTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);
    const FVector ShootDirection = MuzzleTransform.GetRotation().Vector();
    const FVector StartShotPoint = MuzzleTransform.GetLocation();

    const FVector TargetDirection = (InHitResult.ImpactPoint - StartShotPoint).GetSafeNormal();

    const float DotProductValue = FVector::DotProduct(ShootDirection, TargetDirection);
    const FVector CrossProductVector = FVector::CrossProduct(ShootDirection, TargetDirection);

    const float SideCorrectness = FMath::Sign(CrossProductVector.Z);
    const float DegreeBetweenVectors = FMath::RadiansToDegrees(FMath::Acos(DotProductValue) * SideCorrectness);

    UE_LOG(STUBaseWeaponLog, Verbose, TEXT("Degree: %.2f"), DegreeBetweenVectors);

    const bool bIsPhysicallyPossibleShot = DegreeBetweenVectors < 90.f;
    return bIsPhysicallyPossibleShot;
}

bool ASTUBaseWeapon::IsAmmoEmpty() const
{
    return !CurrentAmmo.bIsInfinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool ASTUBaseWeapon::IsClipEmpty() const
{
    return CurrentAmmo.Bullets == 0;
}

void ASTUBaseWeapon::DecreaseAmmo()
{
    if (CurrentAmmo.Bullets == 0)
    {
        UE_LOG(STUBaseWeaponLog, Warning, TEXT("Clip is empty. Impossible to decrease bullets!!"));
        return;
    }

    CurrentAmmo.Bullets--;

    if (IsClipEmpty() /* && !IsAmmoEmpty()*/) // my code
    {
        StopFire(); // I am not sure about this change, but let it be
        OnClipEmpty.Broadcast();
    }
}

void ASTUBaseWeapon::ChangeClip()
{
    check(CurrentAmmo.Clips != 0 || CurrentAmmo.bIsInfinite);

    if (!CurrentAmmo.bIsInfinite)
    {
        if (CurrentAmmo.Clips == 0)
        {
            UE_LOG(STUBaseWeaponLog, Warning, TEXT("Ammo is empty. Impossible to change clip!!"));
            return;
        }
        CurrentAmmo.Clips--;
    }
    CurrentAmmo.Bullets = DefaultAmmo.Bullets;
}

bool ASTUBaseWeapon::CanReload() const
{
    return CurrentAmmo.Bullets < DefaultAmmo.Bullets &&
           !IsAmmoEmpty(); // my code, in course clips > 0 is checked, but invalid for infinite set in true
}

void ASTUBaseWeapon::LogAmmo()
{
    const FString AmmoOutput =
        FString::Printf(TEXT("Ammo: %d / %s"), CurrentAmmo.Bullets,
                        (CurrentAmmo.bIsInfinite ? *FString(TEXT("\u221e")) : *FString::FromInt(CurrentAmmo.Clips)));
    UE_LOG(STUBaseWeaponLog, Display, TEXT("%s"), *AmmoOutput);
}
