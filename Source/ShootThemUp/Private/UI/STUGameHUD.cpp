// Shoot Them Up Game. All Rights Reserved

#include "UI/STUGameHUD.h"
#include "Engine/Canvas.h"

void ASTUGameHUD::DrawHUD()
{

    Super::DrawHUD();

    const FVector2f ScreenCenter(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);

    const FLinearColor LineColor = FLinearColor::Gray;
    const float HalfAimSize = 8.f;
    const float Thickness = 2.f;

    DrawLine(ScreenCenter.X - HalfAimSize, ScreenCenter.Y, ScreenCenter.X + HalfAimSize, ScreenCenter.Y, LineColor,
             Thickness);
    DrawLine(ScreenCenter.X, ScreenCenter.Y - HalfAimSize, ScreenCenter.X, ScreenCenter.Y + HalfAimSize, LineColor,
             Thickness);
}
