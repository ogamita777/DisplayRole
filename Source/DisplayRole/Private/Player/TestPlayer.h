#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TestPlayer.generated.h"

UCLASS()
class ATestPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	ATestPlayer();

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void InitializeInputBindings();

	void MoveForward(float value);
	void MoveRight(float value);

	void DebugDisplay();
};
