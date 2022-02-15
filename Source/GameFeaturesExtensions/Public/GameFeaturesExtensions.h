#pragma once

#include <CoreMinimal.h>
#include <Modules/ModuleManager.h>

class FGameFeaturesExtensionsModule : public IModuleInterface
{
public:
    void StartupModule() override;
    void ShutdownModule() override;
};
