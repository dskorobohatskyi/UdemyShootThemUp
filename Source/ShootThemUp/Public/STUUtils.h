#pragma once
#include "GameFramework/Pawn.h"

class STUUtils
{
public:
    template<typename T>
    UE_DEPRECATED(5.4, "Use AActor::FindComponentByClass instead. No need to have this one")
    static T* GetSTUPlayerComponent(APawn* PlayerPawn)
    {
        if (!PlayerPawn)
        {
            return nullptr;
        }

        return PlayerPawn->GetComponentByClass<T>();
    }
};