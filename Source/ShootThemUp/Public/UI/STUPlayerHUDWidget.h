// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "STUCoreTypes.h"
#include "STUPlayerHUDWidget.generated.h"

class USTUWeaponComponent;
class USTUHealthComponent;
/**
 *
 */
UCLASS()
class SHOOTTHEMUP_API USTUPlayerHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetHealthPercent() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetCurrentAmmoData(FAmmoData& AmmoData) const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool IsPlayerAlive() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool IsPlayerSpectating() const;

    // my code start
    UFUNCTION(BlueprintCallable, Category = "UI")
    FString GetWeaponBulletsAsText() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    FString GetWeaponClipsAsText() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool HasInfiniteClips() const;
    // my code end
private:
    USTUWeaponComponent* GetWeaponComponent() const;
    USTUHealthComponent* GetHealthComponent() const;
};
