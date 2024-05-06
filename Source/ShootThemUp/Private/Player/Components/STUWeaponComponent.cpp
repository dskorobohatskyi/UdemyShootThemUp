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

    SpawnWeapons();

    CurrentWeaponIndex = 0;
    EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    CurrentWeapon = nullptr;

    for (const auto Weapon : Weapons)
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();
    }
    Weapons.Empty();

    Super::EndPlay(EndPlayReason);
}

void USTUWeaponComponent::SpawnWeapons()
{
    check(WeaponClasses.Num() != 0);

    ACharacter* Owner = Cast<ACharacter>(GetOwner());

    if (!GetWorld() || !Owner)
    {
        return;
    }

    for (const auto WeaponClass : WeaponClasses)
    {
        auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);
        if (!Weapon)
            continue;

        Weapon->SetOwner(Owner); // base character should be the new owner of weapon
        PutWeaponToArmory(Weapon);

        Weapons.Add(Weapon);
    }
}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* MeshComponent,
                                               const FName& SocketName)
{
    if (!Weapon || !MeshComponent)
    {
        return;
    }

    const FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(MeshComponent, TransformRules, SocketName);
}

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
    check(WeaponIndex < Weapons.Num());

    USkeletalMeshComponent* OwnerMesh = GetOwnerSkeletalMesh();
    if (!OwnerMesh)
    {
        return;
    }

    if (CurrentWeapon)
    {
        CurrentWeapon->StopFire();
        PutWeaponToArmory(CurrentWeapon);
    }

    AttachWeaponToSocket(Weapons[WeaponIndex], OwnerMesh, WeaponEquipSocketName);
    CurrentWeapon = Weapons[WeaponIndex];
}

void USTUWeaponComponent::PutWeaponToArmory(ASTUBaseWeapon* Weapon)
{
    USkeletalMeshComponent* OwnerMesh = GetOwnerSkeletalMesh();
    if (!Weapon || !OwnerMesh)
    {
        return;
    }
    AttachWeaponToSocket(Weapon, OwnerMesh, WeaponArmorySocketName);
}

void USTUWeaponComponent::SwitchToNextWeapon()
{
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

USkeletalMeshComponent* USTUWeaponComponent::GetOwnerSkeletalMesh() const
{
    ACharacter* Owner = Cast<ACharacter>(GetOwner());
    if (!Owner)
    {
        return nullptr;
    }
    return Owner->GetMesh();
}
