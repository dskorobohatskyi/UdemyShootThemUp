// Shoot Them Up Game. All Rights Reserved

#include "Player/Components/STUHealthComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"

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

    check(MaxHealth > 0.f);

    UpdateHealthSafe(MaxHealth);

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

    UpdateHealthSafe(Health - Damage);

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

void USTUHealthComponent::Heal(float HPIncrease)
{
    UpdateHealthSafe(Health + HPIncrease);
}

void USTUHealthComponent::StartAutoHealTimer()
{
    if (auto* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(AutoHealTimerHandle, this, &USTUHealthComponent::TryToHeal, HealIntervalInSec,
                                          true, HealDelayInSec);
    }
}

void USTUHealthComponent::StopAutoHealTimer()
{
    if (AutoHealTimerHandle.IsValid())
    {
        GetWorld()->GetTimerManager().ClearTimer(AutoHealTimerHandle);
        AutoHealTimerHandle.Invalidate();
    }
}

void USTUHealthComponent::UpdateHealthSafe(float NewHealth)
{
    Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(Health);
}
