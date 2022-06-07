#pragma once

#include <CoreMinimal.h>
#include <GameFeatureAction.h>

#include "GFEGameFeatureAction_WorldActionBase.generated.h"

// This is a copy / paste from the one you can find in the ValleyOfTheAncient UE5 sample
UCLASS( Abstract )
class GAMEFEATURESEXTENSIONS_API UGFEGameFeatureAction_WorldActionBase : public UGameFeatureAction
{
    GENERATED_BODY()

public:
    void OnGameFeatureActivating() override;
    void OnGameFeatureDeactivating( FGameFeatureDeactivatingContext & context ) override;

private:
    void HandleGameInstanceStart( UGameInstance * game_instance );
    virtual void AddToWorld( const FWorldContext & world_context ) PURE_VIRTUAL( UGFEGameFeatureAction_WorldActionBase::AddToWorld, );

    FDelegateHandle GameInstanceStartHandle;
};
