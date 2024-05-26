// Shoot Them Up Game. All Rights Reserved

#include "UI/STUPlayerHUDWidget.h"
#include "Player/Components/STUHealthComponent.h"
#include "Player/Components/STUWeaponComponent.h"

#include "STUUtils.h"

static const FString InvalidValueString(FString(TEXT("-")));


float USTUPlayerHUDWidget::GetHealthPercent() const
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
    if (!HealthComponent)
    {
        return 0.0f;
    }

    return HealthComponent->GetHealthPercent();
}

bool USTUPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
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

bool USTUPlayerHUDWidget::Initialize()
{
    auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
    if (HealthComponent)
    {
        HealthComponent->OnHealthChanged.AddUObject(this, &USTUPlayerHUDWidget::OnHealthChanged);
    }

    return Super::Initialize();
}

void USTUPlayerHUDWidget::OnHealthChanged(float Health, float Delta)
{
    const bool HasReceivedDamage = Delta < 0.f;
    if (HasReceivedDamage)
    {
        OnTakeDamage();
    }
}

bool USTUPlayerHUDWidget::GetCurrentAmmoData(FAmmoData& AmmoData) const
{
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
    if (!WeaponComponent)
    {
        return false;
    }

    return WeaponComponent->GetCurrentAmmoData(AmmoData);
}

bool USTUPlayerHUDWidget::IsPlayerAlive() const
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
    return HealthComponent && !HealthComponent->IsDead();
}

bool USTUPlayerHUDWidget::IsPlayerSpectating() const
{
    const auto Controller = GetOwningPlayer();
    return Controller && Controller->GetStateName() == NAME_Spectating;
}
