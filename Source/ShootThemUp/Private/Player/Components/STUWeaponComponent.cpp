// Shoot Them Up Game. All Rights Reserved

#include "Player/Components/STUWeaponComponent.h"
#include "GameFramework/Character.h"
#include "Weapon/STUBaseWeapon.h"

#include "Animations/STU_EquipFinishedAnimNotify.h"

// Sets default values for this component's properties
USTUWeaponComponent::USTUWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::StartFire()
{
    if (!CanFire())
    {
        return;
    }

    CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire()
{
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

    InitAnimations();

    CurrentWeaponIndex = 0;
    EquipWeapon(CurrentWeaponIndex, false);
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

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex, bool bIsAnimating)
{
    check(WeaponIndex < Weapons.Num());

    auto OwnerMesh = GetOwnerSkeletalMesh();
    if (!OwnerMesh)
    {
        return;
    }

    if (CurrentWeapon)
    {
        CurrentWeapon->StopFire();
        PutWeaponToArmory(CurrentWeapon);
    }

    if (bIsAnimating)
    {
        bIsEquipInProgress = true;
        PlayAnimMontage(EquipAnimMontage);
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
    if (!CanEquip())
    {
        return;
    }

    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

bool USTUWeaponComponent::CanFire() const
{
    return CurrentWeapon && !bIsEquipInProgress;
}

bool USTUWeaponComponent::CanEquip() const
{
    return !bIsEquipInProgress;
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

void USTUWeaponComponent::InitAnimations()
{
    if (!EquipAnimMontage)
    {
        return;
    }

    const auto MontageNotifies = EquipAnimMontage->Notifies;
    for (auto& NotifyEvent : MontageNotifies)
    {
        auto EquipFinishedNotify = Cast<USTU_EquipFinishedAnimNotify>(NotifyEvent.Notify.Get());
        if (EquipFinishedNotify)
        {
            EquipFinishedNotify->OnEquipFinished.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
            break;
        }
    }
}

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
    check(EquipAnimMontage);
    ACharacter* Owner = Cast<ACharacter>(GetOwner());
    if (!Owner)
    {
        return;
    }

    Owner->PlayAnimMontage(EquipAnimMontage);
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
    if (!MeshComponent || GetOwnerSkeletalMesh() != MeshComponent)
    {
        return;
    }

    bIsEquipInProgress = false;
}
