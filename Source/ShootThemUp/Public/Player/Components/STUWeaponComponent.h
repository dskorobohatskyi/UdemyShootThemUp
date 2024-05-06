// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

    void StartFire();
    void StopFire();

    void SwitchToNextWeapon();

    bool CanFire() const;
    bool CanEquip() const;

protected:
    // Called when the game starts
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TArray<TSubclassOf<ASTUBaseWeapon>> WeaponClasses;

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

    TArray<ASTUBaseWeapon*> Weapons;

    int32 CurrentWeaponIndex = 0;

    bool bIsEquipInProgress = false;

    void AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* MeshComponent, const FName& SocketName);
    void EquipWeapon(int32 WeaponIndex, bool bIsAnimating = true);
    
    void InitAnimations();
    void OnEquipFinished(USkeletalMeshComponent* MeshComponent);

    void PlayAnimMontage(UAnimMontage* Animation);

    // my code
    void PutWeaponToArmory(ASTUBaseWeapon* Weapon);

    USkeletalMeshComponent* GetOwnerSkeletalMesh() const;
};
