// Shoot Them Up Game. All Rights Reserved

#include "Player/Components/STUWeaponComponent.h"
#include "GameFramework/Character.h"
#include "Weapon/STUBaseWeapon.h"

#include "Animations/STU_EquipFinishedAnimNotify.h"
#include "Animations/STU_ReloadFinishedAnimNotify.h"

#include "Animations/AnimUtils.h"

DEFINE_LOG_CATEGORY_STATIC(STUWeaponComponentLog, Display, All);

constexpr static int32 WeaponNum = 2;

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

    checkf(WeaponDatas.Num() == WeaponNum, TEXT("Our character can hold only %i weapons"), WeaponNum);

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
        Weapon->OnClipEmpty.AddUObject(this, &USTUWeaponComponent::OnClipEmptyHandle);

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
    ChangeClip();
}

bool USTUWeaponComponent::GetWeaponUIData(FWeaponUIData& UIData) const
{
    if (CurrentWeapon)
    {
        UIData = CurrentWeapon->GetUIData();
        return true;
    }
    return false;
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
    return CurrentWeapon && !bIsEquipInProgress && !bIsReloadInProgress && CurrentWeapon->CanReload();
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
    auto EquipFinishedNotify = AnimUtils::FindNotifyByClassSafe<USTU_EquipFinishedAnimNotify>(EquipAnimMontage);
    if (EquipFinishedNotify)
    {
        EquipFinishedNotify->OnNotifyTriggered.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
    }
    else
    {
        UE_LOG(STUWeaponComponentLog, Error, TEXT("Equip anim notify is forgotten to set"));
        checkNoEntry();
    }

    for (const auto& WeaponData : WeaponDatas)
    {
        auto ReloadFinishedNotify =
            AnimUtils::FindNotifyByClassSafe<USTU_ReloadFinishedAnimNotify>(WeaponData.ReloadAnimMontage);
        if (ReloadFinishedNotify)
        {
            ReloadFinishedNotify->OnNotifyTriggered.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
        }
        else
        {
            UE_LOG(STUWeaponComponentLog, Error, TEXT("Reload anim notify is forgotten to set"));
            checkNoEntry();
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

void USTUWeaponComponent::OnClipEmptyHandle()
{
    // TODO probably should be virtual method of base weapon (rifle override true, launcher - no need)
    const bool bIsAutoReloadEnabled = true;
    if (bIsAutoReloadEnabled)
    {
        ChangeClip();
    }
}

void USTUWeaponComponent::ChangeClip()
{
    if (!CanReload())
    {
        return;
    }

    CurrentWeapon->StopFire(); // my code
    // CurrentWeapon->ChangeClip(); // my code, I'd rather change clip in callback after reload

    bIsReloadInProgress = true;

    PlayAnimMontage(CurrentReloadAnimMontage);
}

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
    if (!MeshComponent || GetOwnerSkeletalMesh() != MeshComponent)
    {
        return;
    }

    bIsReloadInProgress = false;

    CurrentWeapon->ChangeClip(); // my code
}
