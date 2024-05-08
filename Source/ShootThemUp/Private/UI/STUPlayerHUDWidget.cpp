// Shoot Them Up Game. All Rights Reserved

#include "UI/STUPlayerHUDWidget.h"
#include "Player/Components/STUHealthComponent.h"
#include "Player/Components/STUWeaponComponent.h"


static const FString InvalidValueString(FString(TEXT("-")));


float USTUPlayerHUDWidget::GetHealthPercent() const
{
    APawn* OwningPlayer = GetOwningPlayerPawn();
    if (!OwningPlayer)
    {
        return 0.0f;
    }

    auto HealthComponent = OwningPlayer->GetComponentByClass<USTUHealthComponent>();
    if (!HealthComponent)
    {
        return 0.0f;
    }

    return HealthComponent->GetHealthPercent();
}

bool USTUPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    const auto WeaponComponent = GetWeaponComponent();
    if (!WeaponComponent)
    {
        return false;
    }

    return WeaponComponent->GetCurrentWeaponUIData(UIData);
}

FString USTUPlayerHUDWidget::GetWeaponBulletsAsText() const
{
    FAmmoData CurrentAmmo;
    const bool bSuccess = GetCurrentAmmoData(CurrentAmmo);
    return bSuccess ? FString::FromInt(CurrentAmmo.Bullets) : InvalidValueString;
}

FString USTUPlayerHUDWidget::GetWeaponClipsAsText() const
{
    FAmmoData CurrentAmmo;
    const bool bSuccess = GetCurrentAmmoData(CurrentAmmo);
    const FString ClipsText = CurrentAmmo.bIsInfinite ? FString(TEXT("\u221e")) : FString::FromInt(CurrentAmmo.Clips);
    return bSuccess ? ClipsText : InvalidValueString;
}

bool USTUPlayerHUDWidget::HasInfiniteClips() const
{
    FAmmoData CurrentAmmo;
    const bool bSuccess = GetCurrentAmmoData(CurrentAmmo);
    return bSuccess ? CurrentAmmo.bIsInfinite : false;
}

bool USTUPlayerHUDWidget::GetCurrentAmmoData(FAmmoData& AmmoData) const
{
    const auto WeaponComponent = GetWeaponComponent();
    if (!WeaponComponent)
    {
        return false;
    }

    return WeaponComponent->GetCurrentAmmoData(AmmoData);
}

USTUWeaponComponent* USTUPlayerHUDWidget::GetWeaponComponent() const
{
    APawn* OwningPlayer = GetOwningPlayerPawn();
    if (!OwningPlayer)
    {
        return nullptr;
    }

    return OwningPlayer->GetComponentByClass<USTUWeaponComponent>();
}
