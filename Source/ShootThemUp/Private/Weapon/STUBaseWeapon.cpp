// Shoot Them Up Game. All Rights Reserved

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"

#include "Engine/World.h"
#include "Engine/HitResult.h"

#include "DrawDebugHelpers.h"
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

void ASTUBaseWeapon::Fire()
{
    MakeShot();
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
    if (!MakeHit(HitResult))
    {
        return;
    }

    // (#initiative: IsPhysicallyPossibleShot) 
    // Check to not shoot enemy if it stands behind main character but player aim hits the enemy.
    const FVector MuzzleLocation = GetMuzzleSocketLocation();
    const FVector ShootDirection = HitResult.TraceEnd - HitResult.TraceStart;
    const FVector PlayerToEnemyDirection = (HitResult.ImpactPoint - MuzzleLocation).GetSafeNormal();

    if (HitResult.bBlockingHit && IsPhysicallyPossibleShot(ShootDirection, PlayerToEnemyDirection))
    {
        DrawDebugLine(GetWorld(), MuzzleLocation, HitResult.ImpactPoint, FColor::Red, false, 3.f, 0, 3.f);
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.f, 24, FColor::Yellow, false, 4.f, 0, 3.f);
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

bool ASTUBaseWeapon::GetCameraViewPoint(FVector& OutViewLocation, FRotator& ViewRotation) const
{
    APlayerController* Controller = GetPlayerController();
    if (!Controller)
    {
        return false;
    }

    Controller->GetPlayerViewPoint(OutViewLocation, ViewRotation);
    return true;
}

FVector ASTUBaseWeapon::GetMuzzleSocketLocation() const
{
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool ASTUBaseWeapon::MakeHit(FHitResult& HitResult)
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
    const FVector ShootDirection = ViewRotation.Vector();
    const FVector TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

    FCollisionQueryParams CollisionQueryParams;
    CollisionQueryParams.AddIgnoredActor(GetOwner());
    const bool _ = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd,
                                                        ECollisionChannel::ECC_Visibility, CollisionQueryParams);

    return true;
}

//(#initiative)
bool ASTUBaseWeapon::IsPhysicallyPossibleShot(const FVector& ShootDirection, const FVector& TargetDirection) const
{
    const float DotProductValue = FVector::DotProduct(ShootDirection, TargetDirection);
    const FVector CrossProductVector = FVector::CrossProduct(ShootDirection, TargetDirection);

    const float SideCorrectness = FMath::Sign(CrossProductVector.Z);
    const float DegreeBetweenVectors = FMath::RadiansToDegrees(FMath::Acos(DotProductValue) * SideCorrectness);

    UE_LOG(STUBaseWeaponLog, Verbose, TEXT("Degree: %.2f"), DegreeBetweenVectors);

    const bool bIsPhysicallyPossibleShot = DegreeBetweenVectors < 90.f;
    return bIsPhysicallyPossibleShot;
}
