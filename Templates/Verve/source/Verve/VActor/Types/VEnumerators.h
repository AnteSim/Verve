//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_ENUMERATORS_H_
#define _VT_ENUMERATORS_H_

//-----------------------------------------------------------------------------

enum eMove
{
    k_NullMove     = 0,

    k_ForwardMove  = ( 1 << 0 ),
    k_BackwardMove = ( 1 << 1 ),
    k_LeftMove     = ( 1 << 2 ),
    k_RightMove    = ( 1 << 3 ),
    k_UpMove       = ( 1 << 4 ),
    k_DownMove     = ( 1 << 5 ),

    k_XMove        = ( k_LeftMove | k_RightMove ),
    k_YMove        = ( k_ForwardMove | k_BackwardMove ),
    k_ZMove        = ( k_UpMove | k_DownMove ),
};

enum eControlState
{
    k_NullControlState = 0,

    k_PathControlState,
    k_GoToControlState,
};

#endif // _VT_ENUMERATORS_H_