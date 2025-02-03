#include "TestPlayer.h"

#include "GameFramework/PlayerInput.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

ATestPlayer::ATestPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ATestPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DisplayDebug();
}

void ATestPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InitializeInputBindings();

	PlayerInputComponent->BindAxis("MoveForward", this, &ATestPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATestPlayer::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ATestPlayer::AddControllerYawInput);
}

void ATestPlayer::InitializeInputBindings()
{
	static bool isBindingsAdded = false;

	if (isBindingsAdded)
	{
		return;
	}

	isBindingsAdded = true;

	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveForward", EKeys::W, 1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveForward", EKeys::S, -1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveRight", EKeys::A, -1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveRight", EKeys::D, 1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Turn", EKeys::MouseX, 1.f));
}

void ATestPlayer::MoveForward(float value)
{
	if ((Controller != nullptr) && (value != 0.0f))
	{
		const FRotator controlSpaceRot = Controller->GetControlRotation();
		AddMovementInput(FRotationMatrix(controlSpaceRot).GetScaledAxis(EAxis::X), value);
	}
}

void ATestPlayer::MoveRight(float value)
{
	if ((Controller != nullptr) && (value != 0.0f))
	{
		const FRotator controlSpaceRot = Controller->GetControlRotation();
		AddMovementInput(FRotationMatrix(controlSpaceRot).GetScaledAxis(EAxis::Y), value);
	}
}

void ATestPlayer::DisplayDebug()
{
	FString debugString = TEXT("");

	// PlayerState
	{
		APlayerState* playerState = GetPlayerState();

		if (playerState)
		{
			debugString += FString::Printf(TEXT("PlayerState->GetPlayerId(): %d\n"), playerState->GetPlayerId());
		}
		else
		{
			debugString += FString::Printf(TEXT("PlayerState: nullptr\n"));
		}
	}

	// Pawn
	{
		debugString += FString::Printf(TEXT("Pawn: %s\n"), *GetActorNameOrLabel());
	}

	// Controller
	{
		if (Controller)
		{
			debugString += FString::Printf(TEXT("Controller: %s\n"), *Controller->GetActorNameOrLabel());
			debugString += FString::Printf(TEXT("Controller->IsLocalController(): %s\n"), Controller->IsLocalController() ? TEXT("true") : TEXT("false"));

			{
				static const TCHAR* STR[] = {
					TEXT("NM_Standalone"),
					TEXT("NM_DedicatedServer"),
					TEXT("NM_ListenServer"),
					TEXT("NM_Client"),
				};

				const ENetMode netMode = Controller->GetNetMode();
				debugString += FString::Printf(TEXT("Controller->GetNetMode(): %s\n"), STR[netMode]);
			}
		}
		else
		{
			debugString += FString::Printf(TEXT("Controller: nullptr\n"));
		}
	}

	// Role
	{
		const ENetRole localRole = GetLocalRole();
		debugString += FString::Printf(TEXT("LocalRole: %s\n"), *UEnum::GetDisplayValueAsText(localRole).ToString());

		const ENetRole remoteRole = GetRemoteRole();
		debugString += FString::Printf(TEXT("RemoteRole: %s\n"), *UEnum::GetDisplayValueAsText(remoteRole).ToString());
	}

	if (UKismetSystemLibrary::IsDedicatedServer(this))
	{
		UE_LOG(LogTemp, Log, TEXT("%s"), *debugString);
	}
	else
	{
		DrawDebugString(GetWorld(), GetActorLocation(), debugString, nullptr, FColor::Black, 0);
	}
}

