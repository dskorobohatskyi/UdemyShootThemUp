// Shoot Them Up Game. All Rights Reserved

#include "Player/Components/STUWeaponComponent.h"
#include "GameFramework/Character.h"
#include "Weapon/STUBaseWeapon.h"

// Sets default values for this component's properties
USTUWeaponComponent::USTUWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::Fire()
{
    if (CurrentWeapon == nullptr)
    {
        return;
    }

    CurrentWeapon->Fire();
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
}