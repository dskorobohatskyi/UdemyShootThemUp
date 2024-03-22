// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath); // C++ only delegate, no exposing to BPs
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    USTUHealthComponent();

    float GetHealth() const { return Health; }

    UFUNCTION(BlueprintCallable, Category = "Health")
    bool IsDead() const;

    FOnDeath OnDeath;
    FOnHealthChanged OnHealthChanged;

    UFUNCTION(BlueprintCallable, Category = "Health")
    void Heal(float HP);

protected:
    // Called when the game starts
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = 0.f, ClampMax = 1000.f))
    float MaxHealth = 100.f;

    // In the future can be switchable in runtime also
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
    bool bIsAutoHealEnabled = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = 0.f, ClampMax = 60.f))
    float HealDelayInSec = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = 0.f, ClampMax = 60.f))
    float HealIntervalInSec = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = 0.f, ClampMax = 500.f))
    float HealingPoints = 1.f;

private:
    float Health = 0.f;
    FTimerHandle AutoHealTimerHandle;

    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
                         class AController* InstigatedBy, AActor* DamageCauser);

    void TryToHeal();
    void StartAutoHealTimer();
    void StopAutoHealTimer();
};
