//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Core/Persistence/VPersistence.h"

#include "Verve/Core/VController.h"
#include "Verve/Core/VObject.h"

namespace VPersistence
{
    //-----------------------------------------------------------------------------
    //
    // VController
    //
    //-----------------------------------------------------------------------------

    template <>
    bool write( TiXmlElement *pElement, VController *pObject )
    {
        // Write Properties.
        if ( !writeProperties( pElement, pObject ) )
        {
            return false;
        }

        // Write Data Table.
        if ( !pObject->writeDataTable( pElement ) )
        {
            return false;
        }

        // Write Objects.
        return writeObjects( pElement, pObject );
    }

    template <>
    bool read( TiXmlElement *pElement, VController *pObject )
    {
        // Read Properties.
        if ( !readProperties( pElement, pObject ) )
        {
            // Invalid Properties.
            return false;
        }

        // Read Data Table.
        if ( !pObject->readDataTable( pElement ) )
        {
            // Invalid Data Table.
            return false;
        }

        // Read Objects.
        if ( !readObjects( pElement, pObject ) )
        {
            // Invalid Read.
            return false;
        }

        // Valid Read.
        return true;
    }

    //-----------------------------------------------------------------------------
    //
    // VObject
    //
    //-----------------------------------------------------------------------------

    template <>
    bool write( TiXmlElement *pElement, VObject *pObject )
    {
        // Create Element.
        TiXmlElement *objectElement = new TiXmlElement( "VObject" );
        pElement->LinkEndChild( objectElement );

        // Attributes.
        objectElement->SetAttribute( "Type", pObject->getClassName() );

        // Write Properties.
        if ( !writeProperties( objectElement, pObject ) )
        {
            return false;
        }

        // Write Objects.
        return writeObjects( objectElement, pObject );
    }

    template <>
    bool read( TiXmlElement *pElement, VObject *pObject )
    {
        // Read Properties.
        if ( !readProperties( pElement, pObject ) )
        {
            // Invalid Properties.
            return false;
        }

        // Set Name Unique.
        pObject->setLabelUnique( pObject->getLabel() );

        // Read Objects.
        if ( !readObjects( pElement, pObject ) )
        {
            // Invalid Objects.
            return false;
        }

#ifdef VT_EDITOR
        // Callback.
        Con::executef( pObject, "onRead" );
#endif

        // Valid Read.
        return true;
    }
}