// Shoot Them Up Game. All Rights Reserved


#include "AI/Tasks/BTTNextLocationTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"

UBTTNextLocationTask::UBTTNextLocationTask()
{
    NodeName = TEXT("Next location");
}

EBTNodeResult::Type UBTTNextLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();

    if (!Controller || !Blackboard)
    {
        return EBTNodeResult::Failed;
    }

    const auto Pawn = Controller->GetPawn();
    if (!Pawn)
    {
        return EBTNodeResult::Failed;
    }

    const auto NavSys = UNavigationSystemV1::GetCurrent(Pawn);
    if (!NavSys)
    {
        return EBTNodeResult::Failed;
    }

    FVector Location = Pawn->GetActorLocation();
    if (!SelfCenter)
    {
        auto CenterActor = Cast<AActor>(Blackboard->GetValueAsObject(CenterActorKey.SelectedKeyName));
        if (!CenterActor)
        {
            return EBTNodeResult::Failed;
        }
        Location = CenterActor->GetActorLocation();
    }

    FNavLocation NavLocation;
    const bool Found = NavSys->GetRandomReachablePointInRadius(Location, Radius, NavLocation);

    if (Found)
    {
        Location = NavLocation.Location;
    }
    //else
    //{
    //    return EBTNodeResult::Failed;
    //}

    Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, Location);

    return EBTNodeResult::Succeeded;
}
