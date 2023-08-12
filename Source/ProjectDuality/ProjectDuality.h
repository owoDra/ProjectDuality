// Copyright (C) 2023 owoDra

#pragma once

#include "Modules/ModuleManager.h"

class FDualityGameModule : public FDefaultGameModuleImpl
{
public:
	/**
	 * FDefaultGameModuleImpl から継承 
	 */
	virtual void StartupModule() override;

	/**
	 * FDefaultGameModuleImpl から継承
	 */
	virtual void ShutdownModule() override;
};
