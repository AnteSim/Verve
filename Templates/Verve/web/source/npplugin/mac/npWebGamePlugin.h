//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#pragma once

#include "../../common/webCommon.h"

class NPWebGamePlugin
{
public:

   NPWebGamePlugin(NPP aInstance);
   ~NPWebGamePlugin();

   NPBool	Open(NPWindow* aWindow);
   void	   Close();
   NPBool	IsOpen();

   NPP	 mInstance;
   bool  mOpen;

   static NPWebGamePlugin* sInstance;
};


