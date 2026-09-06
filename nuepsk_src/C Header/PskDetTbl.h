/* Modified by Milt Cram, W8NUE--June 2006  */

/* //////////////////////////////////////////////////////////////////// */
/* //////////////////////////////////////////////////////////////////// */
/* / PSK31Core Library for transmission and reception of PSK31 signals */
/* /    using a PC soundcard  or .wav files.                           */
/* /	               Copyright 2000, Moe Wheatley, AE4JY             */
/* /                                                                    */
/* /This library is free software; you can redistribute it and/or       */
/* /modify it under the terms of the GNU Lesser General Public          */
/* /License as published by the Free Software Foundation; either        */
/* /version 2.1 of the License, or (at your option) any later version.  */
/* /                                                                    */
/* /This library is distributed in the hope that it will be useful,     */
/* /but WITHOUT ANY WARRANTY; without even the implied warranty of      */
/* /MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU   */
/* /Lesser General Public License for more details.                     */
/* /                                                                    */
/* /You should have received a copy of the GNU Lesser General Public    */
/* /License along with this library; if not, write to the Free Software */
/* /Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */
/* / http://www.gnu.org/copyleft/lesser.html                              */
/* ///////////////////////////////////////////////////////////////////// */
/* ///////////////////////////////////////////////////////////////////// */

#include "math.h"
/*                                                                      */
#define PI2 6.2831853071795864765		/*            2 Pi              */
#define KCONV 10430.37835              /* 		4096*16/PI2 			*/
/*                                                                      */

/* phase wraparound correction tables for viterbi decoder       */
const double ANGLE_TBL1[4] = { 3.0*PI2/4.0, 0.0, PI2/4.0, PI2/4.0};
const double ANGLE_TBL2[4] = { 3.0*PI2/4.0, PI2, PI2/4.0, PI2/4.0 };

/* bit sync half distance table                                  */
/* index any position 0 to 15 and it returns a position halfway from the index */
const int HALF_TBL[16] = {
    7,	/* 0           */
   8,	/* 1           */
   9,	/* 2           */
   10,	/* 3           */
   11,	/* 4           */
   12,	/* 5           */
   13,	/* 6           */
   14,	/* 7           */
    0,	/* 8           */
    1,	/* 9           */
    2,	/* 10          */
    3,	/* 11          */
    4,	/* 12          */
    5,	/* 13          */
    6,	/* 14          */
    7,	/* 15          */
};


