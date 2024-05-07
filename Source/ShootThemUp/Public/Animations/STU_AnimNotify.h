// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "STU_AnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifyTriggeredDelegate, USkeletalMeshComponent*);

/**
 *
 */
UCLASS()
class SHOOTTHEMUP_API USTU_AnimNotify : public UAnimNotify
{
    GENERATED_BODY()

public:
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                        const FAnimNotifyEventReference& EventReference) override;

    FOnNotifyTriggeredDelegate OnNotifyTriggered;
};
