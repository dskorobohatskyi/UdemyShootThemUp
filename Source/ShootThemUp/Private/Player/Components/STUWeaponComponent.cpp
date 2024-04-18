// Shoot Them Up Game. All Rights Reserved

#include "Player/Components/STUWeaponComponent.h"
#include "GameFramework/Character.h"
#include "Weapon/STUBaseWeapon.h"

// Sets default values for this component's properties
USTUWeaponComponent::USTUWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::StartFire()
{
    if (CurrentWeapon == nullptr)
    {
        return;
    }

    CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire()
{
    // TODO need to stop fire
    if (CurrentWeapon == nullptr)
    {
        return;
    }

    CurrentWeapon->StopFire();
}

// Called when the game starts
void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    SpawnWeapon();
}

void USTUWeaponComponent::SpawnWeapon()
{
    if (GetWorld() == nullptr)
    {
        return;
    }

    check(WeaponClass.Get() != nullptr);

    ACharacter* Owner = Cast<ACharacter>(GetOwner());
    if (Owner == nullptr)
    {
        return;
    }

    CurrentWeapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);

    if (CurrentWeapon == nullptr)
    {
        return;
    }

    const FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, false);
    CurrentWeapon->AttachToComponent(Owner->GetMesh(), TransformRules, WeaponSocketName);
    CurrentWeapon->SetOwner(GetOwner()); // base character should be the new owner of weapon
}