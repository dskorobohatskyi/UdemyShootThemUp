// Shoot Them Up Game. All Rights Reserved


#include "AI/STUAICharacter.h"
#include "AI/STUAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/Components/STUAIWeaponComponent.h"

#include "BrainComponent.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjectInitializer) 
    : Super(ObjectInitializer.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComponent"))
{
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = ASTUAIController::StaticClass();

    bUseControllerRotationYaw = false;
    if (auto* MovementComp = GetCharacterMovement())
    {
        MovementComp->bUseControllerDesiredRotation = true;
        MovementComp->RotationRate = FRotator(0, 200.f, 0);

    }
}

void ASTUAICharacter::OnCharacterDeath()
{
    Super::OnCharacterDeath();

    auto AIController = Cast<ASTUAIController>(Controller);
    if (AIController)
    {
        if (AIController->BrainComponent)
        {
            AIController->BrainComponent->Cleanup();
        }
    }
}
