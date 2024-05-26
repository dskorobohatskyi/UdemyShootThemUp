// Shoot Them Up Game. All Rights Reserved

#include "Player/Components/STUHealthComponent.h"

#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"

#include "TimerManager.h"
#include "Engine/World.h"

#include "Camera/CameraShakeBase.h"

DEFINE_LOG_CATEGORY_STATIC(STUHealthComponentLog, Display, All);

USTUHealthComponent::USTUHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool USTUHealthComponent::IsDead() const
{
    return FMath::IsNearlyZero(GetHealth()) || FMath::IsNegativeOrNegativeZero(GetHealth());
}

bool USTUHealthComponent::HasFullHP() const
{
    return FMath::IsNearlyEqual(GetHealth(), MaxHealth);
}

bool USTUHealthComponent::TryToAddHP(float HPIncrease)
{
    if (IsDead() || HasFullHP())
    {
        return false;
    }

    Heal(HPIncrease);
    return true;
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

    PlayCameraShake();
}

void USTUHealthComponent::TryToHeal()
{
    if (HasFullHP())
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

void USTUHealthComponent::HealToFull()
{
    UpdateHealthSafe(MaxHealth);
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
    const float UpdatedHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    const float HealthDelta = UpdatedHealth - GetHealth();
    Health = UpdatedHealth;

    // Negative delta indicates recieved damage.
    OnHealthChanged.Broadcast(Health, HealthDelta);
}

void USTUHealthComponent::PlayCameraShake()
{
    if (IsDead())
    {
        return;
    }

    const auto Player = Cast<APawn>(GetOwner());
    if (!Player)
    {
        return;
    }

    const auto Controller = Player->GetController<APlayerController>();
    if (!Controller || !Controller->PlayerCameraManager)
    {
        return;
    }

    Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}
