//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "VPath.h"

#include "console/consoleTypes.h"
#include "core/iTickable.h"
#include "core/stream/bitStream.h"
#include "math/mMathFn.h"
#include "math/mathIO.h"

//-----------------------------------------------------------------------------

// Uncomment this definition to debug the network information.
//#define VPATH_DEBUG_NET

// Uncomment this definition to debug the time step information
//#define VPATH_DEBUG_STEP

//-----------------------------------------------------------------------------

SimObjectPtr<SimSet> VPath::gServerSet         = NULL;

U32                  VPath::gMaxNodeTransmit   = 16;
U32                  VPath::gMaxNodeBits       = 8;
U32                  VPath::gMaxNodeCount      = 1 << gMaxNodeBits;      // 256

U32                  VPath::gMaxObjectTransmit = 4;
U32                  VPath::gMaxObjectBits     = 4;
U32                  VPath::gMaxObjectCount    = 1 << gMaxObjectBits;     // 16

Point3F              VPath::gBezierAxis( 0.f, 1.f, 0.f );
Point3F              VPath::gBezierUp( 0.f, 0.f, 1.f );

//-----------------------------------------------------------------------------

static U32           gPathTypeBits     = getBinLog2( getNextPow2( VPath::k_PathTypeSize ) );
static F32           gBezierInterpStep = 0.0001f;

//-----------------------------------------------------------------------------
// Path Type Table.
//-----------------------------------------------------------------------------

// Implement the Path Type enum list.
ImplementEnumType( VPathType, "" )
    { VPath::k_PathBezier,   "BEZIER" },
    { VPath::k_PathLinear,   "LINEAR" },
EndImplementEnumType;

static VPath::ePathType getPathTypeEnum( const char *pLabel )
{
    VPath::ePathType out;
    if ( !castConsoleTypeFromString( out, pLabel ) )
    {
        // Bah!
        return VPath::k_PathInvalid;
    }

    // Return.
    return out;
}

//-----------------------------------------------------------------------------
IMPLEMENT_CO_NETOBJECT_V1( VPath );
//-----------------------------------------------------------------------------

VPath::VPath( void ) :
        mPathType( k_PathBezier )
{
    // Marker Type.
    mTypeMask = MarkerObjectType;

    // Ghost & Scope.
    mNetFlags.set( Ghostable | ScopeAlways );

    // Process Ticks.
    setProcessTick( true );

    VECTOR_SET_ASSOCIATION( mNodeList );
    VECTOR_SET_ASSOCIATION( mObjectList );
}

VPath::~VPath( void )
{
    // Void.
}

bool VPath::onAdd( void )
{
    if ( !Parent::onAdd() )
    {
        return false;
    }

    // Add to Scene.
    addToScene();

    if ( isServerObject() )
    {
        // Read Fields.
        readFields();

        // Add to Set.
        getServerSet()->addObject( this );   
    }

    return true;
}

void VPath::onDeleteNotify( SimObject *pObject )
{
    // Parent Notify.
    Parent::onDeleteNotify( pObject );

    if ( SceneObject *sceneObject = dynamic_cast<SceneObject*>( pObject ) )
    {
        // Detach Object.
        detachObject( sceneObject );

        // Exit.
        return;
    }

    if ( NetConnection *connection = dynamic_cast<NetConnection*>( pObject ) )
    {
        // Clear Connection References.
        for ( VPathObjectIterator itr = mObjectList.begin(); itr != mObjectList.end(); itr++ )
        {
            // Erase Connection.
            ( *itr )->clearConnection( connection );
        }

        // Exit.
        return;
    }
}

void VPath::onRemove( void )
{
    // Remove From Scene.
    removeFromScene();

    // Clear Everything.
    clear();

    Parent::onRemove();
}

void VPath::initPersistFields( void )
{
    Parent::initPersistFields();

    addProtectedField( "PathType", TYPEID<ePathType>(), Offset( mPathType, VPath ), &setPathType, &defaultProtectedGetFn, "The type of path this is." );
}

SimSet *VPath::getServerSet( void )
{
    if ( !gServerSet )
    {
        gServerSet = new SimSet();
        gServerSet->registerObject( "ServerPathSet" );
        Sim::getRootGroup()->addObject( gServerSet );
    }

    return gServerSet;
}

ConsoleFunction( getServerPathSet, int, 1, 1, "( void )" )
{
    return VPath::getServerSet()->getId();
}

//-----------------------------------------------------------------------------
//
// Editor Methods.
//
//-----------------------------------------------------------------------------

bool VPath::collideBox( const Point3F &pStart, const Point3F &pEnd, RayInfo* pInfo )
{
    if ( mObjBox.isContained( pStart ) )
    {
        pInfo->t        = 0.f;
        pInfo->object   = this;
        pInfo->normal   = VectorF( 0.f, 0.f, 1.f );
        pInfo->material = NULL;

        return true;
    }

    return Parent::collideBox( pStart, pEnd, pInfo );
}

//-----------------------------------------------------------------------------
//
// Update Methods.
//
//-----------------------------------------------------------------------------

F32 VPath::getUpdatePriority( CameraScopeQuery *pFocusObject, U32 pUpdateMask, S32 pUpdateSkips )
{
    if ( mObjectList.size() > 0 )
    {
        for ( VPathObjectIterator itr = mObjectList.begin(); itr != mObjectList.end(); itr++ )
        {
            // Fetch Object.
            VPathObject *pathObject = ( *itr );
            if ( pathObject->isActive() )
            {
                // High Priority.
                return 100.f;
            }
        }
    }

    // Normal Priority.
    return 0.f;
}

void VPath::updateContainer( void )
{
    if ( mNodeList.size() == 0 )
    {
        // Sanity!.
        return;
    }

    // Init Min / Max.
    mObjBox.minExtents = ( mNodeList[0]->getLocalPosition() );
    mObjBox.maxExtents = mObjBox.minExtents;

    for ( VPathNodeIterator itr = mNodeList.begin(); itr != mNodeList.end(); itr++ )
    {
        // Fetch Node.
        VPathNode *node = ( *itr );

        // Node Position.
        const Point3F &nodeLocalPosition = node->getLocalPosition();

        // Update Object Box.
        mObjBox.minExtents.setMin( nodeLocalPosition );
        mObjBox.maxExtents.setMax( nodeLocalPosition );
    }

    // Adjust.
    mObjBox.minExtents -= Point3F( 1.f, 1.f, 1.f );
    mObjBox.maxExtents += Point3F( 1.f, 1.f, 1.f );

    // Reset Box.
    resetWorldBox();
    resetRenderWorldBox();
}

void VPath::updateNodeTransforms( void )
{
    // Fetch Transform Details.
    const MatrixF &pathTransform = getTransform();
    const QuatF   &pathRotation( pathTransform );
    const VectorF &pathScale     = getScale();

    for ( VPathNodeIterator itr = mNodeList.begin(); itr != mNodeList.end(); itr++ )
    {
        // Fetch Node.
        VPathNode *node = ( *itr );

        // Fetch Node Spatials.
        const Point3F &nodePosition = node->getLocalPosition();
        const QuatF   &nodeRotation = node->getLocalRotation();

        // Calculate the new Position.
        Point3F newPosition = nodePosition;
        newPosition.convolve( pathScale );
        pathTransform.mulP( newPosition );

        // Calculate the new Rotation.
        QuatF newRotation;
        newRotation.mul( nodeRotation, pathRotation );

        // Apply.
        node->setWorldPosition( newPosition );
        node->setWorldRotation( newRotation );
    }
}

void VPath::setTransform( const MatrixF &pMatrix )
{
    // Parent Call.
    Parent::setTransform( pMatrix );

    // Update Nodes.
    updateNodeTransforms();

    if ( isServerObject() )
    {
        // Update Path.
        setMaskBits( PathUpdateMask );
    }
}

void VPath::setScale( const VectorF &pScale )
{
    // Parent Call.
    Parent::setScale( pScale );

    // Update Nodes.
    updateNodeTransforms();

    if ( isServerObject() )
    {
        // Update Path.
        setMaskBits( PathUpdateMask );
    }
}

ConsoleMethod( VPath, setPathType, void, 3, 3, "( string pPathType ) - The path type dictates how attached objects move between nodes. There are currently two supported path types, \"BEZIER\" and \"LINEAR\".\n"
                                               "@return No return value." )
{
    // Fetch Enum.
    const VPath::ePathType &type = getPathTypeEnum( argv[2] );

    // Update.
    object->setPathType( type );
}

void VPath::setPathType( const ePathType &pType )
{
    // Apply Value.
    mPathType = pType;

    // Calculate Path.
    calculatePath();

    if ( isServerObject() )
    {
        // Update Path.
        setMaskBits( PathUpdateMask );
    }
}

bool VPath::setPathType( void *pObject, const char *pArray, const char *pData )
{
    // Apply Type.
    static_cast<VPath*>( pObject )->setPathType( getPathTypeEnum( pData ) );
    return false;
}

//-----------------------------------------------------------------------------
//
// Mounting Methods.
//
//-----------------------------------------------------------------------------

bool VPath::isMountIndex( const U32 &pIndex )
{
    for ( SceneObject *itr = getMountList(); itr != NULL; itr = itr->getMountLink() )
    {
        if ( itr->getMountNode() == pIndex )
        {
            // Yes.
            return true;
        }
    }

    // No.
    return false;
}

U32 VPath::getAvailableMountIndex( void )
{
    U32 i = 0;
    while( isMountIndex( i ) )
    {
        // Increment.
        i++;
    }

    // Return Index.
    return i;
}

void VPath::mountObject( SceneObject *pObject, S32 pIndex, const MatrixF &pTransform )
{
#ifdef VPATH_DEBUG_NET
    Con::printf( "VPath::mountObject() %d | %d, IsAttached %d", isServerObject(), pObject->getId(), isObjectAttached( pObject ) );
#endif

    // Attached?
    if ( !isObjectAttached( pObject ) )
    {
        if ( isServerObject() )
        {
            // Shouldn't Use this Method.
            Con::warnf( "VPath::mountObject() - Use 'attachObject' instead." );
        }

        // Not Attached.
        return;
    }

    // Parent Call.
    Parent::mountObject( pObject, pIndex, pTransform );

    // Clear the mounted mask.
    // Note: This is so that we send the mounting information via the VPath
    //       packets instead of letting T3D handle it.
    pObject->clearMaskBits( SceneObject::MountedMask );
}

void VPath::unmountObject( SceneObject *pObject )
{
    // Fetch Path Object.
    VPathObject *pathObject = getPathObject( pObject );

#ifdef VPATH_DEBUG_NET
    Con::printf( "VPath::unmountObject() %d | %d, IsAttached %d", isServerObject(), pObject->getId(), pathObject != NULL );
#endif

    // Valid?
    if ( !pathObject || pObject->getObjectMount() != this ) 
    {
        // Warn.
        Con::warnf( "VPath::unmountObject() - Object is not attached to this Path. %d", pObject->getId() );
        // Not Mounted Here!
        return;
    }

    // Parent Call.
    Parent::unmountObject( pObject );

    // Clear the mounted mask.
    // Note: This is so that we send the mounting information via the VPath
    //       packets instead of letting T3D handle it.
    pObject->clearMaskBits( SceneObject::MountedMask );
}

void VPath::getMountTransform( S32 pIndex, const MatrixF &pInTransform, MatrixF *pTransform )
{
    // Fetch the Scene Object.
    VPathObject *pathObject = NULL;
    for ( SceneObject *itr = getMountList(); itr != NULL; itr = itr->getMountLink() )
    {
        if ( itr->getMountNode() == pIndex )
        {
            pathObject = getPathObject( itr );
            break;
        }
    }

    if ( !pathObject )
    {
        // Reset Transform.
        *pTransform = pInTransform;
        // Sanity!
        return;
    }

    // Advance the Object.
    advanceObject( pathObject, TickSec );

    // Apply Transform.
    *pTransform = pathObject->getTransform();
}

void VPath::getRenderMountTransform( F32 pDelta, S32 pIndex, const MatrixF &pInTransform, MatrixF *pTransform )
{
    // Fetch the Scene Object.
    VPathObject *pathObject = NULL;
    for ( SceneObject *itr = getMountList(); itr != NULL; itr = itr->getMountLink() )
    {
        if ( itr->getMountNode() == pIndex )
        {
            pathObject = getPathObject( itr );
            break;
        }
    }

    if ( !pathObject )
    {
        // Reset Transform.
        *pTransform = pInTransform;
        // Sanity!
        return;
    }

    // Apply Transform.
    *pTransform = pathObject->getRenderTransform( pDelta );
}

VectorF VPath::getMountVelocity( const U32 &pIndex )
{
    // Fetch the Scene Object.
    VPathObject *pathObject = NULL;
    for ( SceneObject *itr = getMountList(); itr != NULL; itr = itr->getMountLink() )
    {
        if ( itr->getMountNode() == pIndex )
        {
            pathObject = getPathObject( itr );
            break;
        }
    }

    if ( !pathObject )
    {
        // Sanity!
        return VectorF::Zero;
    }

    // Determine Velocity.
    return ( pathObject->getOrientation() * pathObject->getSpeed() );
}

//-----------------------------------------------------------------------------
//
// Persistence Methods.
//
//-----------------------------------------------------------------------------

void VPath::readFields( void )
{
    const char *nodeData = "";
    for ( S32 nodeIndex = 0; dStrcmp( nodeData = getDataField( StringTable->insert( avar( "Node%d", nodeIndex ) ), NULL ), "" ) != 0; nodeIndex++ )
    {
        // Create Node.
        VPathNode *node = createNode();
        // Deserialize the Node.
        node->fromString( nodeData );
        // Add the Node.
        addNode( node );

        // Clear Field.
        setDataField( StringTable->insert( avar( "Node%d", nodeIndex ) ), NULL, "" );
    }

    // Update Transforms.
    updateNodeTransforms();

    // Update Size.
    updateContainer();

    // Calculate Path.
    calculatePath();
}

void VPath::writeFields( Stream &pStream, U32 pTabStop )
{
    // Field Name.
    StringTableEntry fieldName = StringTable->insert( "node" );

    for ( VPathNodeIterator itr = mNodeList.begin(); itr != mNodeList.end(); itr++ )
    {
        // Set Field.
        setDataField( fieldName, avar( "%d" , ( itr - mNodeList.begin() ) ), ( *itr )->toString().c_str() );
    }

    // Write Fields.
    Parent::writeFields( pStream, pTabStop );

    for ( VPathNodeIterator itr = mNodeList.begin(); itr != mNodeList.end(); itr++ )
    {
        // Clear Field.
        setDataField( fieldName, avar( "%d" , ( itr - mNodeList.begin() ) ), "" );
    }
}

U32 VPath::packUpdate( NetConnection *pConnection, U32 pMask, BitStream *pStream )
{
    U32 retMask = Parent::packUpdate( pConnection, pMask, pStream );

    if ( pMask & InitialUpdateMask )
    {
        // Delete Notify.
        deleteNotify( pConnection );
    }

    if ( pStream->writeFlag( pMask & PathUpdateMask ) )
    {
        // Write Path Type.
        pStream->writeInt( mPathType, gPathTypeBits );

        // Write Transform.
        mathWrite( *pStream, mObjToWorld );
        // Write Scale.
        mathWrite( *pStream, mObjScale );
    }

    if ( pStream->writeFlag( pMask & NodeUpdateMask ) )
    {
        // Path needs recalculating?
        bool needsCalculating = false;

        // Delete Vector.
        Vector<U32> deleteVector;
        // Update Vector.
        Vector<U32> updateVector;

        for ( U32 i = 0; i < mNodeList.size(); i++ )
        {
            // Fetch Node.
            VPathNode *node = mNodeList[i];

            // Already In Map?
            if ( !node->isConnection( pConnection ) )
            {
                // Insert.
                node->addConnection( pConnection );
            }

            // Fetch State.
            VNetStateInfo *state = node->getState( pConnection );

            // Delete new node?
            if ( state->Mask & VPathNode::k_StateDelete
                 && state->Mask & VPathNode::k_StateCreate )
            {
                // Remove Node.
                removeNode( i-- );
                // Flag true.
                needsCalculating = true;
            }

            // Delete?
            else if ( state->Mask & VPathNode::k_StateDelete )
            {
                // Add To List.
                deleteVector.push_front( i );
            }

            // Update?
            else if ( state->Mask & VPathNode::k_StateUpdate )
            {
                if ( updateVector.size() < gMaxNodeTransmit )
                {
                    // Add To List.
                    updateVector.push_back( i );
                }
            }
        }

        // More Updates?
        if ( updateVector.size() == gMaxNodeTransmit )
        {
            // More Updates.
            retMask |= NodeUpdateMask;
        }

        // Write Count.
        pStream->writeInt( updateVector.size(), gMaxNodeBits + 1 );

        for ( Vector<U32>::iterator itr = updateVector.begin(); itr != updateVector.end(); itr++ )
        {
            // Fetch Index.
            const U32 index = ( *itr );

            // Write Index.
            pStream->writeInt( index, gMaxNodeBits );
            // Pack Update.
            retMask |= mNodeList[index]->packNode( pConnection, pStream );
        }

        // Write Count.
        pStream->writeInt( deleteVector.size(), gMaxNodeBits + 1 );

        if ( deleteVector.size() > 0 )
        {
            for ( Vector<U32>::iterator itr = deleteVector.begin(); itr != deleteVector.end(); itr++ )
            {
                // Fetch Index.
                const U32 index = ( *itr );

                // Write Index.
                pStream->writeInt( index, gMaxNodeBits );
                // Remove Node.
                removeNode( index );
            }

            // Flag true.
            needsCalculating = true;
            // Clear Vector.
            deleteVector.clear();
        }

        // Recalculate path?
        if ( needsCalculating )
        {
            // Update Size.
            updateContainer();
            // Calculate Path.
            calculatePath();
        }
    }

    if ( pStream->writeFlag( pMask & ObjectUpdateMask ) )
    {
        // Detach Vector.
        Vector<U32> detachVector;
        // Update Vector.
        Vector<U32> updateVector;

        for ( U32 i = 0; i < mObjectList.size(); i++ )
        {
            // Fetch Node.
            VPathObject *pathObject = mObjectList[i];

            // Already In Map?
            if ( !pathObject->isConnection( pConnection ) )
            {
                // Insert.
                pathObject->addConnection( pConnection );
            }

            // Fetch State.
            VNetStateInfo *state = pathObject->getState( pConnection );

            // Detach newly attached object?
            if ( state->Mask & VPathObject::k_StateAttach
                 && state->Mask & VPathObject::k_StateDetach )
            {
                // Process Detach.
                onDetachObject( pathObject );
                // Decrease index.
                i -= 1;

                // Skip.
                continue;
            }

            // Update?
            if ( state->Mask & VPathObject::k_StateUpdate )
            {
                if ( updateVector.size() < gMaxObjectTransmit )
                {
                    // Add To List.
                    updateVector.push_back( i );
                }
            }

            // Detach?
            if ( state->Mask & VPathObject::k_StateDetach )
            {
                // Add To List.
                detachVector.push_front( i );
            }
        }

        // More Updates?
        if ( updateVector.size() == gMaxObjectTransmit )
        {
            // More Updates.
            retMask |= ObjectUpdateMask;
        }

        // Write Count.
        pStream->writeInt( updateVector.size(), gMaxObjectBits + 1 );

        for ( Vector<U32>::iterator itr = updateVector.begin(); itr != updateVector.end(); itr++ )
        {
            // Fetch Index.
            const U32 index = ( *itr );

            // Write Index.
            pStream->writeInt( index, gMaxObjectBits );

            // Fetch the object.
            VPathObject *pathObject = mObjectList[index];
            // Fetch State.
            VNetStateInfo *state = pathObject->getState( pConnection );

            // Was the Object Attached?
            if ( pStream->writeFlag( state->Mask & VPathObject::k_StateAttach ) )
            {
#ifdef VPATH_DEBUG_NET
                Con::printf( "VPath::packUpdate() - Attached - %d | %d", isServerObject(), index );
#endif

                // Clear Update.
                state->Mask &= ~VPathObject::k_StateAttach;
            }

            // Pack Object.
            retMask |= mObjectList[index]->packUpdate( pConnection, pStream );
        }

        // Write Count.
        pStream->writeInt( detachVector.size(), gMaxObjectBits + 1 );

        if ( detachVector.size() > 0 )
        {
            for ( Vector<U32>::iterator itr = detachVector.begin(); itr != detachVector.end(); itr++ )
            {
                // Fetch Index.
                const U32 index = ( *itr );
                // Write Index.
                pStream->writeInt( index, gMaxObjectBits );
                // Process Detach.
                onDetachObject( mObjectList[index] );
            }

            // Clear Vector.
            detachVector.clear();
        }
    }

    // Return.
    return retMask;
}

void VPath::unpackUpdate( NetConnection *pConnection, BitStream *pStream )
{
    Parent::unpackUpdate( pConnection, pStream );

    // Update Path?
    if ( pStream->readFlag() )
    {
        // Read Path Type.
        mPathType = pStream->readInt( gPathTypeBits );

        // Read Transform.
        mathRead( *pStream, &mObjToWorld );
        // Read Scale.
        mathRead( *pStream, &mObjScale );

        // Update Nodes.
        updateNodeTransforms();
        // Calculate Path.
        calculatePath();
    }

    // Update Nodes?
    if ( pStream->readFlag() )
    {
        // Number To Update.
        const U32 updateCount = pStream->readInt( gMaxNodeBits + 1 );

        for ( U32 i = 0; i < updateCount; i++ )
        {
            // Read Index.
            const U32 nodeIndex = pStream->readInt( gMaxNodeBits );

            // Was the Node Created?
            if ( pStream->readFlag() )
            {
                // Create Node.
                VPathNode *node = createNode();
                // Add the Node.
                addNode( node, nodeIndex );
            }

            // Reference Node.
            VPathNode *node = mNodeList[nodeIndex];
            // Apply Update.
            node->unpackNode( pConnection, pStream );
        }

        // Number To Delete.
        const U32 deleteCount = pStream->readInt( gMaxNodeBits + 1 );

        for ( U32 i = 0; i < deleteCount; i++ )
        {
            // Remove Node.
            removeNode( pStream->readInt( gMaxNodeBits ) );
        }

        // Update Size.
        updateContainer();
        // Calculate Path.
        calculatePath();
    }

    // Update Objects?
    if ( pStream->readFlag() )
    {
        // Number To Update.
        const U32 updateCount = pStream->readInt( gMaxObjectBits + 1 );

        for ( U32 i = 0; i < updateCount; i++ )
        {
            // Read Index.
            const U32 objectIndex = pStream->readInt( gMaxObjectBits );

            // Read Attached.
            // Note: The editor handles the both the server and client side attachment calls.
            //       This is dangerous because there could be a mix up in indices, but it is
            //       needed to ensure the editor runs smoothly :(
            const bool wasAttached = pStream->readFlag();
            if ( wasAttached && objectIndex >= mObjectList.size() )
            {
#ifdef VPATH_DEBUG_NET
                Con::printf( "VPath::unpackUpdate() - WasAttached - %d | %d", isServerObject(), objectIndex );
#endif

                // Create & Add to the List.
                attachObject( new VPathObject() );
            }

            // Reference Node.
            VPathObject *pathObject = mObjectList[objectIndex];

            // Unpack Update.
            pathObject->unpackUpdate( pConnection, pStream );

            // Object Attached this Unpack?
            if ( wasAttached )
            {
                // Reset.
                setPathObjectInterp( pathObject, pathObject->getTimeInterp() );
            }
        }

        // Number To Detach.
        const U32 detachCount = pStream->readInt( gMaxObjectBits + 1 );

        for ( U32 i = 0; i < detachCount; i++ )
        {
            // Fetch the path object.
            VPathObject *pathObject = mObjectList[pStream->readInt( gMaxObjectBits )];
            // Detach callback.
            onDetachObject( pathObject );
        }
    }
}

//-----------------------------------------------------------------------------
//
// Node Methods.
//
//-----------------------------------------------------------------------------

VPathNode *VPath::createNode( void )
{
    return new VPathNode();
}

void VPath::deleteNode( VPathNode *pNode )
{
    delete pNode;
}

void VPath::clear( void )
{
    for ( VPathObjectIterator itr = mObjectList.begin(); itr != mObjectList.end(); itr++ )
    {
        VPathObject *pathObject = ( *itr );

        // Fetch the attached object.
        SceneObject *refObject = pathObject->getObject();
        // Unmount Object.
        unmountObject( refObject );

        // Delete the Path Object.
        delete pathObject;
    }

    // Clear Object List.
    mObjectList.clear();

    for ( VPathNodeIterator itr = mNodeList.begin(); itr != mNodeList.end(); itr++ )
    {
        deleteNode( ( *itr ) );
    }

    // Clear Node List.
    mNodeList.clear();

    if ( isServerObject() )
    {
        // Update.
        setMaskBits( NodeUpdateMask );
    }
}

VPathNode *VPath::getNode( const S32 &pNodeIndex )
{
    // Sanity!
    AssertFatal( pNodeIndex >= 0 && pNodeIndex < mNodeList.size(), "VPath::getNode() - Invalid Index" );

    // Return Node.
    return mNodeList[pNodeIndex];
}

ConsoleMethod( VPath, addNode, void, 4, 5, "( transform pTransform, float pWeight, [int pLocation] ) - Add a node with the given properties. Nodes represent physical points that attached objects move towards or between, but the PathType determines \"how\" they move between them.\n"
                                           "@param pTransform The position and rotation of the new node.\n"
                                           "@param pWeight The weight of the new node.\n"
                                           "@param pLocation The index of the new node.\n"
                                           "@return No return value.")
{
    // Fetch Invers Path Transform.
    MatrixF pathTransformInv = object->getTransform();
    pathTransformInv.setPosition( Point3F::Zero );
    pathTransformInv.inverse();

    Point3F  pos;
    QuatF    rot;
    AngAxisF aa;

    // Scan.
    dSscanf( argv[2], "%g %g %g %g %g %g %g", &pos.x, &pos.y, &pos.z, &aa.axis.x, &aa.axis.y, &aa.axis.z, &aa.angle );
    
    // Set Rotation.
    rot.set( aa );

    // Weight.
    F32 weight = dAtof( argv[3] );

    // World to Local Position.
    Point3F nodePosition = ( pos - object->getPosition() );
    pathTransformInv.mulP( nodePosition );

    // World to Local Rotation.
    MatrixF nodeRotationMat;
    rot.setMatrix( &nodeRotationMat );
    pathTransformInv.mul( nodeRotationMat );
    
    // Set Quat.
    QuatF nodeRotation;
    nodeRotation.set( nodeRotationMat );

    S32 location = -1;
    if ( argc >= 5 )
    {
        // Target Location.
        location = dAtoi( argv[4] );
    }

    // Add Node.
    VPathNode *node = object->addNode( nodePosition, nodeRotation, weight, location );

    // Valid Node?
    if ( node )
    {
        // Update Size.
        object->updateContainer();

        // Calculate Path.
        object->calculatePath();
    }
}

VPathNode *VPath::addNode( const Point3F &pPosition, const QuatF &pRotation, const F32 &pWeight, const S32 &pLocation )
{
    // Reference Object.
    VPathNode *pathNode = createNode();

    // Store Properties.
    pathNode->setLocalPosition( pPosition );
    pathNode->setLocalRotation( pRotation );
    pathNode->setWeight( pWeight );

    // Add Node.
    return addNode( pathNode, pLocation );
}

VPathNode *VPath::addNode( VPathNode *pNode, const S32 &pLocation )
{
    if ( pNode->getPath() )
    {
        // Error.
        Con::errorf( "VPath::addNode() - Node already belongs to a Path, '%d'", pNode->getPath()->getId() );

        return NULL;
    }
    else if ( mNodeList.size() == gMaxNodeCount )
    {
        // Error.
        Con::errorf( "VPath::addNode() - Reached Max Nodes (%d)", gMaxNodeCount );

        // Delete Node.
        deleteNode( pNode );

        return NULL;
    }

    // Set Path.
    pNode->setPath( this );

    // Update World Data.
    pNode->updateWorldData();

    if ( pLocation < 0 )
    {
        // Push Back.
        mNodeList.push_back( pNode );
    }
    else
    {
        // Fetch Size.
        const S32 nodeCount = mNodeList.size();

        if ( pLocation >= nodeCount )
        {
            // Push Back.
            mNodeList.push_back( pNode );
        }
        else
        {
            // Insert.
            mNodeList.insert( ( mNodeList.address() + pLocation ), pNode );
        }
    }

    if ( isServerObject() )
    {
        // Update.
        setMaskBits( NodeUpdateMask );
    }

    // Return Node.
    return pNode;
}

ConsoleMethod( VPath, deleteNode, void, 3, 3, "( int pNodeIndex ) - Delete the node with the given index. If you delete a node that an attached object is moving to, or from then the object's movement will adjust so that it has a valid path.\n"
                                              "@param pNodeIndex The index of the node to be deleted.\n"
                                              "@return No return value." )
{
    // Fetch Index.
    const S32 nodeIndex = dAtoi( argv[2] );
    
    // Apply Update.
    object->deleteNode( nodeIndex );
}

void VPath::deleteNode( const S32 &pNodeIndex )
{
    if ( pNodeIndex < 0 || pNodeIndex >= mNodeList.size() )
    {
        // Woops!
        Con::warnf( "VPath::deleteNode() - Invalid Index Specified (%d).", pNodeIndex );
        return;
    }

    // Fetch Node.
    VPathNode *node = mNodeList[pNodeIndex];

    // Remove Node References.
    for ( VPathObjectIterator itr = mObjectList.begin(); itr != mObjectList.end(); itr++ )
    {
        // Fetch Object.
        VPathObject *pathObject = ( *itr );

        if ( ( pathObject->getSourceNode() >= pNodeIndex ) || ( pathObject->getDestinationNode() >= pNodeIndex ) )
        {
            S32 srcNode = pathObject->getSourceNode();
            S32 dstNode = pathObject->getDestinationNode();

            if ( pathObject->isForward() )
            {
                if ( srcNode >= pNodeIndex )
                {
                    srcNode -= 1;
                }

                if ( dstNode > pNodeIndex )
                {
                    dstNode -= 1;
                }
            }
            else
            {
                if ( srcNode > pNodeIndex )
                {
                    srcNode -= 1;
                }

                if ( dstNode >= pNodeIndex )
                {
                    dstNode -= 1;
                }
            }

            // Normalize indices.
            normalizeNodeIndex( srcNode, ( mNodeList.size() - 1 ) );
            normalizeNodeIndex( dstNode, ( mNodeList.size() - 1 ) );

            // Apply Update.
            pathObject->setNode( srcNode, dstNode );

            if ( isServerObject() )
            {
                // Update Objects.
                setMaskBits( ObjectUpdateMask );
            }
        }
    }

    if ( isServerObject() )
    {
        // Network Flags.
        setMaskBits( NodeUpdateMask );

        // Flag for Deletion.
        node->setMaskBits( VPathNode::k_StateDelete );
    }
}

void VPath::removeNode( const S32 &pNodeIndex )
{
    // Fetch the node.
    VPathNode *node = getNode( pNodeIndex );
    if ( !node )
    {
        // Quit.
        return;
    }

    // Delete Node.
    deleteNode( node );
    // Erase Node.
    mNodeList.erase( pNodeIndex );
}

S32 VPath::normalizeNodeIndex( S32 &pNodeIndex )
{
    const S32 nodeCount = mNodeList.size();
    if ( nodeCount == 0 )
    {
        // No Nodex.
        pNodeIndex = 0;
    }
    else
    {
        while ( pNodeIndex < 0 )
        {
            // Wrap Backwards.
            pNodeIndex += nodeCount;
        }

        // Wrap Forwards.
        pNodeIndex %= nodeCount;
    }

    // Return Index.
    return pNodeIndex;
}

S32 VPath::normalizeNodeIndex( const S32 &pNodeIndex )
{
    // Temp.
    S32 nodeIndex = pNodeIndex;

    // Return Index.
    return normalizeNodeIndex( nodeIndex );
}

S32 VPath::normalizeNodeIndex( S32 &pNodeIndex, const S32 &pNodeCount )
{
    if ( pNodeCount == 0 )
    {
        // No Nodex.
        pNodeIndex = 0;
    }
    else
    {
        while ( pNodeIndex < 0 )
        {
            // Wrap Backwards.
            pNodeIndex += pNodeCount;
        }

        // Wrap Forwards.
        pNodeIndex %= pNodeCount;
    }

    // Return Index.
    return pNodeIndex;
}

//-----------------------------------------------------------------------------
//
// Object Methods.
//
//-----------------------------------------------------------------------------

ConsoleMethod( VPath, isObjectAttached, bool, 3, 3, "( SimObject pObject ) - Is the object attached to this path?\n"
                                                    "@param pObject The SimObjectID of the object you wish to check.\n"
                                                    "@return Returns true if the object is attached to this path." )
{
    // Fetch Object.
    SceneObject *sceneObject;
    if ( !Sim::findObject( argv[2], sceneObject ) )
    {
        Con::errorf( "VPath::isObjectAttached() - Invalid Target Object." );
        return false;
    }

    // Attached?
    return object->isObjectAttached( sceneObject );
}

bool VPath::isObjectAttached( SceneObject *pObject )
{
    // Valid Object?
    return ( getPathObject( pObject ) != NULL );
}

VPathObject *VPath::getPathObject( SceneObject *pObject )
{
    for ( VPathObjectIterator itr = mObjectList.begin(); itr != mObjectList.end(); itr++ )
    {
        // Correct Object?
        if ( ( *itr )->getObject() == pObject )
        {
            // Yes.
            return ( *itr );
        }
    }

    return NULL;
}

ConsoleMethod( VPath, attachObject, void, 7, 8, "( SimObject pObject, bool pForward, float pSpeed, bool pRelative, int pStartNode, [int pEndNode] ) - Attach an object to this path with the given properties. If the object is already attached to a path, then a warning will be displayed and the object will *not* be attached to this path.\n"
                                                "@param pObject The SimObjectID of the object to be attached.\n"
                                                "@param pForward Should the object be moving forward?\n"
                                                "@param pSpeed The speed that the object will travel around the path.\n"
                                                "@param pRelative Offset the object based on the difference between the start node and its current position.\n"
                                                "@param pStartNode The index of the node this object starts pathing from.\n"
                                                "@param pEndNode The index of the node this object will stop pathing at."
                                                "@return No return value." )
{
    // Fetch Object.
    SceneObject *sceneObject;
    if ( !Sim::findObject( argv[2], sceneObject ) )
    {
        Con::errorf( "VPath::attachObject() - Invalid Target Object." );
        return;
    }

    // Fetch Direction.
    const bool forward   = dAtob( argv[3] );
    // Fetch Speed.
    const F32  speed     = dAtof( argv[4] );
    // Fetch Relativity.
    const bool relative  = dAtob( argv[5] );
    // Fetch Start Node.
    const S32  startNode = dAtoi( argv[6] );
    // Fetch End Node.
    const S32  endNode   = ( argc >= 8 ) ? dAtoi( argv[7] ) : -1;

    // Attach Object.
    object->attachObject( sceneObject, forward, speed, relative, startNode, endNode );
}

void VPath::attachObject( SceneObject *pObject, const bool &pForward, const F32 &pSpeed, const bool &pRelative, const S32 &pStartNode, const S32 &pEndNode )
{
    attachObject( pObject, pForward, pSpeed, pRelative, pStartNode, pEndNode, VPathObject::k_OrientationToPath, NULL );
}

void VPath::attachObject( SceneObject *pObject, const bool &pForward, const F32 &pSpeed, const bool &pRelative, const S32 &pStartNode, const S32 &pEndNode, const VPathObject::eOrientationType &pOrientationMode )
{
    attachObject( pObject, pForward, pSpeed, pRelative, pStartNode, pEndNode, pOrientationMode, NULL );
}

void VPath::attachObject( SceneObject *pObject, const bool &pForward, const F32 &pSpeed, const bool &pRelative, const S32 &pStartNode, const S32 &pEndNode, const VPathObject::eOrientationType &pOrientationMode, void *pOrientationData )
{
    // Already Pathing?
    if ( isObjectAttached( pObject ) )
    {
        Con::warnf( "VPath::attachObject() - Object Already Attached to a Path." );
        return;
    }

    // Determine Target Nodes.
    const S32 srcNode = normalizeNodeIndex( pStartNode );
    const S32 dstNode = normalizeNodeIndex( ( pForward ) ? pStartNode + 1 : pStartNode - 1 );
    const S32 endNode = ( pEndNode == -1 ) ? pEndNode : normalizeNodeIndex( pEndNode );

    // Valid Source Node?
    if ( getNodeCount() == 0 || !getNode( srcNode ) )
    {
        Con::warnf( "VPath::attachObject() - Invalid Start Node." );
        return;
    }

    VPathObject *pathObject = new VPathObject();

    // Init Properties.
    pathObject->setActive( true );
    pathObject->setObject( pObject );

    pathObject->setForward( pForward );

    pathObject->setTimeInterp( 0.f );
    pathObject->setPathInterp( 0.f );
    pathObject->setOffset( Point3F::Zero );
    pathObject->setSpeed( pSpeed );

    switch( pOrientationMode )
    {
        case VPathObject::k_OrientationFree :
        case VPathObject::k_OrientationInterpolate :
        case VPathObject::k_OrientationToPath :
            {
                pathObject->setOrientationMode( pOrientationMode );

            } break;

        case VPathObject::k_OrientationToObject :
            {
                pathObject->setOrientationMode( pOrientationMode, (SceneObject*)pOrientationData );

            } break;

        case VPathObject::k_OrientationToPoint :
            {
                pathObject->setOrientationMode( pOrientationMode, ( *(Point3F*)pOrientationData ) );

            } break;
    }

    pathObject->setNode( srcNode, dstNode );
    pathObject->setStartNode( srcNode );
    pathObject->setEndNode( endNode );

    // Fetch Init Node.
    VPathNode *node = mNodeList[srcNode];

    // Relative Position?
    if ( pRelative )
    {
        // Set Position Offset.
        pathObject->setOffset( pObject->getPosition() - node->getWorldPosition() );
    }

    // Set info.
    setPathObjectInterp( pathObject, 0.f );

    // Attach.
    attachObject( pathObject );
}

void VPath::attachObject( VPathObject *pPathObject )
{
#ifdef VPATH_DEBUG_NET
    Con::printf( "VPath::attachObject() - %d", isServerObject() );
#endif

    if ( mObjectList.size() == gMaxObjectCount )
    {
        Con::errorf( "VPath::attachObject() - Reached Max Objects (%d)", gMaxObjectCount );
        return;
    }

    // Add to List.
    mObjectList.push_back( pPathObject );

    // Callback.
    onAttachObject( pPathObject );

    if ( isServerObject() )
    {
        // Update.
        setMaskBits( ObjectUpdateMask );
    }
}

void VPath::onAttachObject( VPathObject *pPathObject )
{
    // Valid Object?
    SceneObject *refObject = pPathObject->getObject();
    if ( !refObject )
    {
        return;
    }

#ifdef VPATH_DEBUG_NET
    Con::printf( "VPath::onAttachObject() - %d | %d", isServerObject(), refObject->getId() );
#endif

    // Delete Notify.
    deleteNotify( refObject );

    if ( isServerObject() )
    {
        // Fetch the Available Mount Index.
        U32 mountIndex = getAvailableMountIndex();
        // Mount the Object to this Path.
        mountObject( refObject, mountIndex );

        // Return Buffer.
        char buffer[1][32];
        dSprintf( buffer[0], sizeof( buffer[0] ), "%d", refObject->getId() );

        // Callback.
        // VPath::onAttachObject( %object );
        Con::executef( this, "onAttachObject", buffer[0] );
    }
}

ConsoleMethod( VPath, detachObject, void, 3, 3, "( SimObject pObject ) - Detach the object from this path in place.\n"
                                                "@param pObject The SimObjectID of the object to be detached.\n"
                                                "@return No return value." )
{
    // Fetch Object.
    SceneObject *sceneObject;
    if ( !Sim::findObject( argv[2], sceneObject ) )
    {
        Con::errorf( "VPath::detachObject() - Invalid Target Object." );
        return;
    }

    // Detach Object.
    object->detachObject( sceneObject );
}

void VPath::detachObject( SceneObject *pObject )
{
    VPathObject *pathObject = getPathObject( pObject );
    if ( !pathObject )
    {
        Con::warnf( "VPath::detachObject() - Object (%d) Not Attached to Path.", pObject->getId() );
        return;
    }

    // Detach.
    detachObject( pathObject );
}

void VPath::detachObject( VPathObject *pPathObject )
{
#ifdef VPATH_DEBUG_NET
        Con::printf( "VPath::detachObject() - %d", isServerObject() );
#endif

    if ( isServerObject() )
    {
        // Update Objects.
        setMaskBits( ObjectUpdateMask );

        // Detach.
        pPathObject->setMaskBits( VPathObject::k_StateDetach );
    }

    /*
    // Valid Object?
    SceneObject *refObject = pPathObject->getObject();
    if ( refObject )
    {
        // Unmount Object.
        unmountObject( refObject );
    }
    */
}

void VPath::onDetachObject( VPathObject *pPathObject )
{
    // Valid Object?
    SceneObject *refObject = pPathObject->getObject();
    if ( !refObject )
    {
        return;
    }

#ifdef VPATH_DEBUG_NET
        Con::printf( "VPath::onDetachObject() - %d | %d", isServerObject(), refObject->getId() );
#endif
    
    // Reset.
    setPathObjectInterp( pPathObject, pPathObject->getTimeInterp() );
    // Unmount Object.
    unmountObject( refObject );

    // Delete the Path Object.
    delete pPathObject;
    // Remove from the Set.
    mObjectList.erase( mObjectList.find_next( pPathObject ) );

    // Clear Delete Notify.
    clearNotify( refObject );

    if ( isServerObject() )
    {
        // Return Buffer.
        char buffer[1][32];
        dSprintf( buffer[0], sizeof( buffer[0] ), "%d", refObject->getId() );

        // Callback.
        // VPath::onDetachObject( %object );
        Con::executef( this, "onDetachObject", buffer[0] );
    }
}

void VPath::processTick( const Move *pMove )
{
}

void VPath::advanceObject( VPathObject *pPathObject, const F32 &pDelta )
{
    SceneObject *refObject = pPathObject->getObject();
    if ( !refObject || mIsZero( pDelta ) )
    {
        // Ignore.
        return;
    }

    // Spatial Delta.
    pPathObject->popDelta();

    // Active and Moving?
    if ( !pPathObject->isActive() || mIsZero( pPathObject->getSpeed() ) )
    {
        // Update Delta.
        pPathObject->pushDelta( refObject->getPosition(), refObject->getTransform().getForwardVector() );
        // Skip.
        return;
    }

    // Fetch Nodes.
    VPathNode *srcNode = mNodeList[pPathObject->getSourceNode()];
    VPathNode *dstNode = mNodeList[pPathObject->getDestinationNode()];
    VPathNode *lenNode = ( pPathObject->isForward() ) ? srcNode : dstNode;

    // Calculate Interp Delta.
    const F32 stepDistance    = ( pPathObject->getSpeed() * pDelta );
    const F32 speedMod        = ( pPathObject->getSpeed() / lenNode->getLength() );
    F32 timeInterp            = pPathObject->getTimeInterp();
    F32 timeInterpDelta       = ( speedMod * pDelta );
    F32 pathInterp            = pPathObject->getPathInterp();
    F32 pathInterpDelta       = 0.f;

    // Fetch the old position.
    const Point3F oldPosition = pPathObject->getPosition();
    // Calculate the new position and path delta.
    Point3F newPosition = getAdvancedPathPosition( pPathObject, stepDistance, pathInterpDelta );

    // Finished?
    if ( ( timeInterp + timeInterpDelta ) >= 1.f )
    {
        // Finished?
        if ( pPathObject->getDestinationNode() == pPathObject->getEndNode() )
        {
            // Stop Updates.
            pPathObject->setActive( false );
        }
        else
        {
            // Update Nodes.
            const S32 srcNodeIndex = pPathObject->getDestinationNode();
            const S32 dstNodeIndex = normalizeNodeIndex( ( pPathObject->isForward() ) ? srcNodeIndex + 1 : srcNodeIndex - 1 );
            
#ifdef VPATH_DEBUG_STEP
            if ( isServerObject() )
                Con::errorf( "Change Node:\n  Source, %d\n  Destination, %d", srcNodeIndex, dstNodeIndex );
#endif

            // Apply Changes.
            pPathObject->setNode( srcNodeIndex, dstNodeIndex );
            pPathObject->setTimeInterp( 0.f );
            pPathObject->setPathInterp( 0.f );
            pPathObject->setPosition( newPosition );

            // Reset local interp information.
            timeInterp = 0.f;
            timeInterpDelta = 0.f;
            pathInterp = 0.f;
            pathInterpDelta = 0.f;

            // Fetch the distance we've travelled.
            const F32 &advanceDistance = ( newPosition - oldPosition ).len();
            // Any remaining distance?
            if ( ( stepDistance - advanceDistance ) > 0.0001f )
            {
                // Determine how much more we need to move.
                Point3F newPosition0 = newPosition;
                newPosition = getAdvancedPathPosition( pPathObject, ( stepDistance - advanceDistance ), pathInterpDelta );

#ifdef VPATH_DEBUG_STEP
                if ( isServerObject() )
                    Con::errorf( "Transition Step: %f\nTransition Distance: %f + %f = %f", pathInterpDelta, advanceDistance, ( newPosition - newPosition0 ).len(), advanceDistance + ( newPosition - newPosition0 ).len() );
#endif
            }
        }

        if ( isServerObject() )
        {
            // Return Buffer.
            char buffer[3][32];
            dSprintf( buffer[0], sizeof( buffer[0] ), "%d", refObject->getId() );
            dSprintf( buffer[1], sizeof( buffer[1] ), "%d", pPathObject->isActive() ? pPathObject->getSourceNode() : pPathObject->getDestinationNode() );
            dSprintf( buffer[2], sizeof( buffer[2] ), "%d", !pPathObject->isActive() );

            // Callback.
            // VPath::onReachNode( %object, %node, %finished );
            Con::executef( this, "onReachNode", buffer[0], buffer[1], buffer[2] );
        }
    }

    // Update Object Interp.
    timeInterp = mClampF( timeInterp + timeInterpDelta, 0.f, 1.f );
    pathInterp = mClampF( pathInterp + pathInterpDelta, 0.f, 1.f );

    // Apply Changes.
    pPathObject->setTimeInterp( timeInterp );
    pPathObject->setPathInterp( pathInterp );
    pPathObject->setPosition( newPosition );

#ifdef VPATH_DEBUG_STEP
    if ( isServerObject() )
        Con::printf( "Time / Distance: %f %f / %f %f", timeInterp, pathInterp, stepDistance, ( newPosition - oldPosition ).len() );
#endif

    switch ( pPathObject->getOrientationMode().Type )
    {
        case VPathObject::k_OrientationInterpolate :
        case VPathObject::k_OrientationToObject :
        case VPathObject::k_OrientationToPoint :
            {
                // Update Orientation.
                updateOrientation( pPathObject );

            } break;

        case VPathObject::k_OrientationToPath :
            {
                // Determine the path orientation.
                VectorF pathOrientation = ( newPosition - oldPosition );
                pathOrientation.normalize();

                // Update Orientation.
                updateOrientation( pPathObject, pathOrientation );

            } break;
    }

    // Update Delta.
    pPathObject->pushDelta( pPathObject->getPosition(), pPathObject->getOrientation() );

    if ( isServerObject() )
    {
        // Update Objects.
        setMaskBits( ObjectUpdateMask );

        // Update This Object.
        pPathObject->setMaskBits( VPathObject::k_StateUpdatePosition );
    }
}

void VPath::updatePosition( VPathObject *pPathObject )
{
    // Fetch Nodes.
    VPathNode *srcNode = getNode( pPathObject->getSourceNode() );
    VPathNode *dstNode = getNode( pPathObject->getDestinationNode() );

    // Fetch Position.
    F32 pathInterp = 0.f;
    const Point3F newPosition = getPathPosition( srcNode, dstNode, pPathObject->getTimeInterp(), pPathObject->isForward(), pathInterp );

    // Apply Position.
    pPathObject->setPosition( newPosition );
    pPathObject->setPathInterp( pathInterp );
}

void VPath::updateOrientation( VPathObject *pPathObject )
{
    // Update Orientation?
    if ( pPathObject->getOrientationMode().Type == VPathObject::k_OrientationFree )
    {
        // Skip.
        return;
    }

    // Fetch Nodes.
    VPathNode *srcNode = getNode( pPathObject->getSourceNode() );
    VPathNode *dstNode = getNode( pPathObject->getDestinationNode() );

    // Determine Path Orientation.
    VectorF pathOrientation;
    switch ( pPathObject->getOrientationMode().Type )
    {
        case VPathObject::k_OrientationInterpolate :
            {
                // Interpolate Between Transforms.
                QuatF rot;
                rot.interpolate( srcNode->getWorldRotation(), dstNode->getWorldRotation(), pPathObject->getPathInterp() );

                // Set Matrix.
                MatrixF mat;
                rot.setMatrix( &mat );

                // Fetch Orientation.
                pathOrientation = mat.getColumn3F( 1 );

            } break;

        case VPathObject::k_OrientationToObject :
            {
                // Fetch Orientation.
                pathOrientation = ( pPathObject->getOrientationMode().Object->getPosition() - pPathObject->getWorldPosition() );
                pathOrientation.normalizeSafe();

            } break;

        case VPathObject::k_OrientationToPoint :
            {
                // Fetch Orientation.
                pathOrientation = ( pPathObject->getOrientationMode().Point - pPathObject->getWorldPosition() );
                pathOrientation.normalizeSafe();

            } break;

        case VPathObject::k_OrientationToPath :
            {
                // Fetch Orientation.
                pathOrientation = getPathOrientation( srcNode, dstNode, pPathObject->getPathInterp(), pPathObject->isForward() );

            } break;
    }

    // Update.
    updateOrientation( pPathObject, pathOrientation );
}

void VPath::updateOrientation( VPathObject *pPathObject, const Point3F &pPathOrientation )
{
    // Update Orientation?
    if ( pPathObject->getOrientationMode().Type == VPathObject::k_OrientationFree )
    {
        // Skip.
        return;
    }

    // Fetch Nodes.
    VPathNode *srcNode = getNode( pPathObject->getSourceNode() );
    VPathNode *dstNode = getNode( pPathObject->getDestinationNode() );

    // Determine Source Orientation.
    VectorF srcOrientation;
    switch ( srcNode->getOrientationMode().Type )
    {
        case VPathNode::k_OrientationToPoint :
            {
                // Fetch Orientation.
                srcOrientation = ( srcNode->getOrientationMode().Point - pPathObject->getWorldPosition() );
                srcOrientation.normalize();

            } break;

        default :
            {
                // Use Path Orientation.
                srcOrientation = pPathOrientation;

            } break;
    }

    // Determine Destination Orientation.
    VectorF dstOrientation;
    switch ( dstNode->getOrientationMode().Type )
    {
        case VPathNode::k_OrientationToPoint :
            {
                // Fetch Orientation.
                dstOrientation = ( dstNode->getOrientationMode().Point - pPathObject->getWorldPosition() );
                dstOrientation.normalize();

            } break;

        default :
            {
                // Use Path Orientation.
                dstOrientation = pPathOrientation;

            } break;
    }

    // Determine Actual Orientation.
    VectorF orientation;
    orientation.interpolate( srcOrientation, dstOrientation, pPathObject->getTimeInterp() );

    // Apply.
    pPathObject->setOrientation( orientation );
}

//-----------------------------------------------------------------------------
//
// Path Methods.
//
//-----------------------------------------------------------------------------

void VPath::calculatePath( void )
{
    if ( mNodeList.size() < 2 )
    {
        // No Path.
        return;
    }

    switch ( mPathType )
    {
        case k_PathLinear : 
            {
                for ( VPathNodeIterator itr = mNodeList.begin(); itr != mNodeList.end(); itr++ )
                {
                    if ( itr == ( mNodeList.end() - 1 ) )
                    {
                        // Head, Front.
                        calculateLinearPath( ( *itr ), ( *( mNodeList.begin() ) ) );
                    }
                    else
                    {
                        // Head, Next.
                        calculateLinearPath( ( *itr ), ( *( itr + 1 ) ) );
                    }
                }

            } break;

        case k_PathBezier :
            {
                for ( VPathNodeIterator itr = mNodeList.begin(); itr != mNodeList.end(); itr++ )
                {
                    if ( itr == ( mNodeList.end() - 1 ) )
                    {
                        // Head, Prev, Front.
                        calculateBezierPath( ( *itr ), ( *( mNodeList.begin() ) ) );
                    }
                    else
                    {
                        // Head, Prev, Next.
                        calculateBezierPath( ( *itr ), ( *( itr + 1 ) ) );
                    }
                }

            } break;
    }
}

Point3F VPath::getAdvancedPathPosition( VPathObject *pPathObject, const F32 &pTargetDistance, F32 &pPathInterpDelta )
{
    switch( mPathType )
    {
        case k_PathLinear : 
            {
                return getAdvancedLinearPathPosition( pPathObject, pTargetDistance, pPathInterpDelta );

            } break;

        case k_PathBezier :
            {
                return getAdvancedBezierPathPosition( pPathObject, pTargetDistance, pPathInterpDelta );

            } break;
    }

    // Sanity!
    AssertFatal( false, "Invalid path type!" );
    return Point3F::Zero;
}

ConsoleMethod( VPath, getPathTransform, const char *, 5, 5, "( int pSrcNodeIndex, int pDstNodeIndex, int pTimeInterp ) - Get the transform of the path at the interp point between two nodes.\n"
                                                            "@param pSrcNodeIndex The first node.\n"
                                                            "@param pDstNodeIndex The second node.\n"
                                                            "@param pTimeInterp The time to interp between the two nodes. Value is between 0.0 and 1.0.\n"
                                                            "@return Returns the transform of the interp time between the two given nodes." )
{
    // Fetch Nodes.
    VPathNode *srcNode = object->getNode( dAtoi( argv[2] ) );
    VPathNode *dstNode = object->getNode( dAtoi( argv[3] ) );

    // Interp Time.
    const F32 &interp = dAtof( argv[4] );

    // Fetch Position & Orientation.
    const Point3F position    = object->getPathPosition( srcNode, dstNode, interp, true );
    const VectorF orientation = object->getPathOrientation( srcNode, dstNode, interp, true );

    // Y-Axis.
    VectorF yVec = orientation;
    yVec.normalize();

    // X-Axis.
    VectorF xVec = mCross( yVec, VPath::gBezierUp );
    xVec.normalize();

    // Z-Axis.
    VectorF zVec = mCross( xVec, yVec );
    zVec.normalize();

    // Setup Object Transform.
    MatrixF mat( true );
    mat.setColumn( 0, xVec );
    mat.setColumn( 1, yVec );
    mat.setColumn( 2, zVec );

    // AngAxis.
    AngAxisF aa( mat );

    // Return Buffer;
    char *buffer = Con::getReturnBuffer( 256 );
    dSprintf( buffer, 256, "%g %g %g %g %g %g %g", position.x, position.y, position.z,
                                                   aa.axis.x, aa.axis.y, aa.axis.z, aa.angle );

    // Return.
    return buffer;
}

ConsoleMethod( VPath, getPathPosition, const char *, 5, 5, "( int pSrcNodeIndex, int pDstNodeIndex, int pTimeInterp ) - Get the world position of the path at the interp point between two nodes.\n"
                                                           "@param pSrcNodeIndex The first node.\n"
                                                           "@param pDstNodeIndex The second node.\n"
                                                           "@param pTimeInterp The time to interp between the two nodes. Value is between 0.0 and 1.0.\n"
                                                           "@return Returns the world position of the interp time between the two given nodes." )
{
    // Fetch Nodes.
    VPathNode *srcNode = object->getNode( dAtoi( argv[2] ) );
    VPathNode *dstNode = object->getNode( dAtoi( argv[3] ) );

    // Interp Time.
    const F32 &interp = dAtof( argv[4] );

    // Find Position.
    const Point3F position = object->getPathPosition( srcNode, dstNode, interp, true );

    // Return Buffer;
    char *buffer = Con::getReturnBuffer( 128 );
    dSprintf( buffer, 128, "%g %g %g", position.x, position.y, position.z );

    // Return.
    return buffer;
}

Point3F VPath::getPathPosition( VPathNode *pSourceNode, VPathNode *pDestinationNode, const F32 &pTimeInterp, const bool &pForward )
{
    F32 pathInterp = 0.f;
    return getPathPosition( pSourceNode, pDestinationNode, pTimeInterp, pForward, pathInterp );
}

Point3F VPath::getPathPosition( VPathNode *pSourceNode, VPathNode *pDestinationNode, const F32 &pTimeInterp, const bool &pForward, F32 &pPathInterp )
{
    switch( mPathType )
    {
        case k_PathBezier :
            {
                return getBezierPathPosition( pSourceNode, pDestinationNode, pTimeInterp, pForward, pPathInterp );

            } break;

        case k_PathLinear :
            {
                return getLinearPathPosition( pSourceNode, pDestinationNode, pTimeInterp, pForward, pPathInterp );

            } break;
    }

    // NULL.
    return Point3F::Zero;
}

VectorF VPath::getPathOrientation( VPathNode *pSourceNode, VPathNode *pDestinationNode, const F32 &pTimeInterp, const bool &pForward )
{
    switch( mPathType )
    {
        case k_PathBezier :
            {
                return getBezierPathOrientation( pSourceNode, pDestinationNode, pTimeInterp, pForward );

            } break;

        case k_PathLinear :
            {
                return getLinearPathOrientation( pSourceNode, pDestinationNode, pTimeInterp, pForward );

            } break;
    }

    // NULL.
    return VectorF::Zero;
}

//-----------------------------------------------------------------------------
//
// Linear Path Methods.
//
//-----------------------------------------------------------------------------

void VPath::calculateLinearPath( VPathNode *pNode, VPathNode *pNextNode )
{
    // Calculate Segment Length.
    pNode->setLength( ( pNextNode->getWorldPosition() - pNode->getWorldPosition() ).len() );
}

Point3F VPath::getAdvancedLinearPathPosition( VPathObject *pPathObject, const F32 &pTargetDistance, F32 &pPathInterpDelta )
{
    // Fetch Nodes.
    VPathNode *srcNode = mNodeList[pPathObject->getSourceNode()];
    VPathNode *dstNode = mNodeList[pPathObject->getDestinationNode()];

    // Fetch the length of the segment.
    const F32 length = ( pPathObject->isForward() ) ? srcNode->getLength() : dstNode->getLength();

    // Set the interp delta.
    pPathInterpDelta = ( pTargetDistance / length );

    // Return the position.
    F32 pathInterp = 0.f;
    return getLinearPathPosition( srcNode, dstNode, pPathObject->getPathInterp(), pPathObject->isForward(), pathInterp );
}

Point3F VPath::getLinearPathPosition( VPathNode *pSourceNode, VPathNode *pDestinationNode, const F32 &pTimeInterp, const bool &pForward, F32 &pPathInterp )
{
    // Set path interp to the time interp.
    pPathInterp = pTimeInterp;

    if ( pTimeInterp <= 0.f )
    {
        // Source Node.
        return pSourceNode->getWorldPosition();
    }
    else if ( pTimeInterp >= 1.f )
    {
        // Destination Node.
        return pDestinationNode->getWorldPosition();
    }

    // Calculate Position.
    Point3F position;
    position.interpolate( pSourceNode->getWorldPosition(), pDestinationNode->getWorldPosition(), pTimeInterp );

    // Return.
    return position;
}

VectorF VPath::getLinearPathOrientation( VPathNode *pSourceNode, VPathNode *pDestinationNode, const F32 &pTimeInterp, const bool &pForward )
{
    // Calculate Orientation.
    VectorF newOrientation = ( pDestinationNode->getWorldPosition() - pSourceNode->getWorldPosition() );
    newOrientation.normalizeSafe();

    // Return.
    return newOrientation;
}

//-----------------------------------------------------------------------------
//
// Bezier Path Methods.
//
//-----------------------------------------------------------------------------

void VPath::calculateBezierPath( VPathNode *pNode, VPathNode *pNextNode )
{
    // Reset Length.
    F32 segmentLength = 0.f;

    // Positions.
    const Point3F &pt0 = pNode->getWorldPosition();
    const Point3F &pt3 = pNextNode->getWorldPosition();

    // Fetch Node Rotation Matrices.
    MatrixF mat0, mat1;
    pNode->getWorldRotation().setMatrix( &mat0 );
    pNextNode->getWorldRotation().setMatrix( &mat1 );

    // Determine Tangent Axis.
    Point3F pt1(  gBezierAxis * pNode->getWeight() );
    Point3F pt2( -gBezierAxis * pNextNode->getWeight() );

    // Rotate Axis.
    mat0.mulP( pt1 );
    mat1.mulP( pt2 );

    // Offset Points.
    pt1 += pt0;
    pt2 += pt3;

    // Initial Position.
    Point3F ptA = pt0;
    const F32 i = gBezierInterpStep;
    for ( F32 t = 0.f, it = ( 1.f - t ); t <= 1.f; t += i, it = ( 1.f - t ) )
    {
        // Calculate Position.
        Point3F ptB = ( pt0 * it * it * it ) + ( 3 * pt1 * it * it * t ) + ( 3 * pt2 * it * t * t ) + ( pt3 * t * t * t );

        // Add Segment.
        segmentLength += ( ptB - ptA ).len();

        // Store Position.
        ptA = ptB;
    }

    // Apply Update.
    pNode->setLength( segmentLength );
}

Point3F VPath::getAdvancedBezierPathPosition( VPathObject *pPathObject, const F32 &pTargetDistance, F32 &pPathInterpDelta )
{
    // Fetch Nodes.
    VPathNode *srcNode = mNodeList[pPathObject->getSourceNode()];
    VPathNode *dstNode = mNodeList[pPathObject->getDestinationNode()];

    // Fetch the delta position.
    return getBezierPathPosition( srcNode, dstNode, pPathObject->getPathInterp(), pPathObject->getPosition(), pTargetDistance, pPathObject->isForward(), true, pPathInterpDelta );
}

Point3F VPath::getBezierPathPosition( VPathNode *pSourceNode, VPathNode *pDestinationNode, const F32 &pTimeInterp, const bool &pForward, F32 &pPathInterp )
{
    // Fetch the length of the segment.
    const F32 length = ( pForward ) ? pSourceNode->getLength() : pDestinationNode->getLength();

    // Determine the real interp time for the distance fraction.
    return getBezierPathPosition( pSourceNode, pDestinationNode, 0.f, pSourceNode->getWorldPosition(), ( length * pTimeInterp ), pForward, false, pPathInterp );
}

Point3F VPath::getBezierPathPosition( VPathNode *pSourceNode, VPathNode *pDestinationNode, const F32 &pTimeInterp, const Point3F &pReferencePosition, const F32 &pTargetDistance, const bool &pForward, const bool &pRelativeToReference, F32 &pPathInterpDelta )
{
    // Positions.
    const Point3F &pt0 = pSourceNode->getWorldPosition();
    const Point3F &pt3 = pDestinationNode->getWorldPosition();

    // Fetch Node Rotation Matrices.
    MatrixF mat0, mat1;
    pSourceNode->getWorldRotation().setMatrix( &mat0 );
    pDestinationNode->getWorldRotation().setMatrix( &mat1 );

    // Determine Tangent Axis.
    Point3F pt1(  gBezierAxis * pSourceNode->getWeight() );
    Point3F pt2( -gBezierAxis * pDestinationNode->getWeight() );

    if ( !pForward )
    {
        pt1 *= -1.f;
        pt2 *= -1.f;
    }

    // Rotate Axis.
    mat0.mulP( pt1 );
    mat1.mulP( pt2 );

    // Offset Points.
    pt1 += pt0;
    pt2 += pt3;

    // Move Position.
    Point3F movePosition = pReferencePosition;
    // Movement Distance.
    F32 moveDistance = 0.f;

    // Determine the Real Delta.
    const F32 i = gBezierInterpStep;
    for ( F32 t = ( pTimeInterp + i ), it = ( 1.f - t ); t <= 1.f; t += i, it = ( 1.f - t ) )
    {
        // Calculate Step.
        const Point3F stepPosition = ( pt0 * it * it * it ) + ( 3 * pt1 * it * it * t ) + ( 3 * pt2 * it * t * t ) + ( pt3 * t * t * t );
        // Step Length.
        const F32 &stepDistance = ( stepPosition - movePosition ).len();

        if ( pRelativeToReference )
        {
            // Calculate Distance.
            moveDistance = ( pReferencePosition - stepPosition ).len();

            // Moved Target Distance?
            if ( moveDistance >= pTargetDistance )
            {
                // Interpolate Step.
                const F32 stepInterp = ( moveDistance - pTargetDistance ) / moveDistance;
                // Store Interp Delta.
                pPathInterpDelta = ( t - pTimeInterp ) * ( 1.f - stepInterp );
                
                // Interpolate the step.
                Point3F outPosition;
                outPosition.interpolate( pReferencePosition, stepPosition, ( 1.f - stepInterp ) );
                // Return the position.
                return outPosition;
            }
        }
        else
        {
            // Calculate Distance.
            moveDistance += stepDistance;

            // Moved Target Distance?
            if ( moveDistance >= pTargetDistance )
            {
                // Interpolate Step.
                const F32 stepInterp = ( moveDistance - pTargetDistance ) / stepDistance;
                // Store Interp Delta.
                pPathInterpDelta = ( t - pTimeInterp ) - ( stepInterp * i );
                
                // Interpolate the step.
                Point3F outPosition;
                outPosition.interpolate( movePosition, stepPosition, ( 1.f - stepInterp ) );
                // Return the position.
                return outPosition;
            }
        }

        // Apply New Position.
        movePosition = stepPosition;
    }
    
    // Update.
    pPathInterpDelta = ( 1.f - pTimeInterp );
    // At the destination node?
    return pt3;
}

VectorF VPath::getBezierPathOrientation( VPathNode *pSourceNode, VPathNode *pDestinationNode, const F32 &pTimeInterp, const bool &pForward )
{
    // Positions.
    const Point3F &pt0 = pSourceNode->getWorldPosition();
    const Point3F &pt3 = pDestinationNode->getWorldPosition();

    // Fetch Node Rotation Matrices.
    MatrixF mat0, mat1;
    pSourceNode->getWorldRotation().setMatrix( &mat0 );
    pDestinationNode->getWorldRotation().setMatrix( &mat1 );

    // Determine Tangent Axis.
    Point3F pt1(  gBezierAxis * pSourceNode->getWeight() );
    Point3F pt2( -gBezierAxis * pDestinationNode->getWeight() );

    if ( !pForward )
    {
        pt1 *= -1.f;
        pt2 *= -1.f;
    }

    // Rotate Axis.
    mat0.mulP( pt1 );
    mat1.mulP( pt2 );

    const F32 halfStep = ( gBezierInterpStep / 2.f );
    if ( ( pTimeInterp - halfStep ) <= 0.f )
    {
        // Orientation From Node Tangent.
        pt1.normalize();

        // Return.
        return pt1;
    }
    else if ( ( pTimeInterp + halfStep ) >= 1.f )
    {
        // Orientation From Node Tangent.
        pt2.normalize();

        // Return.
        return -pt2;
    }

    // Offset Points.
    pt1 += pt0;
    pt2 += pt3;

    // Interp Times.
    const F32 t0  = ( pTimeInterp - halfStep );
    const F32 it0 = ( 1.f - t0 );

    const F32 t1  = ( pTimeInterp + halfStep );
    const F32 it1 = ( 1.f - t1 );

    // Calculate Position.
    Point3F d0 = ( pt0 * it0 * it0 * it0 ) + ( 3 * pt1 * it0 * it0 * t0 ) + ( 3 * pt2 * it0 * t0 * t0 ) + ( pt3 * t0 * t0 * t0 );
    Point3F d1 = ( pt0 * it1 * it1 * it1 ) + ( 3 * pt1 * it1 * it1 * t1 ) + ( 3 * pt2 * it1 * t1 * t1 ) + ( pt3 * t1 * t1 * t1 );

    // Set Orientation.
    Point3F orientation = ( d1 - d0 );
    orientation.normalizeSafe();

    // Return.
    return orientation;
}

//-----------------------------------------------------------------------------
//
// Path Node Property Methods.
//
//-----------------------------------------------------------------------------

ConsoleMethod( VPath, getNodeCount, S32, 2, 2, "() - Get the number of nodes in this path.\n"
                                               "@return Returns the number of nodes." )
{
    // Return Count.
    return object->getNodeCount();
}

S32 VPath::getNodeCount( void )
{
    // Return the Size of the Node List.
    return mNodeList.size();
}

ConsoleMethod( VPath, getNodeLocalTransform, const char *, 3, 3, "( int pNodeIndex ) - Get the local transform (local position and rotation) of the given node.\n"
                                                                 "@param pNodeIndex The index of the node.\n"
                                                                 "@return Returns the transform of the given node." )
{
    // Fetch Position.
    const Point3F &position = object->getNodeLocalPosition( dAtoi( argv[2] ) );

    // Fetch Rotation.
    const QuatF   &rotation = object->getNodeLocalRotation( dAtoi( argv[2] ) );

    // Angle & Axis.
    AngAxisF aa( rotation );

    // Return Buffer.
    char *buffer = Con::getReturnBuffer( 256 );
    dSprintf( buffer, 128, "%.3g %.3g %.3g %.3g %.3g %.3g %.3g", position.x, position.y, position.z, aa.axis.x, aa.axis.y, aa.axis.z, aa.angle );

    return buffer;
}

ConsoleMethod( VPath, getNodeLocalPosition, const char *, 3, 3, "( int pNodeIndex ) - Get the position of the given node.\n"
                                                                "@param pNodeIndex The index of the node.\n"
                                                           "    @return Returns the Local Position of the given node." )
{
    // Fetch Position.
    const Point3F &position = object->getNodeLocalPosition( dAtoi( argv[2] ) );

    // Return Buffer.
    char *buffer = Con::getReturnBuffer( 128 );
    dSprintf( buffer, 128, "%.3g %.3g %.3g", position.x, position.y, position.z );

    return buffer;
}

Point3F VPath::getNodeLocalPosition( const S32 &pNodeIndex )
{
    if ( pNodeIndex < 0 || pNodeIndex >= mNodeList.size() )
    {
        // Woops!
        Con::warnf( "VPath::getNodeLocalPosition() - Invalid Index Specified (%d).", pNodeIndex );
        return Point3F::Zero;
    }

    return mNodeList[pNodeIndex]->getLocalPosition();
}

ConsoleMethod( VPath, getNodeLocalRotation, const char *, 3, 3, "( int pNodeIndex ) - Get the Local Rotation of the given node.\n"
                                                                "@param pNodeIndex The index of the node.\n"
                                                                "@return Returns the Local Rotation of the given node." )
{
    // Fetch Rotation.
    const QuatF &rotation = object->getNodeLocalRotation( dAtoi( argv[2] ) );

    // Angle & Axis.
    AngAxisF aa( rotation );

    // Return Buffer.
    char *buffer = Con::getReturnBuffer( 128 );
    dSprintf( buffer, 128, "%.3g %.3g %.3g %.3g", aa.axis.x, aa.axis.y, aa.axis.z, aa.angle );

    return buffer;
}

QuatF VPath::getNodeLocalRotation( const S32 &pNodeIndex )
{
    if ( pNodeIndex < 0 || pNodeIndex >= mNodeList.size() )
    {
        // Woops!
        Con::warnf( "VPath::getNodeLocalRotation() - Invalid Index Specified (%d).", pNodeIndex );
        return QuatF( Point3F::Zero, 0.f );
    }

    return mNodeList[pNodeIndex]->getLocalRotation();
}

ConsoleMethod( VPath, getNodeWorldTransform, const char *, 3, 3, "( int pNodeIndex ) - Get the World Transform (position and rotation) of the given node.\n"
                                                                 "@param pNodeIndex The index of the node.\n"
                                                                 "@return Returns the transform of the given node." )
{
    // Fetch Position.
    const Point3F &position = object->getNodeWorldPosition( dAtoi( argv[2] ) );

    // Fetch Rotation.
    const QuatF   &rotation = object->getNodeWorldRotation( dAtoi( argv[2] ) );

    // Angle & Axis.
    AngAxisF aa( rotation );

    // Return Buffer.
    char *buffer = Con::getReturnBuffer( 256 );
    dSprintf( buffer, 128, "%.3g %.3g %.3g %.3g %.3g %.3g %.3g", position.x, position.y, position.z, aa.axis.x, aa.axis.y, aa.axis.z, aa.angle );

    return buffer;
}

ConsoleMethod( VPath, getNodeWorldPosition, const char *, 3, 3, "( int pNodeIndex ) - Get the position of the given node.\n"
                                                                "@param pNodeIndex The index of the node.\n"
                                                                "@return Returns the World Position of the given node." )
{
    // Fetch Position.
    const Point3F &position = object->getNodeWorldPosition( dAtoi( argv[2] ) );

    // Return Buffer.
    char *buffer = Con::getReturnBuffer( 128 );
    dSprintf( buffer, 128, "%.3g %.3g %.3g", position.x, position.y, position.z );

    return buffer;
}

Point3F VPath::getNodeWorldPosition( const S32 &pNodeIndex )
{
    if ( pNodeIndex < 0 || pNodeIndex >= mNodeList.size() )
    {
        // Woops!
        Con::warnf( "VPath::getNodeWorldPosition() - Invalid Index Specified (%d).", pNodeIndex );
        return Point3F::Zero;
    }

    return mNodeList[pNodeIndex]->getWorldPosition();
}

ConsoleMethod( VPath, getNodeWorldRotation, const char *, 3, 3, "( int pNodeIndex ) - Get the World Rotation of the given node.\n"
                                                                "@param pNodeIndex The index of the node.\n"
                                                                "@return Returns the World Rotation of the given node." )
{
    // Fetch Rotation.
    const QuatF &rotation = object->getNodeWorldRotation( dAtoi( argv[2] ) );

    // Angle & Axis.
    AngAxisF aa( rotation );

    // Return Buffer.
    char *buffer = Con::getReturnBuffer( 128 );
    dSprintf( buffer, 128, "%.3g %.3g %.3g %.3g", aa.axis.x, aa.axis.y, aa.axis.z, aa.angle );

    return buffer;
}

QuatF VPath::getNodeWorldRotation( const S32 &pNodeIndex )
{
    if ( pNodeIndex < 0 || pNodeIndex >= mNodeList.size() )
    {
        // Woops!
        Con::warnf( "VPath::getNodeWorldRotation() - Invalid Index Specified (%d).", pNodeIndex );
        return QuatF( Point3F::Zero, 0.f );
    }

    return mNodeList[pNodeIndex]->getWorldRotation();
}

ConsoleMethod( VPath, getNodeWeight, F32, 3, 3, "( int pNodeIndex ) - Get the weight of the given node.\n"
                                                "@param pNodeIndex The index of the node.\n"
                                                "@return Returns the weight of the given node." )
{
    // Fetch Weight.
    return object->getNodeWeight( dAtoi( argv[2] ) );
}

F32 VPath::getNodeWeight( const S32 &pNodeIndex )
{
    if ( pNodeIndex < 0 || pNodeIndex >= mNodeList.size() )
    {
        // Woops!
        Con::warnf( "VPath::getNodeWeight() - Invalid Index Specified (%d).", pNodeIndex );
        return 0.f;
    }

    return mNodeList[pNodeIndex]->getWeight();
}

ConsoleMethod( VPath, getNodeLength, F32, 3, 3, "( int pNodeIndex ) - Get the length of the given node.\n"
                                                "@param pNodeIndex The index of the node.\n"
                                                "@return Returns the length of the given node." )
{
    // Fetch Length.
    return object->getNodeLength( dAtoi( argv[2] ) );
}

F32 VPath::getNodeLength( const S32 &pNodeIndex )
{
    if ( pNodeIndex < 0 || pNodeIndex >= mNodeList.size() )
    {
        // Woops!
        Con::warnf( "VPath::getNodeLength() - Invalid Index Specified (%d).", pNodeIndex );
        return 0.f;
    }

    return mNodeList[pNodeIndex]->getLength();
}

ConsoleMethod( VPath, setNodeTransform, void, 4, 4, "( int pNodeIndex, matrix pTransform ) - Set the transform of the given node.\n"
                                                    "@param pNodeIndex The index of the node.\n"
                                                    "@param pTransform The new transform to be applied to the node.\n"
                                                    "@return No return value." )
{
    // Fetch Index.
    const S32 nodeIndex = dAtoi( argv[2] );
    
    // Fetch Position & Rotation.
    Point3F position;
    AngAxisF aa;
    QuatF    rotation;
    dSscanf( argv[3], "%g %g %g %g %g %g %g", &position.x, &position.y, &position.z,
                                              &aa.axis.x, &aa.axis.y, &aa.axis.z, &aa.angle );

    // Set Rotation.
    rotation.set( aa );

    // Apply Update.
    object->setNodePosition( nodeIndex, position );
    object->setNodeRotation( nodeIndex, rotation );
}

ConsoleMethod( VPath, setNodePosition, void, 4, 4, "( int pNodeIndex, vector pPosition ) - Set the position of the given node.\n"
                                                   "@param pNodeIndex The index of the node.\n"
                                                   "@param pPosition The new position to be applied to the node.\n"
                                                   "@return No return value." )
{
    // Fetch Index.
    const S32 nodeIndex = dAtoi( argv[2] );
    
    // Fetch Position.
    Point3F position;
    dSscanf( argv[3], "%g %g %g", &position.x, &position.y, &position.z );

    // Apply Update.
    object->setNodePosition( nodeIndex, position );
}

void VPath::setNodePosition( const S32 &pNodeIndex, const Point3F &pPosition )
{
    if ( pNodeIndex < 0 || pNodeIndex >= mNodeList.size() )
    {
        // Woops!
        Con::warnf( "VPath::setNodePosition() - Invalid Index Specified (%d).", pNodeIndex );
        return;
    }

    // Fetch Node.
    VPathNode *node = mNodeList[pNodeIndex];

    // Apply Update.
    node->setLocalPosition( pPosition );

    // Update Size.
    updateContainer();

    // Calculate Path.
    calculatePath();

    if ( isServerObject() )
    {
        // Network Flags.
        setMaskBits( NodeUpdateMask );
    }
}

ConsoleMethod( VPath, setNodeRotation, void, 4, 4, "( int pNodeIndex, angAxis pRotation ) - Set the rotation of the given node.\n"
                                                   "@param pNodeIndex The index of the node.\n"
                                                   "@param pRotation The new rotation to be applied to the node.\n"
                                                   "@return No return value."  )
{
    // Fetch Index.
    const S32 nodeIndex = dAtoi( argv[2] );

    // Fetch Rotation.
    AngAxisF aa;
    QuatF    rotation;
    dSscanf( argv[3], "%g %g %g %g", &aa.axis.x, &aa.axis.y, &aa.axis.z, &aa.angle );

    // Set Rotation.
    rotation.set( aa );

    // Apply Update.
    object->setNodeRotation( nodeIndex, rotation );
}

void VPath::setNodeRotation( const S32 &pNodeIndex, const QuatF &pRotation )
{
    if ( pNodeIndex < 0 || pNodeIndex >= mNodeList.size() )
    {
        // Woops!
        Con::warnf( "VPath::setNodeRotation() - Invalid Index Specified (%d).", pNodeIndex );
        return;
    }

    // Fetch Node.
    VPathNode *node = mNodeList[pNodeIndex];

    // Apply Update.
    node->setLocalRotation( pRotation );

    // Calculate Path.
    calculatePath();

    if ( isServerObject() )
    {
        // Network Flags.
        setMaskBits( NodeUpdateMask );
    }
}

ConsoleMethod( VPath, setNodeWeight, void, 4, 4, "( int pNodeIndex, float pWeight ) - Set the weight of the given node.\n"
                                                 "@param pNodeIndex The index of the node.\n"
                                                 "@param pWeight The new weight to be applied to the node.\n"
                                                 "@return No return value."  )
{
    // Fetch Index.
    const S32 nodeIndex = dAtoi( argv[2] );

    // Fetch Weight.
    const F32 nodeWeight = dAtof( argv[3] );

    // Apply Update.
    object->setNodeWeight( nodeIndex, nodeWeight );
}

void VPath::setNodeWeight( const S32 &pNodeIndex, const F32 &pWeight )
{
    if ( pNodeIndex < 0 || pNodeIndex >= mNodeList.size() )
    {
        // Woops!
        Con::warnf( "VPath::setNodeWeight() - Invalid Index Specified (%d).", pNodeIndex );
        return;
    }

    // Fetch Node.
    VPathNode *node = mNodeList[pNodeIndex];

    // Apply Update.
    node->setWeight( pWeight );

    // Calculate Path.
    calculatePath();

    if ( isServerObject() )
    {
        // Network Flags.
        setMaskBits( NodeUpdateMask );
    }
}

ConsoleMethod( VPath, getNodeOrientationMode, const char *, 3, 3, "( int pNodeIndex ) - Gets the current orientation mode of the node.\n"
                                                                  "@param pNodeIndex The index of the node.\n"
                                                                  "@return Returns a string indicating the orientation mode and its properties." )
{
    // Fetch Index.
    const S32 nodeIndex = dAtoi( argv[2] );

    if ( nodeIndex < 0 || nodeIndex >= object->getNodeCount() )
    {
        // Woops!
        Con::warnf( "VPath::getNodeOrientationMode() - Invalid Index Specified (%d).", nodeIndex );
        return "";
    }

    // Fetch Object
    VPathNode *node = object->getNode( nodeIndex );

    // Fetch Orientation Mode.
    const VPathNode::sOrientation &orientation = node->getOrientationMode();

    // Determine the Type.
    StringTableEntry type = VPathNode::getOrientationTypeLabel( orientation.Type );

    // Buffer.
    char *buffer = Con::getReturnBuffer( 128 );

    switch( orientation.Type )
    {
        case VPathNode::k_OrientationFree :
            {
                // Buffer String.
                dSprintf( buffer, 128, "%s", type );

            } break;

        case VPathNode::k_OrientationToPoint:
            {
                // Fetch Point.
                const Point3F &lookAtPoint = orientation.Point;
                // Buffer String.
                dSprintf( buffer, 128, "%s\t%.2f %.2f %.2f", type, lookAtPoint.x, lookAtPoint.y, lookAtPoint.z );

            } break;
    }

    // Return Buffer.
    return buffer;
}

ConsoleMethod( VPath, setNodeOrientationMode, void, 4, 5, "( int pNodeIndex, string pOrientationType, [vector pPoint] ) - Set the orientation mode of the node.\n"
                                                          "@param pNodeIndex The index of the node.\n"
                                                          "@param pOrientationType The new orientation type of the object.\n"
                                                          "@param pPoint If the orientation type is set to POINT, this parameter must be a vector.\n"
                                                          "@return No return value." )
{
    // Fetch Index.
    const S32 nodeIndex = dAtoi( argv[2] );

    // Orient?
    const VPathNode::eOrientationType type = VPathNode::getOrientationTypeEnum( argv[3] );

    switch ( type )
    {
        case VPathNode::k_OrientationFree :
            {
                // Apply Mode.
                object->setNodeOrientationMode( nodeIndex, type );

            } break;

        case VPathNode::k_OrientationToPoint:
            {
                // Fetch Point.
                Point3F lookAtPoint( 0.f, 0.f, 0.f );
                dSscanf( argv[4], "%g %g %g", &lookAtPoint.x, &lookAtPoint.y, &lookAtPoint.z );

                // Apply Mode.
                object->setNodeOrientationMode( nodeIndex, type, lookAtPoint );

            } break;

        default :
            {
                AssertFatal( false, "VPath::setNodeOrientationMode() - Invalid Orientation Mode Specified." );

            } break;
    }
}

void VPath::setNodeOrientationMode( const S32 &pNodeIndex, const VPathNode::eOrientationType &pType )
{
    if ( pNodeIndex < 0 || pNodeIndex >= mNodeList.size() )
    {
        // Woops!
        Con::warnf( "VPath::setNodeOrientationMode() - Invalid Index Specified (%d).", pNodeIndex );
        return;
    }

    // Fetch Node.
    VPathNode *node = mNodeList[pNodeIndex];

    // Apply.
    node->setOrientationMode( pType );

    // Network Flags.
    setMaskBits( NodeUpdateMask );
}

void VPath::setNodeOrientationMode( const S32 &pNodeIndex, const VPathNode::eOrientationType &pType, const Point3F pPoint )
{
    if ( pNodeIndex < 0 || pNodeIndex >= mNodeList.size() )
    {
        // Woops!
        Con::warnf( "VPath::setNodeOrientationMode() - Invalid Index Specified (%d).", pNodeIndex );
        return;
    }

    // Fetch Node.
    VPathNode *node = mNodeList[pNodeIndex];

    // Apply.
    node->setOrientationMode( pType, pPoint );

    // Network Flags.
    setMaskBits( NodeUpdateMask );
}

//-----------------------------------------------------------------------------
//
// Path Object Property Methods.
//
//-----------------------------------------------------------------------------

ConsoleMethod( VPath, isPathObjectActive, bool, 3, 3, "( SimObject pObject ) - Is the object actively traveling around this path?\n"
                                                      "@param pObject The SimObjectID of the object being observed.\n"
                                                      "@return Returns true of the object is active." )
{
    // Fetch Object.
    SceneObject *sceneObject;
    if ( !Sim::findObject( argv[2], sceneObject ) )
    {
        Con::errorf( "VPath::isPathObjectActive() - Invalid Target Object." );
        return false;
    }

    // Fetch Object
    VPathObject *pathObject = object->getPathObject( sceneObject );

    // Return.
    return pathObject->isActive();
}

ConsoleMethod( VPath, setPathObjectActive, void, 4, 4, "( SimObject pObject, bool pActive ) - Enable or disable the object from traveling around this path. Inactive objects are still attached to the path, but are not updated.\n"
                                                       "@param pObject The SimObjectID of the object being altered.\n"
                                                       "@param pActive The new status of the object.\n"
                                                       "@return No return value." )
{
    // Fetch Object.
    SceneObject *sceneObject;
    if ( !Sim::findObject( argv[2], sceneObject ) )
    {
        Con::errorf( "VPath::setPathObjectActive() - Invalid Target Object." );
        return;
    }

    // Apply.
    object->setPathObjectActive( sceneObject, dAtob( argv[3] ) );
}

void VPath::setPathObjectActive( SceneObject *pObject, const bool &pActive )
{
    VPathObject *pathObject = getPathObject( pObject );
    if ( !pathObject )
    {
        Con::warnf( "VPath::setPathObjectActive() - Object (%d) Not Attached to Path.", pObject->getId() );
        return;
    }

    // Apply.
    pathObject->setActive( pActive );

    // Network Flags.
    setMaskBits( ObjectUpdateMask );
}

ConsoleMethod( VPath, getPathObjectInterp, F32, 3, 3, "( SimObject pObject ) - Get the current interp position of the path object.\n"
                                                      "@param pObject The SimObjectID of the object being observed.\n"
                                                      "@return Returns the current interp position." )
{
    // Fetch Object.
    SceneObject *sceneObject;
    if ( !Sim::findObject( argv[2], sceneObject ) )
    {
        Con::errorf( "VPath::getPathObjectInterp() - Invalid Target Object." );
        return false;
    }

    // Fetch Object
    VPathObject *pathObject = object->getPathObject( sceneObject );

    // Return.
    return pathObject->getTimeInterp();
}

ConsoleMethod( VPath, setPathObjectInterp, void, 4, 4, "( SimObject pObject, float pTimeInterp ) - Set the interp position of the object between its current nodes.\n"
                                                       "@param pObject The SimObjectID of the object being altered.\n"
                                                       "@param pTimeInterp The new interp position of the object.\n"
                                                       "@return No return value." )
{
    // Fetch Object.
    SceneObject *sceneObject;
    if ( !Sim::findObject( argv[2], sceneObject ) )
    {
        Con::errorf( "VPath::setPathObjectInterp() - Invalid Target Object." );
        return;
    }

    // Apply.
    object->setPathObjectInterp( sceneObject, dAtof( argv[3] ) );
}

void VPath::setPathObjectInterp( SceneObject *pObject, const F32 &pTimeInterp )
{
    VPathObject *pathObject = getPathObject( pObject );
    if ( !pathObject )
    {
        Con::warnf( "VPath::setPathObjectInterp() - Object (%d) Not Attached to Path.", pObject->getId() );
        return;
    }

    // Update.
    setPathObjectInterp( pathObject, pTimeInterp );
}

void VPath::setPathObjectInterp( VPathObject *pPathObject, const F32 &pTimeInterp )
{
    // Set Interp Time.
    pPathObject->setTimeInterp( pTimeInterp );

    // Update Position.
    updatePosition( pPathObject );
    // Update Orientation.
    updateOrientation( pPathObject );
    // Reset the delta.
    pPathObject->resetDelta();

    // Set the object transform.
    pPathObject->getObject()->setTransform( pPathObject->getTransform() );

    if ( isServerObject() )
    {
        // Update Objects.
        setMaskBits( ObjectUpdateMask );

        // Update This Object.
        pPathObject->setMaskBits( VPathObject::k_StateUpdatePosition );
    }
}

ConsoleMethod( VPath, getPathObjectOffset, const char *, 3, 3, "( SimObject pObject ) - Get the position offset assigned to this object.\n"
                                                      "@param pObject The SimObjectID of the object being observed.\n"
                                                      "@return Returns the position offset." )
{
    // Fetch Object.
    SceneObject *sceneObject;
    if ( !Sim::findObject( argv[2], sceneObject ) )
    {
        Con::errorf( "VPath::getPathObjectOffset() - Invalid Target Object." );
        return false;
    }

    // Fetch Object
    VPathObject *pathObject = object->getPathObject( sceneObject );

    // Fetch Offset.
    const Point3F &offset = pathObject->getOffset();

    // Buffer.
    char *buffer = Con::getReturnBuffer( 64 );
    dSprintf( buffer, 64, "%f %f %f", offset.x, offset.y, offset.z );
    return buffer;
}

ConsoleMethod( VPath, setPathObjectOffset, void, 4, 4, "( SimObject pObject, vector pOffset ) - Set the position offset of the object. As the object is moving along the path, its position is offset by this value. Setting the \"Relative\" parameter while attaching an object will automatically apply an offset value.\n"
                                                       "@param pObject The SimObjectID of the object being altered.\n"
                                                       "@param pOffset The new position offset of the object.\n"
                                                       "@return No return value." )
{
    // Fetch Object.
    SceneObject *sceneObject;
    if ( !Sim::findObject( argv[2], sceneObject ) )
    {
        Con::errorf( "VPath::setPathObjectOffset() - Invalid Target Object." );
        return;
    }

    // Relative Offset.
    Point3F offset( 0.f, 0.f, 0.f );
    // Scan.
    dSscanf( argv[3], "%g %g %g", &offset.x, &offset.y, &offset.z );

    // Apply.
    object->setPathObjectOffset( sceneObject, offset );
}

void VPath::setPathObjectOffset( SceneObject *pObject, const Point3F &pOffset )
{
    VPathObject *pathObject = getPathObject( pObject );
    if ( !pathObject )
    {
        Con::warnf( "VPath::setPathObjectOffset() - Object (%d) Not Attached to Path.", pObject->getId() );
        return;
    }

    // Apply.
    pathObject->setOffset( pOffset );

    // Network Flags.
    setMaskBits( ObjectUpdateMask );
}

ConsoleMethod( VPath, getPathObjectSpeed, F32, 3, 3, "( SimObject pObject ) - Get the speed this object is traveling along the path at.\n"
                                                     "@param pObject The SimObjectID of the object being observed.\n"
                                                     "@return Returns the speed of the object." )
{
    // Fetch Object.
    SceneObject *sceneObject;
    if ( !Sim::findObject( argv[2], sceneObject ) )
    {
        Con::errorf( "VPath::getPathObjectSpeed() - Invalid Target Object." );
        return false;
    }

    // Fetch Object
    VPathObject *pathObject = object->getPathObject( sceneObject );

    // Return.
    return pathObject->getSpeed();
}

ConsoleMethod( VPath, setPathObjectSpeed, void, 4, 4, "( SimObject pObject, float pSpeed ) - Set the speed of the object.\n"
                                                      "@param pObject The SimObjectID of the object being altered.\n"
                                                      "@param pSpeed The new speed of the object.\n"
                                                      "@return No return value." )
{
    // Fetch Object.
    SceneObject *sceneObject;
    if ( !Sim::findObject( argv[2], sceneObject ) )
    {
        Con::errorf( "VPath::setPathObjectSpeed() - Invalid Target Object." );
        return;
    }

    // Apply.
    object->setPathObjectSpeed( sceneObject, dAtof( argv[3] ) );
}

void VPath::setPathObjectSpeed( SceneObject *pObject, const F32 &pSpeed )
{
    VPathObject *pathObject = getPathObject( pObject );
    if ( !pathObject )
    {
        Con::warnf( "VPath::setPathObjectSpeed() - Object (%d) Not Attached to Path.", pObject->getId() );
        return;
    }

    // Apply.
    pathObject->setSpeed( mFabs( pSpeed ) );

    // Network Flags.
    setMaskBits( ObjectUpdateMask );  
}

ConsoleMethod( VPath, getPathObjectOrientationMode, const char *, 3, 3, "( SimObject pObject ) - Gets the current orientation mode of the object.\n"
                                                                        "@param pObject The SimObjectID of the object being observed.\n"
                                                                        "@return Returns a string indicating the orientation mode and its properties." )
{
    // Fetch Object.
    SceneObject *sceneObject;
    if ( !Sim::findObject( argv[2], sceneObject ) )
    {
        Con::errorf( "VPath::getPathObjectOrientationMode() - Invalid Target Object." );
        return false;
    }

    // Fetch Object
    VPathObject *pathObject = object->getPathObject( sceneObject );

    // Fetch Orientation Mode.
    const VPathObject::sOrientation &orientation = pathObject->getOrientationMode();

    // Determine the Type.
    StringTableEntry type = VPathObject::getOrientationTypeLabel( orientation.Type );

    // Buffer.
    char *buffer = Con::getReturnBuffer( 128 );

    switch( orientation.Type )
    {
        case VPathObject::k_OrientationFree :
        case VPathObject::k_OrientationInterpolate :
        case VPathObject::k_OrientationToPath :
            {
                // Buffer String.
                dSprintf( buffer, 128, "%s", type );

            } break;

        case VPathObject::k_OrientationToObject : 
            {
                // Fetch the Object ID.
                const S32 objId = ( ( orientation.Object ) ? orientation.Object->getId() : 0 );
                // Buffer String.
                dSprintf( buffer, 128, "%s %d", type, objId );

            } break;

        case VPathObject::k_OrientationToPoint:
            {
                // Fetch Point.
                const Point3F &lookAtPoint = orientation.Point;
                // Buffer String.
                dSprintf( buffer, 128, "%s %f %f %f", type, lookAtPoint.x, lookAtPoint.y, lookAtPoint.z );

            } break;
    }

    // Return Buffer.
    return buffer;
}

ConsoleMethod( VPath, setPathObjectOrientationMode, void, 4, 5, "( SimObject pObject, string pOrientationType, [SimObject pObject / vector pPoint] ) - Set the orientation mode of the object. This property affects the rotation of the object. If you wish to ignore the object's rotation altogether, set the mode to \"FREE\".\n"
                                                                "@param pObject The SimObjectID of the object being altered.\n"
                                                                "@param pOrientationType The new orientation type of the object.\n"
                                                                "@param pObject If the orientation type is set to OBJECT, this parameter must be the SimObjectID of a scene object.\n"
                                                                "@param pPoint If the orientation type is set to POINT, this parameter must be a vector.\n"
                                                                "@return No return value." )
{
    // Fetch Object.
    SceneObject *sceneObject;
    if ( !Sim::findObject( argv[2], sceneObject ) )
    {
        Con::errorf( "VPath::setPathObjectOrientationMode() - Invalid Target Object." );
        return;
    }

    // Orient?
    const VPathObject::eOrientationType type = VPathObject::getOrientationTypeEnum( argv[3] );

    switch ( type )
    {
        case VPathObject::k_OrientationFree :
        case VPathObject::k_OrientationInterpolate :
        case VPathObject::k_OrientationToPath :
            {
                // Apply Mode.
                object->setPathObjectOrientationMode( sceneObject, type );

            } break;

        case VPathObject::k_OrientationToObject : 
            {
                // Fetch Object.
                SceneObject *lookAtObject = dynamic_cast<SceneObject*>( Sim::findObject( argv[4] ) );
                if ( !lookAtObject )
                {
                    Con::errorf( "VPath::setPathObjectOrientationMode() - Invalid LookAt Object." );
                    return;
                }

                // Apply Mode.
                object->setPathObjectOrientationMode( sceneObject, type, lookAtObject );

            } break;

        case VPathObject::k_OrientationToPoint:
            {
                // Fetch Point.
                Point3F lookAtPoint( 0.f, 0.f, 0.f );
                dSscanf( argv[4], "%g %g %g", &lookAtPoint.x, &lookAtPoint.y, &lookAtPoint.z );

                // Apply Mode.
                object->setPathObjectOrientationMode( sceneObject, type, lookAtPoint );

            } break;

        default :
            {
                AssertFatal( false, "VPath::setPathObjectOrientationMode() - Invalid Orientation Mode Specified." );

            } break;
    }
}

void VPath::setPathObjectOrientationMode( SceneObject *pObject, const VPathObject::eOrientationType &pType )
{
    VPathObject *pathObject = getPathObject( pObject );
    if ( !pathObject )
    {
        Con::warnf( "VPath::setPathObjectOrientationMode() - Object (%d) Not Attached to Path.", pObject->getId() );
        return;
    }

    // Apply.
    pathObject->setOrientationMode( pType );

    // Network Flags.
    setMaskBits( ObjectUpdateMask );
}

void VPath::setPathObjectOrientationMode( SceneObject *pObject, const VPathObject::eOrientationType &pType, SceneObject *pLookAtObject )
{
    VPathObject *pathObject = getPathObject( pObject );
    if ( !pathObject )
    {
        Con::warnf( "VPath::setPathObjectOrientationMode() - Object (%d) Not Attached to Path.", pObject->getId() );
        return;
    }

    // Apply.
    pathObject->setOrientationMode( pType, pLookAtObject );

    // Network Flags.
    setMaskBits( ObjectUpdateMask );
}

void VPath::setPathObjectOrientationMode( SceneObject *pObject, const VPathObject::eOrientationType &pType, const Point3F pPoint )
{
    VPathObject *pathObject = getPathObject( pObject );
    if ( !pathObject )
    {
        Con::warnf( "VPath::setPathObjectOrientationMode() - Object (%d) Not Attached to Path.", pObject->getId() );
        return;
    }

    // Apply.
    pathObject->setOrientationMode( pType, pPoint );

    // Network Flags.
    setMaskBits( ObjectUpdateMask );
}

ConsoleMethod( VPath, isPathObjectForward, bool, 3, 3, "( SimObject pObject ) - Get if this object is traveling forwards along the path.\n"
                                                       "@param pObject The SimObjectID of the object being observed.\n"
                                                       "@return Returns true if the object is traveling forwards." )
{
    // Fetch Object.
    SceneObject *sceneObject;
    if ( !Sim::findObject( argv[2], sceneObject ) )
    {
        Con::errorf( "VPath::isPathObjectForward() - Invalid Target Object." );
        return false;
    }

    // Fetch Object
    VPathObject *pathObject = object->getPathObject( sceneObject );

    // Return.
    return pathObject->isForward();
}

ConsoleMethod( VPath, setPathObjectForward, void, 4, 4, "( SimObject pObject, bool pForward ) - Set the travel direction of the object.\n"
                                                        "@param pObject The SimObjectID of the object being altered.\n"
                                                        "@param pForward The direction of the object.\n"
                                                        "@return No return value." )
{
    // Fetch Object.
    SceneObject *sceneObject;
    if ( !Sim::findObject( argv[2], sceneObject ) )
    {
        Con::errorf( "VPath::setPathObjectForward() - Invalid Target Object." );
        return;
    }

    // Apply.
    object->setPathObjectForward( sceneObject, dAtob( argv[3] ) );
}

void VPath::setPathObjectForward( SceneObject *pObject, const bool &pForward )
{
    VPathObject *pathObject = getPathObject( pObject );
    if ( !pathObject )
    {
        Con::warnf( "VPath::setPathObjectForward() - Object (%d) Not Attached to Path.", pObject->getId() );
        return;
    }

    // Apply.
    pathObject->setForward( pForward );

    // Network Flags.
    setMaskBits( ObjectUpdateMask );
}

ConsoleMethod( VPath, getPathObjectNode, S32, 3, 3, "( SimObject pObject ) - Gets the last node of the object.\n"
                                                    "@param pObject The SimObjectID of the object being observed.\n"
                                                    "@return Returns the node index." )
{
    // Fetch Object.
    SceneObject *sceneObject;
    if ( !Sim::findObject( argv[2], sceneObject ) )
    {
        Con::errorf( "VPath::getPathObjectNode() - Invalid Target Object." );
        return false;
    }

    // Fetch Object
    VPathObject *pathObject = object->getPathObject( sceneObject );

    // Return.
    return pathObject->getSourceNode();
}

ConsoleMethod( VPath, setPathObjectNode, void, 4, 4, "( SimObject pObject, bool pNodeIndex ) - Move the object to the node's position. You may also want to observe the \"setPathObjectInterp\" method.\n"
                                                     "@param pObject The SimObjectID of the object being altered.\n"
                                                     "@param pNodeIndex The index of the node that the object will reposition to.\n"
                                                     "@return No return value." )
{
    // Fetch Object.
    SceneObject *sceneObject;
    if ( !Sim::findObject( argv[2], sceneObject ) )
    {
        Con::errorf( "VPath::setPathObjectNode() - Invalid Target Object." );
        return;
    }

    // Apply.
    object->setPathObjectNode( sceneObject, dAtoi( argv[3] ) );
}

void VPath::setPathObjectNode( SceneObject *pObject, const S32 &pNodeIndex )
{
    VPathObject *pathObject = getPathObject( pObject );
    if ( !pathObject )
    {
        Con::warnf( "VPath::setPathObjectNode() - Object (%d) Not Attached to Path.", pObject->getId() );
        return;
    }

    // Source & Destination Nodes.
    const S32 srcNode = pNodeIndex;
    const S32 dstNode = ( pathObject->isForward() ) ? ( pNodeIndex + 1 ) : ( pNodeIndex - 1 );

    // Set Current Node.
    pathObject->setNode( normalizeNodeIndex( srcNode ), normalizeNodeIndex( dstNode ) );

    // Reset Interp.
    pathObject->setTimeInterp( 0.f );
    pathObject->setPathInterp( 0.f );

    // Network Flags.
    setMaskBits( ObjectUpdateMask );
}

ConsoleMethod( VPath, getPathObjectEndNode, S32, 3, 3, "( SimObject pObject ) - Get the index of the node this object is meant to stop upon reaching.\n"
                                                       "@param pObject The SimObjectID of the object being observed.\n"
                                                       "@return Returns the node index." )
{
    // Fetch Object.
    SceneObject *sceneObject;
    if ( !Sim::findObject( argv[2], sceneObject ) )
    {
        Con::errorf( "VPath::getPathObjectEndNode() - Invalid Target Object." );
        return false;
    }

    // Fetch Object
    VPathObject *pathObject = object->getPathObject( sceneObject );

    // Return.
    return pathObject->getEndNode();
}

ConsoleMethod( VPath, setPathObjectEndNode, void, 4, 4, "( SimObject pObject, bool pNodeIndex ) - Set end node of the path object. If a value of \"-1\" is applied, the object will path indefinitely.\n"
                                                        "@param pObject The SimObjectID of the object being altered.\n"
                                                        "@param pNodeIndex The index of the node that the object will cease pathing upon reaching.\n"
                                                        "@return No return value." )
{
    // Fetch Object.
    SceneObject *sceneObject;
    if ( !Sim::findObject( argv[2], sceneObject ) )
    {
        Con::errorf( "VPath::setPathObjectEndNode() - Invalid Target Object." );
        return;
    }

    // Apply.
    object->setPathObjectEndNode( sceneObject, dAtoi( argv[3] ) );
}

void VPath::setPathObjectEndNode( SceneObject *pObject, const S32 &pNodeIndex )
{
    VPathObject *pathObject = getPathObject( pObject );
    if ( !pathObject )
    {
        Con::warnf( "VPath::setPathObjectEndNode() - Object (%d) Not Attached to Path.", pObject->getId() );
        return;
    }

    // Set index.
    S32 index = pNodeIndex;

    if ( index != -1 )
    {
        // Normalize index.
        normalizeNodeIndex( index );
    }

    // Apply.
    pathObject->setEndNode( index );

    // Network Flags.
    setMaskBits( ObjectUpdateMask );
}