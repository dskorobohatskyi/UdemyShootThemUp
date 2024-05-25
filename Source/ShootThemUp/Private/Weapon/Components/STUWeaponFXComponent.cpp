// Shoot Them Up Game. All Rights Reserved

#include "Weapon/Components/STUWeaponFXComponent.h"
#include "Components/DecalComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

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

    FImpactData* ImpactData = &DefaultImpactData; // I used ptr here to prevent copying

    if (Hit.PhysMaterial.IsValid())
    {
        FImpactData* CorrespondingImpactData = ImpactDataMap.Find(Hit.PhysMaterial.Get());
        if (CorrespondingImpactData != nullptr)
        {
            ImpactData = CorrespondingImpactData;
        }
    }

    // spawn niagara effect at first
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),                //
                                                   ImpactData->NiagaraEffect, //
                                                   Hit.ImpactPoint,           //
                                                   SpawnRotator               //
    );

    // decal spawn
    // DecalMaterial's nullptr check is done within SpawnDecalAtLocation
    // I expected to have nullptr set for head and body phys materials, since shot decal is likely suitable for ground
    UDecalComponent* DecalComp = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),                     //
                                                                        ImpactData->DecalData.Material, //
                                                                        ImpactData->DecalData.Size,     //
                                                                        Hit.ImpactPoint,                //
                                                                        Hit.ImpactNormal.Rotation()     //
    );

    if (DecalComp)
    {
        DecalComp->SetFadeOut(ImpactData->DecalData.LifeTime,    //
                              ImpactData->DecalData.FadeOutTime, //
                              true                               //
        );
    }
}

FRotator USTUWeaponFXComponent::GetRotationForFX(const FHitResult& Hit) const
{
    const FVector HitNormal = Hit.ImpactNormal.Rotation().Vector();
    const FVector HitStartDirection = (Hit.TraceStart - Hit.TraceEnd);

    const float BlendingCoefToNormal = 0.75f;
    const FVector BlendedVector = HitNormal * BlendingCoefToNormal + HitStartDirection * (1.f - BlendingCoefToNormal);

    return BlendedVector.Rotation();
}
