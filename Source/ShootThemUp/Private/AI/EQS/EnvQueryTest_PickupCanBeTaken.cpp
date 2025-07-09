// Shoot Them Up Game. All Rights Reserved


#include "AI/EQS/EnvQueryTest_PickupCanBeTaken.h"
#include "Pickups/STUBasePickup.h"

#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"

UEnvQueryTest_PickupCanBeTaken::UEnvQueryTest_PickupCanBeTaken(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    Cost = EEnvTestCost::Low;
    ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();

    SetWorkOnFloatValues(false); // this test requires boolean value, not float ones
}

void UEnvQueryTest_PickupCanBeTaken::RunTest(FEnvQueryInstance& QueryInstance) const
{
    UObject* DataOwner = QueryInstance.Owner.Get();
    BoolValue.BindData(DataOwner, QueryInstance.QueryID);
    bool bWantsBeTakeable = BoolValue.GetValue();

    for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
    {
        AActor* ItemActor = GetItemActor(QueryInstance, It.GetIndex());
        const auto PickupActor = Cast<ASTUBasePickup>(ItemActor);
        if (!PickupActor)
        {
            continue;
        }

        const bool CanBeTaken = PickupActor->CanBeTaken();
        It.SetScore(TestPurpose, FilterType, CanBeTaken, bWantsBeTakeable);
    }
}
