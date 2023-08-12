// Copyright (C) 2023 owoDra

#pragma once

#include "Modules/ModuleManager.h"

class FDualityCoreModule : public IModuleInterface
{
public:
	/**
	 * IModuleInterface から継承
	 */
	virtual void StartupModule() override;

	/**
	 * IModuleInterface から継承
	 */
	virtual void ShutdownModule() override;
};
