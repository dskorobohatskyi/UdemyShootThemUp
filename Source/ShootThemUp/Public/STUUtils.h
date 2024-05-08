#pragma once
#include "GameFramework/Pawn.h"

class STUUtils
{
public:
    template<typename T> static T* GetSTUPlayerComponent(APawn* PlayerPawn)
    {
        if (!PlayerPawn)
        {
            return nullptr;
        }

        return PlayerPawn->GetComponentByClass<T>();
    }
};