// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "STUCoreTypes.h"
#include "STUPlayerHUDWidget.generated.h"

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

    virtual bool Initialize() override;

    UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void OnTakeDamage();

private:
    void OnHealthChanged(float Health, float Delta);
};
