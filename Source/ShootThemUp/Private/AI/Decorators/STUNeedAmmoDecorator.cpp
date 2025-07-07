// Shoot Them Up Game. All Rights Reserved


#include "AI/Decorators/STUNeedAmmoDecorator.h"

#include "AIController.h"
#include "Player/Components/STUWeaponComponent.h"

USTUNeedAmmoDecorator::USTUNeedAmmoDecorator()
{
    NodeName = TEXT("Needs Ammo");
}

bool USTUNeedAmmoDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller || !Controller->GetPawn())
        return false;

    const auto WeaponComp = Controller->GetPawn()->FindComponentByClass<USTUWeaponComponent>();
    if (!WeaponComp)
        return false;

    return !WeaponComp->IsSpecifiedAmmoFull(WeaponTypeToCheck);
}
