// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "STU_EquipFinishedAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquipFinishedDelegate, USkeletalMeshComponent*);

/**
 *
 */
UCLASS() class SHOOTTHEMUP_API USTU_EquipFinishedAnimNotify : public UAnimNotify
{
    GENERATED_BODY()

public:
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                        const FAnimNotifyEventReference& EventReference) override;

    FOnEquipFinishedDelegate OnEquipFinished;
};
