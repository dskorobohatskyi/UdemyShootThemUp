// Shoot Them Up Game. All Rights Reserved

#include "Player/Components/STUWeaponComponent.h"
#include "GameFramework/Character.h"
#include "Weapon/STUBaseWeapon.h"

#include "Animations/STU_EquipFinishedAnimNotify.h"
#include "Animations/STU_ReloadFinishedAnimNotify.h"

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
    check(WeaponDatas.Num() != 0);

    ACharacter* Owner = Cast<ACharacter>(GetOwner());

    if (!GetWorld() || !Owner)
    {
        return;
    }

    for (const auto& WeaponData : WeaponDatas)
    {
        auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponData.WeaponClass);
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
    check(WeaponIndex >= 0 && WeaponIndex < Weapons.Num());

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

    // code below can fail if some sort logic is present
    // CurrentReloadAnimMontage = WeaponDatas[CurrentWeaponIndex].ReloadAnimMontage;
    const auto CurrentWeaponData = WeaponDatas.FindByPredicate([this](const FWeaponData& Data) { //
        return CurrentWeapon->GetClass() == Data.WeaponClass;                                    //
    });
    CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;
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

    CurrentWeapon->StopFire(); // my code

    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::Reload()
{
    if (!CanReload())
    {
        return;
    }

    CurrentWeapon->StopFire(); // my code

    bIsReloadInProgress = true;

    PlayAnimMontage(CurrentReloadAnimMontage);
}

bool USTUWeaponComponent::CanFire() const
{
    return CurrentWeapon && !bIsEquipInProgress && !bIsReloadInProgress;
}

bool USTUWeaponComponent::CanEquip() const
{
    return !bIsEquipInProgress && !bIsReloadInProgress;
}

bool USTUWeaponComponent::CanReload() const
{
    // TODO add ammo check c'mon
    return CurrentWeapon && !bIsEquipInProgress && !bIsReloadInProgress;
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
    auto EquipFinishedNotify = FindNotifyByClassSafe<USTU_EquipFinishedAnimNotify>(EquipAnimMontage);
    if (EquipFinishedNotify)
    {
        EquipFinishedNotify->OnNotifyTriggered.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
    }

    for (const auto& WeaponData : WeaponDatas)
    {
        auto ReloadFinishedNotify = FindNotifyByClassSafe<USTU_ReloadFinishedAnimNotify>(WeaponData.ReloadAnimMontage);
        if (ReloadFinishedNotify)
        {
            ReloadFinishedNotify->OnNotifyTriggered.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
        }
    }
}

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
    check(Animation);
    ACharacter* Owner = Cast<ACharacter>(GetOwner());
    if (!Owner)
    {
        return;
    }

    Owner->PlayAnimMontage(Animation);
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
    if (!MeshComponent || GetOwnerSkeletalMesh() != MeshComponent)
    {
        return;
    }

    bIsEquipInProgress = false;
}

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
    if (!MeshComponent || GetOwnerSkeletalMesh() != MeshComponent)
    {
        return;
    }

    bIsReloadInProgress = false;
    // TODO CurrentWeapon change clip should be called
}
