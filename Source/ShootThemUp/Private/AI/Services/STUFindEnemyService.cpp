// Shoot Them Up Game. All Rights Reserved


#include "AI/Services/STUFindEnemyService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AI/Components/STUAIPerceptionComponent.h"

USTUFindEnemyService::USTUFindEnemyService()
{
    NodeName = TEXT("Find Enemy");
}

void USTUFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (Blackboard)
    {
        const auto Controller = OwnerComp.GetAIOwner();
        const auto PerceptionComp = Controller->FindComponentByClass<USTUAIPerceptionComponent>();
        if (PerceptionComp)
        {
            Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComp->GetClosestEnemy());
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
