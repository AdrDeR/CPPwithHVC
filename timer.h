//
///////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// 2019 Adriano De Rosa
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
