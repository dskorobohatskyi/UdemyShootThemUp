// Shoot Them Up Game. All Rights Reserved

#include "UI/STUGameHUD.h"
#include "Engine/Canvas.h"

#include "Blueprint/UserWidget.h"

#include "UI/STUPlayerHUDWidget.h"

void ASTUGameHUD::DrawHUD()
{
    Super::DrawHUD();
}

void ASTUGameHUD::BeginPlay()
{
    Super::BeginPlay();

    auto PlayerWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass.Get());
    if (PlayerWidget)
    {
        PlayerWidget->AddToViewport();
    }
}
