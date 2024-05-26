// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUHealthComponent.generated.h"

class UCameraShakeBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    USTUHealthComponent();

    FOnDeathDelegate OnDeath;
    FOnHealthChangedDelegate OnHealthChanged;

    float GetHealth() const { return Health; }

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetHealthPercent() const { return GetHealth() / MaxHealth; }

    UFUNCTION(BlueprintCallable, Category = "Health")
    bool IsDead() const;

    UFUNCTION(BlueprintCallable, Category = "Health")
    bool HasFullHP() const;

    UFUNCTION(BlueprintCallable, Category = "Health")
    bool TryToAddHP(float HPIncrease);

    // my code start
    UFUNCTION(BlueprintCallable, Category = "Health")
    void Heal(float HPIncrease);

    UFUNCTION(BlueprintCallable, Category = "Health")
    void HealToFull();
    // my code end

protected:
    // Called when the game starts
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = 0.f, ClampMax = 1000.f))
    float MaxHealth = 100.f;

    // In the future can be switchable in runtime also
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
    bool bIsAutoHealEnabled = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal",
              meta = (ClampMin = 0.f, ClampMax = 60.f, EditCondition = "bIsAutoHealEnabled"))
    float HealDelayInSec = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal",
              meta = (ClampMin = 0.f, ClampMax = 60.f, EditCondition = "bIsAutoHealEnabled"))
    float HealIntervalInSec = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal",
              meta = (ClampMin = 0.f, ClampMax = 500.f, EditCondition = "bIsAutoHealEnabled"))
    float HealingPoints = 1.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TSubclassOf<UCameraShakeBase> CameraShake;

private:
    float Health = 0.f;
    FTimerHandle AutoHealTimerHandle;

    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
                         class AController* InstigatedBy, AActor* DamageCauser);

    void TryToHeal();
    void StartAutoHealTimer();
    void StopAutoHealTimer();

    void UpdateHealthSafe(float NewHealth);

    // VFX
    void PlayCameraShake();
};
