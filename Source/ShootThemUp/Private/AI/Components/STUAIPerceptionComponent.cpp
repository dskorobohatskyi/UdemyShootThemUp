// Shoot Them Up Game. All Rights Reserved


#include "AI/Components/STUAIPerceptionComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Player/Components/STUHealthComponent.h"
#include "Perception/AISense_Sight.h"

AActor* USTUAIPerceptionComponent::GetClosestEnemy() const
{
    TArray<AActor*> PerceiveActors;
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceiveActors);

    if (PerceiveActors.Num() == 0)
    {
        return nullptr;
    }

    const auto Controller = Cast<AAIController>(GetOwner());
    if (!Controller)
    {
        return nullptr;
    }

    const auto Pawn = Controller->GetPawn();
    if (!Pawn)
    {
        return nullptr;
    }

    float BestDistance = MAX_FLT;
    AActor* BestPawn = nullptr;

    for (const auto PerceiveActor : PerceiveActors)
    {
        const auto HealthComp = PerceiveActor->FindComponentByClass<USTUHealthComponent>();
        if (HealthComp && !HealthComp->IsDead()) // TODO check if it's enemy or not
        {
            const auto CurrentDistance = (PerceiveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
            if (CurrentDistance < BestDistance)
            {
                BestPawn = PerceiveActor;
            }
            BestDistance = FMath::Min(CurrentDistance, BestDistance);
        }
    }

    return BestPawn;
}
