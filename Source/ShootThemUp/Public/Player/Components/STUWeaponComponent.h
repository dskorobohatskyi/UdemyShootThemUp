// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;
class UAnimMontage;



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    USTUWeaponComponent();

    virtual void StartFire();
    void StopFire();

    virtual void SwitchToNextWeapon();
    void Reload();

    bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;
    bool GetCurrentAmmoData(FAmmoData& AmmoData) const;

    bool TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount);

    // my code, I decided to expose these checks to public
    bool CanFire() const;
    bool CanEquip() const;
    bool CanReload() const;

protected:
    // Called when the game starts
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    void EquipWeapon(int32 WeaponIndex, bool bIsAnimating = true);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TArray<FWeaponData> WeaponDatas;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName WeaponEquipSocketName = TEXT("WeaponPoint");

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName WeaponArmorySocketName = TEXT("ArmorySocket");

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    UAnimMontage* EquipAnimMontage;

    UPROPERTY()
    ASTUBaseWeapon* CurrentWeapon = nullptr;

    UPROPERTY()
    TArray<ASTUBaseWeapon*> Weapons;
    int32 CurrentWeaponIndex = 0;


private:
    void SpawnWeapons();

    UPROPERTY()
    UAnimMontage* CurrentReloadAnimMontage = nullptr;


    bool bIsEquipInProgress = false;
    bool bIsReloadInProgress = false;

    void AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* MeshComponent, const FName& SocketName);

    void InitAnimations();
    void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
    void OnReloadFinished(USkeletalMeshComponent* MeshComponent);
    
    void OnClipEmptyHandle(ASTUBaseWeapon* AmmoEmptyWeapon);
    void ChangeClip();

    void PlayAnimMontage(UAnimMontage* Animation);

    // my code
    void PutWeaponToArmory(ASTUBaseWeapon* Weapon);

    USkeletalMeshComponent* GetOwnerSkeletalMesh() const;
};
