// Shoot Them Up Game. All Rights Reserved

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ASTUBaseWeapon::ASTUBaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
    SetRootComponent(SkeletalMesh);
}

// Called when the game starts or when spawned
void ASTUBaseWeapon::BeginPlay()
{
    Super::BeginPlay();
}
