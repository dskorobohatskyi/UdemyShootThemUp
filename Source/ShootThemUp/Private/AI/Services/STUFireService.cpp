// Shoot Them Up Game. All Rights Reserved

#include "AI/Services/STUFireService.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Player/Components/STUWeaponComponent.h"

USTUFireService::USTUFireService()
{
    NodeName = "Fire";
}

void USTUFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    const auto Controller = OwnerComp.GetAIOwner();
    
    const bool bHasAim = Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName);
    
    if (Controller)
    {
        // TODO exit if character is dead
        const auto WeaponComp = Controller->GetPawn()->FindComponentByClass<USTUWeaponComponent>();
        if (WeaponComp)
        {
            // TODO fire starts now even if ai character is still rotating to player
            bHasAim && WeaponComp->CanFire() ? WeaponComp->StartFire() : WeaponComp->StopFire();
        }
    }
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}


