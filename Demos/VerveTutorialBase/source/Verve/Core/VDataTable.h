//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VDATATABLE_H_
#define _VT_VDATATABLE_H_

#ifndef CORE_TDICTIONARY_H
#include "core/util/tDictionary.h"
#endif

#ifndef _CONSOLE_H_
#include "console/console.h"
#endif

#ifndef _DYNAMIC_CONSOLETYPES_H_
#include "console/dynamicTypes.h"
#endif

#ifndef _STRINGTABLE_H_
#include "core/stringTable.h"
#endif

//-----------------------------------------------------------------------------

class VDataTable
{
public:

    enum eDataType
    {
        k_TypeExpression,
        k_TypeStatic,
        k_TypeVariable,

        k_TypeInvalid,
    };

    struct sDataItem
    {
        eDataType    Type;
        String       FieldName;

        sDataItem( void ) :
                Type( k_TypeInvalid ),
                FieldName( String::EmptyString )
        {
            // Void.
        };

        sDataItem( eDataType pType, const String &pFieldName ) :
                Type( pType ),
                FieldName( pFieldName )
        {
            // Void.
        };
    };

    // Enum Lookup.
    static VDataTable::eDataType    getDataTypeEnum( const char *pLabel );
    static const char              *getDataTypeDescription( const VDataTable::eDataType pEnum );

    // Map Type.
    typedef Map<String, sDataItem> VDataMap;

public:

    VDataMap    mDataMap;

public:

    VDataTable( void );
    ~VDataTable( void );

    // Data.

    void        insert( eDataType pType, const String &pFieldName );
    void        clear( const String &pFieldName );
    void        clear( void );

    // Reference.

    S32         getCount( void );
    bool        getItem( const S32 &pIndex, sDataItem *pDataItem = NULL );
    bool        getItem( const String &pFieldName, sDataItem *pDataItem = NULL );

    bool        getValue( SimObject *pObject, const String &pFieldName, String &pValue );
};

//-----------------------------------------------------------------------------

// Define Types.
typedef VDataTable::eDataType VDataTableDataType;

// Declare Enum Types.
DefineEnumType( VDataTableDataType );

//-----------------------------------------------------------------------------

#endif // _VT_VDATATABLE_H_