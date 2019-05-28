//
///////////////////////////////////////////////////////////////////////////////////////////////
// Copyright notice
// © 2016 Micronas GmbH. All rights reserved
// 
// This software and related documentation (the "Software") are intellectual
// property owned by Micronas and are copyright of Micronas, unless specifically
// noted otherwise. Any use of the Software is permitted only pursuant to the
// terms of the license agreement, if any, which accompanies, is included with
// or applicable to the Software ("License Agreement") or upon express written
// consent of Micronas. Any copying, reproduction or redistribution of the
// Software in whole or in part by any means not in accordance with the License
// Agreement or as agreed in writing by Micronas is expressly prohibited.
//
// THE SOFTWARE IS WARRANTED, IF AT ALL, ONLY ACCORDING TO THE TERMS OF THE
// LICENSE AGREEMENT. EXCEPT AS WARRANTED IN THE LICENSE AGREEMENT THE SOFTWARE
// IS DELIVERED "AS IS" AND MICRONAS HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS
// WITH REGARD TO THE SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES AND CONDITIONS OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIT ENJOYMENT, TITLE AND
// NON-INFRINGEMENT OF ANY THIRD PARTY INTELLECTUAL PROPERTY OR OTHER RIGHTS WHICH
// MAY RESULT FROM THE USE OR THE INABILITY TO USE THE SOFTWARE. IN NO EVENT SHALL
// MICRONAS BE LIABLE FOR INDIRECT, INCIDENTAL, CONSEQUENTIAL, PUNITIVE, SPECIAL OR
// OTHER DAMAGES WHATSOEVER INCLUDING WITHOUT LIMITATION, DAMAGES FOR LOSS OF
// BUSINESS PROFITS, BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, AND THE
// LIKE, ARISING OUT OF OR RELATING TO THE USE OF OR THE INABILITY TO USE THE
// SOFTWARE, EVEN IF MICRONAS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES,
// EXCEPT PERSONAL INJURY OR DEATH RESULTING FROM MICRONAS' NEGLIGENCE
//
//	-----------------------------------------------------------------------
//
// \brief  C-header file for application main module.
//
// \note see \ref main.c documentation for more details.
//
//
//

//	-----------------------------------------------------------------------
//	author:			Adriano De Rosa August 23rd, 2016
//
//	modified:		Adriano De Rosa August 23rd, 2016
//							First tests 
//
//
//  Version:		v1.0
//  
//	C++ test / Nutzung von Klassen
//
//////////////////////////////////////////////////////////////////////////


#ifndef __MAIN_H
#define __MAIN_H

#include "HVC4223F_C1.h"
#include <core_cm3.h>        				/*!< Cortex-M3 processor and core peripherals */
#include "HVC4223F_C1_API.h"
#include "timer.h"

// Globals 
#define MAIN_FSYS_HZ		20000000	/* system clock in Hz => 20 MHz with HVCF */

#define NVRAM_PAGE_10		(8U)


// DEFINITIONS TO BE ADAPTED BY THE USER 


#endif		/* __MAIN_H */
