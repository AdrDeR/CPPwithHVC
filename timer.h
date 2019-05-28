///
///////////////////////////////////////////////////////////////////////////////////////////////
/// Copyright notice
/// © 2016 Micronas GmbH. All rights reserved
/// 
/// This software and related documentation (the "Software") are intellectual
/// property owned by Micronas and are copyright of Micronas, unless specifically
/// noted otherwise. Any use of the Software is permitted only pursuant to the
/// terms of the license agreement, if any, which accompanies, is included with
/// or applicable to the Software ("License Agreement") or upon express written
/// consent of Micronas. Any copying, reproduction or redistribution of the
/// Software in whole or in part by any means not in accordance with the License
/// Agreement or as agreed in writing by Micronas is expressly prohibited.
///
/// THE SOFTWARE IS WARRANTED, IF AT ALL, ONLY ACCORDING TO THE TERMS OF THE
/// LICENSE AGREEMENT. EXCEPT AS WARRANTED IN THE LICENSE AGREEMENT THE SOFTWARE
/// IS DELIVERED "AS IS" AND MICRONAS HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS
/// WITH REGARD TO THE SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES AND CONDITIONS OF
/// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIT ENJOYMENT, TITLE AND
/// NON-INFRINGEMENT OF ANY THIRD PARTY INTELLECTUAL PROPERTY OR OTHER RIGHTS WHICH
/// MAY RESULT FROM THE USE OR THE INABILITY TO USE THE SOFTWARE. IN NO EVENT SHALL
/// MICRONAS BE LIABLE FOR INDIRECT, INCIDENTAL, CONSEQUENTIAL, PUNITIVE, SPECIAL OR
/// OTHER DAMAGES WHATSOEVER INCLUDING WITHOUT LIMITATION, DAMAGES FOR LOSS OF
/// BUSINESS PROFITS, BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, AND THE
/// LIKE, ARISING OUT OF OR RELATING TO THE USE OF OR THE INABILITY TO USE THE
/// SOFTWARE, EVEN IF MICRONAS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES,
/// EXCEPT PERSONAL INJURY OR DEATH RESULTING FROM MICRONAS' NEGLIGENCE
///
///	-----------------------------------------------------------------------
///
/// \brief  C-header file for Timer module with class definitions
///
/// \note see \ref timer.cpp documentation for more details.
///
///
///
///	-----------------------------------------------------------------------
///	author:			Adriano De Rosa August 25rd, 2016
///
///	modified:		Adriano De Rosa August 25rd, 2016
///							First tests with object oriented use of C++ and HVCF timer
///
///
///  Version:		v1.0
///  
///	C++ test with HVCF, using classes/OOP
///
///////////////////////////////////////////////////////////////////////////


#ifndef __TIMER_H
#define __TIMER_H

// Class Definition TIMER
// Normalerweise in header file

enum TimerState {Idle, Active, Done};
enum TimerType  {OneShot, Periodic};


////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  class Timer
/// 
/// Defintion of the Timer  class
///
/// @date August 15th, 2016 - First Implementation
/// @date August 15th, 2016 - Update
/// 
/// @todo -/-
/// @bug  None
/// @version 1.0
////////////////////////////////////////////////////////////////////////////////////////////
class Timer 
{
	public:
	   Timer();
	   ~Timer();
	
	   int32_t start(uint32_t nMilliseconds, TimerType = OneShot);
	   int32_t waitfor();
	   void cancel();
	
	   TimerState state;
	   TimerType type;
	   uint32_t length;
	   uint32_t count;
	   Timer *	pNext;		   
	
	private:
		 void (*Timer0Interrupt)(void);   // Function pointer -> This pointer will be redirected to TIM0_IRQHandler() !
};

////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  class TimerList
/// 
/// Defintion of the TimerList  class
///
/// @date August 15th, 2016 - First Implementation
/// @date August 15th, 2016 - Update
/// 
/// @todo -/-
/// @bug  None
/// @version 1.0
////////////////////////////////////////////////////////////////////////////////////////////
class TimerList
{
    public:
        TimerList();
		    void     insert(Timer * pTimer);
        Timer *  remove(Timer * pTimer);
        void     tick(void);
 
    private:
        Timer *  pTop;
};

#endif		// __TIMER_H 
