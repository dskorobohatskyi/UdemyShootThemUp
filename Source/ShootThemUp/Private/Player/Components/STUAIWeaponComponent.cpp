// Shoot Them Up Game. All Rights Reserved


#include "Player/Components/STUAIWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"

void USTUAIWeaponComponent::StartFire()
{
    // TODO, i'd also add the check about the player being alive
    if (!CanFire())
    {
        return;
    }
    
    if (CurrentWeapon->IsAmmoEmpty())
    {
        SwitchToNextWeapon();
    }
    else
    {
        CurrentWeapon->StartFire(); // based from course, CurrentWeapon->StartFire() is called, but I like the idea to reuse parent one
    }
}

void USTUAIWeaponComponent::SwitchToNextWeapon()
{
    if (!CanEquip())
    {
        return;
    }
    // TODO check if no ammo at all

    int32 NextIndex = (CurrentWeaponIndex + 1) % Weapons.Num();

    while (NextIndex != CurrentWeaponIndex)
    {
        if (!Weapons[NextIndex]->IsAmmoEmpty())
        {
            break;
        }
        NextIndex = (NextIndex + 1) % Weapons.Num();
    }

    if (NextIndex != CurrentWeaponIndex)
    {
        CurrentWeapon->StopFire(); // my code
        EquipWeapon(NextIndex);
        UE_LOG(LogTemp, Verbose, TEXT("Switched weapon to %d index"), CurrentWeaponIndex);
    }

}
