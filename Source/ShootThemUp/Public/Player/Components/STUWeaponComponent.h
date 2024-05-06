// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;

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

private:
    void SpawnWeapons();

    UPROPERTY()
    ASTUBaseWeapon* CurrentWeapon = nullptr;

    TArray<ASTUBaseWeapon*> Weapons;

    int32 CurrentWeaponIndex = 0;


    void AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* MeshComponent, const FName& SocketName);
    void EquipWeapon(int32 WeaponIndex);


    // my code
    void PutWeaponToArmory(ASTUBaseWeapon* Weapon);

    USkeletalMeshComponent* GetOwnerSkeletalMesh() const;
};
