// Shoot Them Up Game. All Rights Reserved

#include "Animations/STU_EquipFinishedAnimNotify.h"

void USTU_EquipFinishedAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          const FAnimNotifyEventReference& EventReference)
{
    Super::Notify(MeshComp, Animation, EventReference);
    OnEquipFinished.Broadcast(MeshComp);
}
