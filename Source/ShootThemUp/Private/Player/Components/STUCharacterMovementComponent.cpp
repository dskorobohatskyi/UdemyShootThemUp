// Shoot Them Up Game. All Rights Reserved


#include "Player/Components/STUCharacterMovementComponent.h"
#include "Player/STUBaseCharacter.h"

float USTUCharacterMovementComponent::GetMaxSpeed() const
{
    const float WalkMaxSpeed = Super::GetMaxSpeed();

    ASTUBaseCharacter* BaseCharacter = Cast<ASTUBaseCharacter>(GetPawnOwner());
    return (BaseCharacter && BaseCharacter->IsRunning()) ? WalkMaxSpeed * RunCoeficientBoost : WalkMaxSpeed;
}
