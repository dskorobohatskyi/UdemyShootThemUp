#pragma once

#include "Animation/AnimSequenceBase.h"

class AnimUtils
{
public:
    template <typename T> static T* FindNotifyByClassSafe(const UAnimSequenceBase* const Animation)
    {
        if (!Animation)
        {
            return nullptr;
        }

        const auto MontageNotifies = Animation->Notifies;
        for (auto& NotifyEvent : MontageNotifies)
        {
            auto Notify = Cast<T>(NotifyEvent.Notify.Get());
            if (Notify)
            {
                return Notify;
            }
        }
        return nullptr;
    }
};
