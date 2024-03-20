// Shoot Them Up Game. All Rights Reserved

#include "Player/STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"

#include "Player/Components/STUCharacterMovementComponent.h"
#include "Player/Components/STUHealthComponent.h"

DEFINE_LOG_CATEGORY_STATIC(ASTUBaseCharacterLog, Display, All);

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(CharacterMovementComponentName))
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need
    // it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    // logical component, no need to attach to root component
    HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("TextRenderComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponent);
    check(HealthTextComponent);
    check(GetCharacterMovement());

    HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnCharacterDeath);
    HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);

    // Initial value setting since Actor components BeginPlay() are called at first, and then Actor itself
    OnHealthChanged(HealthComponent->GetHealth());
}

// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("LookAround", this, &ASTUBaseCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::Jump);
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUBaseCharacter::OnRunningStart);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUBaseCharacter::OnRunningEnd);
}

bool ASTUBaseCharacter::IsRunning() const
{
    return bIsRunningRequested && bIsMovingForward && !GetVelocity().IsNearlyZero();
}

float ASTUBaseCharacter::GetMovementDirection() const
{
    const FVector VelocityNormalVector = GetVelocity().GetSafeNormal();
    if (VelocityNormalVector.IsZero())
    {
        return 0.0f;
    }

    const FVector ForwardNormalVector = GetActorForwardVector();

    const float DotProductValue = FVector::DotProduct(ForwardNormalVector, VelocityNormalVector);
    const FVector CrossProductVector = FVector::CrossProduct(ForwardNormalVector, VelocityNormalVector);

    const float SideCorrectness = FMath::Sign(CrossProductVector.Z);
    const float DegreeBetweenVectors = FMath::RadiansToDegrees(FMath::Acos(DotProductValue) * SideCorrectness);

    return DegreeBetweenVectors;
}

void ASTUBaseCharacter::MoveForward(float AxisValue)
{
    bIsMovingForward = AxisValue > 0.0f;

    if (FMath::IsNearlyZero(AxisValue))
    {
        return;
    }

    AddMovementInput(GetActorForwardVector(), AxisValue);
}

void ASTUBaseCharacter::MoveRight(float AxisValue)
{
    if (FMath::IsNearlyZero(AxisValue))
    {
        return;
    }
    AddMovementInput(GetActorRightVector(), AxisValue);
}

void ASTUBaseCharacter::OnRunningStart()
{
    bIsRunningRequested = true;
}

void ASTUBaseCharacter::OnRunningEnd()
{
    bIsRunningRequested = false;
}

void ASTUBaseCharacter::OnCharacterDeath()
{
    UE_LOG(ASTUBaseCharacterLog, Display, TEXT("Player died!"));

    GetCharacterMovement()->DisableMovement();

    if (DeathAnimMontage)
    {
        PlayAnimMontage(DeathAnimMontage);
    }

    const float LifeSpan = 5.f;
    SetLifeSpan(LifeSpan);

    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
}

void ASTUBaseCharacter::OnHealthChanged(float NewHealth) 
{
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%0.f"), NewHealth)));
}
