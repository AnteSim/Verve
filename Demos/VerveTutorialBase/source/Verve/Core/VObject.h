//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VOBJECT_H_
#define _VT_VOBJECT_H_

#ifndef _VT_VERVECONFIG_H_
#include "Verve/VerveConfig.h"
#endif

#ifdef VT_EDITOR
    #ifndef _SIMOBJECT_H_
    #include "console/simObject.h"
    #endif

    #define VObjectRep    SimObject
#else
    #ifndef _CONSOLEOBJECT_H_
    #include "console/consoleObject.h"
    #endif

    #define VObjectRep    ConsoleObject
#endif

#ifndef _VT_VTREENODE_H_
#include "Verve/Core/VTreeNode.h"
#endif

#ifndef TINYXML_INCLUDED
#include "tinyxml/tinyxml.h"
#endif

//-----------------------------------------------------------------------------
class VController;
//-----------------------------------------------------------------------------

class VObject : public VObjectRep,
                public VTreeNode
{
    typedef VObjectRep Parent;

protected:

    VController           *mController;

    String                 mLabel;
    bool                   mEnabled;
    
public:

    VObject( void );
    virtual ~VObject( void );

    static  void           initPersistFields( void );

    // Reference Methods.

    VObject *getObject( const String &pLabel );
    template <class T> inline bool getObject( const String &pLabel, T *&pObject )
    {
        // Reference Object.
        pObject = dynamic_cast<T*>( getObject( pLabel ) );

        // Valid?
        return ( pObject != NULL );
    }

    // Console Declaration.

    DECLARE_CONOBJECT( VObject );

public:

    // Property Methods.

    inline VController        *getController( void ) { return mController; };

    inline const String       &getLabel( void ) const { return mLabel; };
    bool                       isEnabled( void );

    bool                       isControllerPlaying( void );
    bool                       isControllerPaused( void );
    bool                       isControllerStopped( void );
    bool                       isControllerPlayingForward( void );
    bool                       isControllerLooping( void );
    S32                        getControllerTime( void );
    F32                        getControllerTimeScale( void );
    S32                        getControllerDuration( void );

    virtual void               setLabel( const String &pLabel );
    void                       setLabelUnique( const String &pLabel );
    inline void                setEnabled( const bool &pEnabled ) { mEnabled = pEnabled; };

    // Callback Methods.

    virtual void               onAttach( void );
    virtual void               onDetach( void );

    // Static Methods.

    static  bool               setEnabled( void *pObject, const char *pArray, const char *pData ) { static_cast<VObject*>( pObject )->setEnabled( dAtob( pData ) ); return false; };
    static  bool               setLabel( void *pObject, const char *pArray, const char *pData )   { static_cast<VObject*>( pObject )->setLabel( pData );            return false; };
};

//-----------------------------------------------------------------------------

#endif // _VT_VOBJECT_H_