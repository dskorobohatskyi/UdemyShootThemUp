// Shoot Them Up Game. All Rights Reserved

#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

// Sets default values for this component's properties
USTUWeaponFXComponent::USTUWeaponFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void USTUWeaponFXComponent::BeginPlay()
{
    Super::BeginPlay();
}

void USTUWeaponFXComponent::PlayImpactFX(const FHitResult& Hit)
{
    const FRotator SpawnRotator = GetRotationForFX(Hit);

    auto Effect = DefaultEffect;

    if (Hit.PhysMaterial.IsValid())
    {
        UNiagaraSystem** CorrespondingEffect = EffectsMap.Find(Hit.PhysMaterial.Get());
        if (CorrespondingEffect != nullptr)
        {
            Effect = *CorrespondingEffect;
        }
    }
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Effect, Hit.ImpactPoint, SpawnRotator);
}

FRotator USTUWeaponFXComponent::GetRotationForFX(const FHitResult& Hit) const
{
    const FVector HitNormal = Hit.ImpactNormal.Rotation().Vector();
    const FVector HitStartDirection = (Hit.TraceStart - Hit.TraceEnd);

    const float BlendingCoefToNormal = 0.75f;
    const FVector BlendedVector = HitNormal * BlendingCoefToNormal + HitStartDirection * (1.f - BlendingCoefToNormal);

    return BlendedVector.Rotation();
}
