// Shoot Them Up Game. All Rights Reserved

#include "Player/Components/STUHealthComponent.h"

DEFINE_LOG_CATEGORY_STATIC(STUHealthComponentLog, Display, All);

USTUHealthComponent::USTUHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool USTUHealthComponent::IsDead() const
{
    return FMath::IsNearlyZero(GetHealth()) || FMath::IsNegativeOrNegativeZero(GetHealth());
}

// Called when the game starts
void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    Health = MaxHealth;
    OnHealthChanged.Broadcast(Health);

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
    if (Damage < 0.f)
    {
        UE_LOG(STUHealthComponentLog, Warning, TEXT("OnTakeAnyDamage received negative damage value!"));
        return;
    }

    if (IsDead())
    {
        return;
    }

    Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
    OnHealthChanged.Broadcast(Health);

    check(GetHealth() < MaxHealth);

    if (bIsAutoHealEnabled)
    {
        StopAutoHealTimer();
    }

    if (IsDead())
    {
        OnDeath.Broadcast();
    }
    else
    {
        if (bIsAutoHealEnabled)
        {
            StartAutoHealTimer();
        }
    }
}

void USTUHealthComponent::TryToHeal()
{
    if (FMath::IsNearlyEqual(GetHealth(), MaxHealth))
    {
        // No need to heal here
        StopAutoHealTimer();
        return;
    }

    Heal(HealingPoints);
}

void USTUHealthComponent::Heal(float HP)
{
    Health = FMath::Clamp(Health + HP, 0.0f, MaxHealth);

    OnHealthChanged.Broadcast(Health);
}

void USTUHealthComponent::StartAutoHealTimer()
{
    GetWorld()->GetTimerManager().SetTimer(AutoHealTimerHandle, this, &USTUHealthComponent::TryToHeal,
                                           HealIntervalInSec, true, HealDelayInSec);
}

void USTUHealthComponent::StopAutoHealTimer()
{
    if (AutoHealTimerHandle.IsValid())
    {
        GetWorld()->GetTimerManager().ClearTimer(AutoHealTimerHandle);
        AutoHealTimerHandle.Invalidate();
    }
}
