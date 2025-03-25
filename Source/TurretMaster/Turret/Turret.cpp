// CG Spectrum 2025

// This file's header
#include "Turret.h"
#include "TurretProjectile.h"

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;
	
	RotationPoint = CreateDefaultSubobject<USceneComponent>("RotationPoint");
	RotationPoint->SetupAttachment(RootComponent);
	
	ArmMesh = CreateDefaultSubobject<UStaticMeshComponent>("ArmMesh");
	ArmMesh->SetupAttachment(RotationPoint);
	
	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>("CannonMesh");
	CannonMesh->SetupAttachment(ArmMesh);
	
	CentreMuzzle = CreateDefaultSubobject<USceneComponent>("CentreMuzzle");
	CentreMuzzle->SetupAttachment(CannonMesh);

	BoxCollider = CreateDefaultSubobject<UBoxComponent>("BoxCollider");
	BoxCollider->SetupAttachment(RootComponent);
	
	DistanceToProjectile = {0.0f, 0.0f, 0.0f};
	DistanceToRangeLimit = {3280.0f, 0.0f, 0.0f};
	DistanceFromCentreMuzzle = {3280.0f, 0.0f, 0.0f};
	TargetPosition = false;
		
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();

	DistanceToRangeLimit = DistanceToRangeLimit + CentreMuzzle->GetComponentLocation();
	DistanceFromCentreMuzzle = DistanceFromCentreMuzzle + CentreMuzzle->GetComponentLocation();
	
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetInRange.Num() > 0 && (TargetInRange[0]->GetActorLocation().Z < GetActorLocation().Z || TargetInRange[0]->GetActorLocation().Y <= 0 || TargetInRange[0]->GetActorLocation().Y >= 2200))
	{
		TargetInRange.RemoveAt(0);
		FirstContactArray.RemoveAt(0);
	}
	if (TargetInRange.Num() > 0)
	{

		DistanceToProjectile = TargetInRange[0]->GetActorLocation();
		if (DistanceToProjectile.Y <= DistanceToRangeLimit.Y)
		{
			TargetPosition = true;
		} else
		{
			TargetPosition = false;
		}
		
	} else
	{
		DistanceToProjectile = DistanceToRangeLimit;
	}
	DrawDebugLine(GetWorld(), CentreMuzzle->GetComponentLocation(), DistanceToRangeLimit, FColor::Green, false);
	DrawDebugLine(GetWorld(), CentreMuzzle->GetComponentLocation(), DistanceToProjectile, FColor::Silver, false);
	
	// (TODO) Calculate impact point
	FVector ImpactPoint = {0.0f, 0.0f, 0.0f};
	if (TargetInRange.Num() > 0)
	{
		float VertexH = FirstContactArray[0].X;
		float VertexK = FirstContactArray[0].Z;
		float FocusA = DistanceToProjectile.Z / (FMath::Square(DistanceToProjectile.X - VertexH) + VertexK);
		
		float tempVelocity = (DistanceToProjectile.X - FirstContactArray[0].X) / DeltaTime;
		float tempAcceleration = FocusA(FMath::Square((DistanceToProjectile.X - VertexH))) + VertexK;
		ImpactPoint.X = FirstContactArray[0].X + tempVelocity * (2 * DeltaTime) + 1/2 * (tempAcceleration * FMath::Square(2 * DeltaTime));
		
		DrawDebugLine(GetWorld(), FirstContactArray[0], ImpactPoint, FColor::Orange, false);
	}
	
	// (TODO) Set yaw and pitch
	// x = roll; y = pitch; z = yaw

	float PitchAngle = FMath::Asin(abs(DistanceToProjectile.Z - DistanceToRangeLimit.Z) / sqrt(DistanceToProjectile.Dot(DistanceToProjectile)));
	PitchAngle = FMath::RadiansToDegrees(PitchAngle);
	
	float absVectorYaw = sqrt(DistanceToProjectile.Dot(DistanceToProjectile)) * sqrt(DistanceToRangeLimit.Dot(DistanceToRangeLimit));
	float YawAngle = FMath::Acos(DistanceToProjectile.Dot(DistanceToRangeLimit) / absVectorYaw);
	YawAngle = FMath::RadiansToDegrees(YawAngle);
	
	if (TargetPosition)
	{
		YawAngle *= -1;
		
	}
	SetPitch(PitchAngle);
	SetYaw(YawAngle);

	// (TODO) Check muzzle is pointed at impact point
	FVector StartLocation = CentreMuzzle->GetComponentLocation();
	FVector ForwardVector = CentreMuzzle->GetForwardVector();
	FVector EndLocation = StartLocation + ForwardVector * 3280;
	
	DrawDebugLine(GetWorld(), CentreMuzzle->GetComponentLocation(), EndLocation, FColor::Blue, false);
	if (GEngine)
	{
		FString TheFloatStr = DistanceFromCentreMuzzle.ToString();
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, *TheFloatStr);
	}
	
	// (TODO) If it is, FIRE!

	
}

void ATurret::Fire() const
{
	// Spawn Parameters
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// Spawn the turret projectile
	TObjectPtr<AActor> SpawnedProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, CentreMuzzle->GetComponentLocation(), CentreMuzzle->GetComponentRotation(), SpawnParams);
	if (SpawnedProjectile)
	{
		UE_LOG(LogTemp, Warning, TEXT("Turret Projectile spawned."));
	}
}

void ATurret::SetYaw(float TargetYaw) const
{
	FRotator CurrentRotation = RotationPoint->GetComponentRotation();
	float NewYaw = FMath::FInterpTo(CurrentRotation.Yaw, TargetYaw, GetWorld()->GetDeltaSeconds(), TurnSpeed);
	RotationPoint->SetWorldRotation(FRotator(CurrentRotation.Pitch, NewYaw, CurrentRotation.Roll));
	
}

void ATurret::SetPitch(float TargetPitch) const
{
	FRotator CurrentRotation = RotationPoint->GetComponentRotation();
	float NewPitch = FMath::FInterpTo(CurrentRotation.Pitch, TargetPitch, GetWorld()->GetDeltaSeconds(), TurnSpeed);
	RotationPoint->SetWorldRotation(FRotator(NewPitch, CurrentRotation.Yaw, CurrentRotation.Roll));

}

