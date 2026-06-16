

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Poolable.generated.h"


class APlayerCharacter;

UINTERFACE(MinimalAPI)
class UPoolable : public UInterface
{
	GENERATED_BODY()
};


class RANDOMFPS_API IPoolable
{
	GENERATED_BODY()

public:
	virtual void Acquire(AActor* NewOwner) = 0;
	virtual void Release() = 0;
	virtual bool IsActive() const = 0;
};
