//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Core/VGroup.h"
#include "Verve/Core/VTrack.h"

#include "Verve/Extension/Script/VScriptEvent.h"

#include "console/consoleTypes.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VScriptEvent );
//-----------------------------------------------------------------------------

// Implement the Command Type enum list.
ImplementEnumType( VScriptEventCommandType, "" )
    { VScriptEvent::k_TypeExpression, "EXPRESSION" },
    { VScriptEvent::k_TypeMethod,     "METHOD"     },
EndImplementEnumType;

//-----------------------------------------------------------------------------

VScriptEvent::VScriptEvent( void ) : 
        mCommandType( k_TypeMethod ),
        mCommand( String::EmptyString )
{
    setLabel( "ScriptEvent" );
}

void VScriptEvent::initPersistFields( void )
{
    Parent::initPersistFields();

    addField( "CommandType", TYPEID<eCommandType>(), Offset( mCommandType, VScriptEvent ), "The type of command to be evaluated." );
    addField( "Command",     TypeRealString,         Offset( mCommand,     VScriptEvent ), "The command to be evaluated." );
}

//-----------------------------------------------------------------------------
//
// Callback Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VScriptEvet::onTrigger( pTime, pDelta );
// 
// Execute a method or evaluate a command.
// 
//-----------------------------------------------------------------------------
void VScriptEvent::onTrigger( const S32 &pTime, const S32 &pDelta )
{
    Parent::onTrigger( pTime, pDelta );

    switch ( mCommandType )
    {
        case k_TypeExpression :
            {

                // Evaluate Expression.
                Con::evaluate( mCommand, false, NULL );

            } break;

        case k_TypeMethod :
            {

                SimObject *object = getSceneObject();
                if ( object )
                {
                    // Execute Method.
                    Con::executef( object, mCommand );
                }
                else
                {
                    // Execute Function.
                    Con::executef( mCommand );
                }

            } break;
    }
}