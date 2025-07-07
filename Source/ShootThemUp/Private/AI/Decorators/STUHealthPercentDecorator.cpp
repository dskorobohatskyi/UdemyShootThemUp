// Shoot Them Up Game. All Rights Reserved


#include "AI/Decorators/STUHealthPercentDecorator.h"

#include "AIController.h"
#include "Player/Components/STUHealthComponent.h"

USTUHealthPercentDecorator::USTUHealthPercentDecorator()
{
    NodeName = TEXT("Health Percent");
}

bool USTUHealthPercentDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller || !Controller->GetPawn())
        return false;

    const auto HealthComp = Controller->GetPawn()->FindComponentByClass<USTUHealthComponent>();
    if (!HealthComp || HealthComp->IsDead())
        return false;

    return HealthComp->GetHealthPercent() <= HealthPercent;
}

