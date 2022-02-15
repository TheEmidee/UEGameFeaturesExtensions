#include "GFEGameFeatureAction_WorldActionBase.h"

void UGFEGameFeatureAction_WorldActionBase::OnGameFeatureActivating()
{
    GameInstanceStartHandle = FWorldDelegates::OnStartGameInstance.AddUObject( this, &UGFEGameFeatureAction_WorldActionBase::HandleGameInstanceStart );

    for ( const FWorldContext & world_context : GEngine->GetWorldContexts() )
    {
        AddToWorld( world_context );
    }
}

void UGFEGameFeatureAction_WorldActionBase::OnGameFeatureDeactivating( FGameFeatureDeactivatingContext & context )
{
    FWorldDelegates::OnStartGameInstance.Remove( GameInstanceStartHandle );
}

void UGFEGameFeatureAction_WorldActionBase::HandleGameInstanceStart( UGameInstance * game_instance )
{
    if ( const FWorldContext * world_context = game_instance->GetWorldContext() )
    {
        AddToWorld( *world_context );
    }
}
