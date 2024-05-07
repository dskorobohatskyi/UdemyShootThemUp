// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;
class UAnimMontage;

USTRUCT(Blueprintable)
struct FWeaponData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<ASTUBaseWeapon> WeaponClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    UAnimMontage* ReloadAnimMontage;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    USTUWeaponComponent();

    void StartFire();
    void StopFire();

    void SwitchToNextWeapon();
    void Reload();

    bool CanFire() const;
    bool CanEquip() const;
    bool CanReload() const;

protected:
    // Called when the game starts
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TArray<FWeaponData> WeaponDatas;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName WeaponEquipSocketName = TEXT("WeaponPoint");

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName WeaponArmorySocketName = TEXT("ArmorySocket");

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    UAnimMontage* EquipAnimMontage;

private:
    void SpawnWeapons();

    UPROPERTY()
    ASTUBaseWeapon* CurrentWeapon = nullptr;

    UPROPERTY()
    TArray<ASTUBaseWeapon*> Weapons;

    UPROPERTY()
    UAnimMontage* CurrentReloadAnimMontage = nullptr;

    int32 CurrentWeaponIndex = 0;

    bool bIsEquipInProgress = false;
    bool bIsReloadInProgress = false;

    void AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* MeshComponent, const FName& SocketName);
    void EquipWeapon(int32 WeaponIndex, bool bIsAnimating = true);

    void InitAnimations();
    void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
    void OnReloadFinished(USkeletalMeshComponent* MeshComponent);

    void PlayAnimMontage(UAnimMontage* Animation);

    template <typename T> T* FindNotifyByClassSafe(const UAnimSequenceBase* const Animation) const
    {
        if (!Animation)
        {
            return nullptr;
        }

        const auto MontageNotifies = Animation->Notifies;
        for (auto& NotifyEvent : MontageNotifies)
        {
            auto Notify = Cast<T>(NotifyEvent.Notify.Get());
            if (Notify)
            {
                return Notify;
            }
        }
        return nullptr;
    }

    // my code
    void PutWeaponToArmory(ASTUBaseWeapon* Weapon);

    USkeletalMeshComponent* GetOwnerSkeletalMesh() const;
};
