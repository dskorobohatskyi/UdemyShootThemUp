// Shoot Them Up Game. All Rights Reserved


#include "AI/Services/STUChangeWeaponService.h"
#include "AIController.h"

#include "AI/STUAICharacter.h"
#include "Player/Components/STUAIWeaponComponent.h"

USTUChangeWeaponService::USTUChangeWeaponService()
{
    NodeName = TEXT("Change Weapon");
}

void USTUChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    float RandomValue = FMath::FRand();

    const bool bSuitsCondition = RandomValue > 0.0f && RandomValue <= Probability;
    UE_LOG(LogTemp, Display, TEXT("USTUChangeWeaponService::TICK Random: %.2f; allow = %d, interval=%.1f"), RandomValue,
           bSuitsCondition, Interval);
    if (!bSuitsCondition)
    {
        return;
    }

    auto* Controller = OwnerComp.GetAIOwner();
    if (!Controller)
    {
        return;
    }
    auto Character = Cast<ASTUAICharacter>(Controller->GetPawn());
    if (!Character)
    {
        return;
    }

    auto WeaponComp = Character->GetComponentByClass<USTUAIWeaponComponent>();
    if (!WeaponComp)
    {
        return;
    }

    WeaponComp->RequestWeaponSwitch();
}
