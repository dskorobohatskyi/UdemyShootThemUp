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

FVector ASTUBaseWeapon::CalculateShootDirectionFromHit(const FHitResult& InHitResult) const
{
    return (InHitResult.TraceEnd - InHitResult.TraceStart).GetSafeNormal();
}
