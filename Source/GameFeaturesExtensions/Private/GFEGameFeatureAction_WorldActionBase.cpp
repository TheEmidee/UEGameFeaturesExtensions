#include "GFEGameFeatureAction_WorldActionBase.h"

#include "GameFeaturesSubsystem.h"

void UGFEGameFeatureAction_WorldActionBase::OnGameFeatureActivating( FGameFeatureActivatingContext & context )
{
    GameInstanceStartHandles.FindOrAdd( context ) = FWorldDelegates::OnStartGameInstance.AddUObject( this, &ThisClass::HandleGameInstanceStart, FGameFeatureStateChangeContext( context ) );

    for ( const auto & world_context : GEngine->GetWorldContexts() )
    {
        if ( context.ShouldApplyToWorldContext( world_context ) )
        {
            AddToWorld( world_context, context );
        }
    }
}

void UGFEGameFeatureAction_WorldActionBase::OnGameFeatureDeactivating( FGameFeatureDeactivatingContext & context )
{
    if ( const auto * found_handle = GameInstanceStartHandles.Find( context ); ensure( found_handle ) )
    {
        FWorldDelegates::OnStartGameInstance.Remove( *found_handle );
    }
}

void UGFEGameFeatureAction_WorldActionBase::HandleGameInstanceStart( UGameInstance * game_instance, FGameFeatureStateChangeContext change_context )
{
    if ( const FWorldContext * world_context = game_instance->GetWorldContext() )
    {
        if ( change_context.ShouldApplyToWorldContext( *world_context ) )
        {
            AddToWorld( *world_context, change_context );
        }
    }
}
