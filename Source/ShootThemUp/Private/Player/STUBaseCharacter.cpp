// Shoot Them Up Game. All Rights Reserved

#include "Player/STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Engine/DamageEvents.h"

#include "Player/Components/STUCharacterMovementComponent.h"
#include "Player/Components/STUHealthComponent.h"
#include "Player/Components/STUWeaponComponent.h"


DEFINE_LOG_CATEGORY_STATIC(STUBaseCharacterLog, Display, All);

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(CharacterMovementComponentName))
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need
    // it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SocketOffset = FVector(0.f, 100.f, 80.f);
    SpringArmComponent->SetupAttachment(GetRootComponent());

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    // logical component, no need to attach to root component
    HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("TextRenderComponent");
    HealthTextComponent->SetOwnerNoSee(true);
    HealthTextComponent->SetupAttachment(GetRootComponent());

    WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent");
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponent);
    check(HealthTextComponent);
    check(GetCharacterMovement());
    check(WeaponComponent);
    check(GetMesh());
    if (!bIsRagdollDeathEnabled)
    {
        check(DeathAnimMontage);
    }


    HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnCharacterDeath);
    HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);

    // Initial value setting since Actor components BeginPlay() are called at first, and then Actor itself
    OnHealthChanged(HealthComponent->GetHealth());

    LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);
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
    PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &USTUWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &USTUWeaponComponent::StopFire);
    PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &USTUWeaponComponent::SwitchToNextWeapon);
    PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &USTUWeaponComponent::Reload);
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
    UE_LOG(STUBaseCharacterLog, Display, TEXT("Player died!"));

    GetCharacterMovement()->DisableMovement();

    if (!bIsRagdollDeathEnabled)
    {
        PlayAnimMontage(DeathAnimMontage);
    }
    else
    {
        GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        GetMesh()->SetSimulatePhysics(true);
    }

    SetLifeSpan(LifeSpanOnDeath);

    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    WeaponComponent->StopFire();
}

void ASTUBaseCharacter::OnHealthChanged(float NewHealth)
{
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%0.f"), NewHealth)));
}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
    const float LandedVelocity = -GetVelocity().Z;
    UE_LOG(STUBaseCharacterLog, Verbose, TEXT("Landed velocity for character [%s] after falling = %.2f"), *GetName(),
           LandedVelocity)

    // player doesn't reach minimal height to be damaged from falling
    if (LandedVelocity < LandedDamageVelocityRange.X)
    {
        return;
    }

    const float InterpolatedDamage =
        FMath::GetMappedRangeValueClamped(LandedDamageVelocityRange, LandedDamageRange, LandedVelocity);

    UE_LOG(STUBaseCharacterLog, Verbose, TEXT("Received damage from falling for character [%s] = %.2f"), *GetName(),
           InterpolatedDamage)

    TakeDamage(InterpolatedDamage, FDamageEvent{}, nullptr, nullptr);
}
