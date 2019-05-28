#include "main.h"

extern "C" void TIM0_IRQHandler();
	
//#define NULL ((Timer *)0)
#define NULL 0   // Allgemein gültige definition des 0 pointers in C++
#define CYCLES_PER_TICK (25000/4)
#define MS_PER_TICK 1

TimerList timerList;


////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Timer::Timer(void)
/// @param  void
/// @return None defined.
///
/// Create and initialize a timer
/// Note: This is the timer CONSTRUCTOR of the timer class
///
/// @date August 15th, 2016 - First Implementation
/// @date August 15th, 2016 - Update
/// 
/// @todo -/-
/// @bug  None
/// @version 1.0
////////////////////////////////////////////////////////////////////////////////////////////
Timer::Timer(void)
{
	static int32_t bInitialized = 0;  //Flag used to allow only one call of timer HW initialization!
		
	//
	// initialize the new software timer
	//		
	state 	= Idle;
	type 		= OneShot;
	length 	= 0;	
	count = 0;
	pNext = NULL;
	
	//
	// Initialize the TIMER0 hardware
	//
	
	if (!bInitialized)
	{	
		// Setup Interrupt Handler
		NVIC_SetPriority(TIM0_IRQn, 1); 	 // Set TIM0 IRQ Prio = 1	
	  NVIC_EnableIRQ(TIM0_IRQn);				 // TIM0 Interrupt enable
		
		// Funktionenzeiger umbiegen auf TIM0_IRQHandler
		Timer0Interrupt=&TIM0_IRQHandler;
		
		// Initialize the timer 0 hardware
		TIM0->CR_b.CM   = 0;
		TIM0->CR_b.MOD  = 0; // Timer mode
		TIM0->PS 			  = 0x00C7;					  // counter frequency 20MHz/(199+1)=100kHz
		TIM0->AR 			  = 100;						  // auto-reload value -> TIMER0 ISR every 10ms
		TIM0->IEN_b.UPD	= 1;						    // enable update interrupt
		TIM0->CR_b.EN   = 1;  // Enable Timer 0		
		
		// Mark the timer hardware as initialized
		bInitialized = 1;
	}
	
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Timer::~Timer(void)
/// @param  void
/// @return None defined
///
/// Destructor of the timer class
/// This routine ensures that the timer isn't left in the timer list
///
/// @date August 15th, 2016 - First Implementation
/// @date August 15th, 2016 - Update
/// 
/// @todo -/-
/// @bug  None
/// @version 1.0
////////////////////////////////////////////////////////////////////////////////////////////
Timer::~Timer(void)
{
    //
    // Cancel the timer.
    //
    this->cancel();

}   /* ~Timer() */

////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  int32_t Timer::start(uint32_t nMilliseconds, TimerType timerType)
/// @param  uint32_t nMilliseconds, TimerType timerType
/// @return int32_t (-1 if timer is idling, otherwise 0  )
///
/// Method to start the timer
/// 
///
/// @date August 15th, 2016 - First Implementation
/// @date August 15th, 2016 - Update
/// 
/// @todo -/-
/// @bug  None
/// @version 1.0
////////////////////////////////////////////////////////////////////////////////////////////
int32_t 
Timer::start(uint32_t nMilliseconds, TimerType timerType)
{
	if(state != Idle)	
	{
		return (-1);
	}		
	
	// init the software timer 
	state  = Active;
	type   = timerType;
	length = nMilliseconds  / MS_PER_TICK;
	
	timerList.insert(this);
	
	return (0);	
}


////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  TimerList::TimerList(void)
/// @param  void
/// @return None defined.
///
/// Create and initialize a linked list of timers.
/// Note: This is the timer list CONSTRUCTOR of the timer list class
///
/// @date August 15th, 2016 - First Implementation
/// @date August 15th, 2016 - Update
/// 
/// @todo -/-
/// @bug  None
/// @version 1.0
////////////////////////////////////////////////////////////////////////////////////////////
TimerList::TimerList(void)
{
    pTop = NULL;
}   // TimerList() 


////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  void TimerList::insert(Timer * pTimer)
/// @param  Timer * pTimer
/// @return void
///
/// Insert a timer into an ordered linked list.
/// Note: This routine disables interrupts.
///
/// @date August 15th, 2016 - First Implementation
/// @date August 15th, 2016 - Update
/// 
/// @todo -/-
/// @bug  None
/// @version 1.0
////////////////////////////////////////////////////////////////////////////////////////////
void
TimerList::insert(Timer * pTimer)
{
    Timer **  ppPrev = &this->pTop;


    	__disable_irq();		

    //
    // Initialize the new timer's tick count.
    //
    pTimer->count = pTimer->length;

    //
    // Walk down the timer list, subtracting ticks as we go.
    //
   
  	while (*ppPrev != NULL && pTimer->count >= (*ppPrev)->count)
    {
        pTimer->count -= (*ppPrev)->count;
        ppPrev = &(*ppPrev)->pNext;
    }
    
    // 
    // Insert the new timer at this point in the timer list.
    //
    pTimer->pNext = *ppPrev;
    *ppPrev = pTimer;
  
    //
    // Adjust the tick count of the next timer (if any).
    //
    if (pTimer->pNext != NULL)
    {
        pTimer->pNext->count -= pTimer->count;
    } 

    __enable_irq();		

}   // insert()


////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Timer * TimerList::remove(Timer * pTimer)
/// @param  Timer * pTimer
/// @return A pointer to the removed timer, NULL if it wasn't found in the timer list.
///
/// Remove a timer from the timer list.
/// Note: This routine disables interrupts.
///
/// @date August 15th, 2016 - First Implementation
/// @date August 15th, 2016 - Update
/// 
/// @todo -/-
/// @bug  None
/// @version 1.0
////////////////////////////////////////////////////////////////////////////////////////////
Timer *
TimerList::remove(Timer * pTimer)
{
    Timer **  ppPrev = &this->pTop;


    __disable_irq();		

    //
    // Walk down the linked list until the dead timer is found.
    //
    while (*ppPrev != NULL && *ppPrev != pTimer)
    {
        ppPrev = &(*ppPrev)->pNext;
    }

    //
    // Remove the dead timer from the linked list.
    //
    if (*ppPrev != NULL)
    {
        *ppPrev = pTimer->pNext;
        (*ppPrev)->count += pTimer->count;
    }

     __enable_irq();		

    return (*ppPrev);

}   // remove()


////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  void TimerList::tick()
/// @param  void
/// @return void
///
/// Update the linked list of timers for a clock tick.
/// The caller is responsible for disabling interrupts.
///
/// @date August 15th, 2016 - First Implementation
/// @date August 15th, 2016 - Update
/// 
/// @todo -/-
/// @bug  None
/// @version 1.0
////////////////////////////////////////////////////////////////////////////////////////////
void
TimerList::tick()
{
    Timer **  ppTop  = &this->pTop;


    if (*ppTop != NULL)
    {
        //
        // Decrement the tick count of the first timer in the list.
        //
        (*ppTop)->count--;

        //
        // Mark all of the expired timers done and remove them.
        //
        while (*ppTop != NULL && (*ppTop)->count == 0)
        {
            (*ppTop)->state = Done;
            *ppTop = (*ppTop)->pNext;
        }                                   
    }

}   //  tick() 


////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  int32_t Timer::waitfor()
/// @param  void
/// @return 0 on success, -1 if the timer is not running.
///
/// Wait for the software timer to finish.
///
/// @date August 15th, 2016 - First Implementation
/// @date August 15th, 2016 - Update
/// 
/// @todo -/-
/// @bug  None
/// @version 1.0
////////////////////////////////////////////////////////////////////////////////////////////
int32_t
Timer::waitfor()
{
    if (state != Active)
    {
        return (-1);
    }
		
    //
    // Wait for the timer to expire.
    //
    while (state != Done){};

    //
    // Restart or idle the timer, depending on its type.
    //
     if (type == Periodic)
     {
        state = Active;
        timerList.insert(this);
     }
      else
      {
        state = Idle;
      }

    return (0); 
		

}   // waitfor()


////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  void Timer::cancel(void)
/// @param  void
/// @return void
///
/// Cancel a timer instace / stop a running timer
///
/// @date August 15th, 2016 - First Implementation
/// @date August 15th, 2016 - Update
/// 
/// @todo -/-
/// @bug  None
/// @version 1.0
////////////////////////////////////////////////////////////////////////////////////////////
void
Timer::cancel(void)
{
    // 
    // Remove the timer from the timer list.
    //
    if (state == Active)
    {
        timerList.remove(this);
    }

    //
    // Reset the timer's state.
    //
    state = Idle; 

}   // cancel() 

void TIM0_IRQHandler()
{
	uint32_t tmp_pending_flags_u32;		
		
	tmp_pending_flags_u32 = (TIM0->EIPND & 0x000FFF);	 // Read and store temporary the enabled interrupts pending flags 
	TIM0->EIPND = tmp_pending_flags_u32;							 // Clear the enabled interrupts pending flags
	
	timerList.tick();
	
//	Timer::HandleTimer0Interrupt(&TIM0_IRQHandler);
}
