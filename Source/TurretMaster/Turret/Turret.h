// CG Spectrum 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurretMaster/Public/Interfaces/FPInteractInterface.h"
#include "Components/BoxComponent.h"
#include "Turret.generated.h"

class ATurretProjectile;

UCLASS()
class TURRETMASTER_API ATurret : public AActor, public IFPInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//virtual void Interact_Implementation() override;
	//void Interact();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Fire() const;

	UFUNCTION(BlueprintCallable)
	void SetYaw(float TargetYaw) const;
	
	UFUNCTION(BlueprintCallable)
	void SetPitch(float TargetPitch) const;

protected:
	// Components
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> BaseMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USceneComponent> RotationPoint;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> ArmMesh;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> CannonMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USceneComponent> CentreMuzzle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> BoxCollider;
	
	// Variables
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATurretProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	float TurnSpeed = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<AActor>> TargetInRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> FirstContactArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> SecondContactArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector DistanceToProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector DistanceToRangeLimit;
	
};
