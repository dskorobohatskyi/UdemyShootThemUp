// Shoot Them Up Game. All Rights Reserved


#include "AI/STUAICharacter.h"
#include "AI/STUAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjectInitializer) 
    : Super(ObjectInitializer)
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
