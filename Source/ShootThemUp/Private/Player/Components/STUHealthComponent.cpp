// Shoot Them Up Game. All Rights Reserved

#include "Player/Components/STUHealthComponent.h"

USTUHealthComponent::USTUHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    Health = MaxHealth;

    AActor* ComponentOwner = GetOwner();

    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
    }
}

void USTUHealthComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    AActor* ComponentOwner = GetOwner();

    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.RemoveDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
    }
}

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                          AController* InstigatedBy, AActor* DamageCauser)
{
    Health -= Damage;
    // TODO add logs
}
