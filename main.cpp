/// @cond
/////////////////////////////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////////////////////////
/// @endcond
///
/// @mainpage Index Page for HVCF0401 CPP test
///
/// @section intro_sec (1) Introduction
///
/// This software is used to check the usage of classes with C++   \n
/// Targeted MCU: HVCF0401 (HVC 4223F-C1)
///
/// The project can be compiled with KEIL MDK 5.14.0.0 and higher
/// Compiler Option:
/// --cpp11 must be set under misc controls in c/c++ 
///
/// @section hw1 (2) Application Board Hardware: which HW is needed
/// Boards which can be used with this software
///
/// - All
/// - LED 0 at LGPIO 0 is blinking with 2 Hz
///
/// @section hw2 (3) MCU pins to connect 
///
/// Default
///
///  
/// @section install_sec (4) Tool Installation
/// @subsection ide IDE: How to install
/// MDK can be installed according to information on the KEIL hompage  http://www.keil.com/
///
//////////////////////////////////////////////////////////////////////////////////////  
/// @file	main.c
/// @brief  Main file including function use the CPP tests
/// @author Adriano De Rosa adriano.derosa@micronas.com
///
/// Refer to source code documentation for more details \n
///  
/// Chip Project  HVCF-4-x \n
/// 
/// @date August 23rd, 2016 - Created / Adriano De Rosa
/// @date August 23rd, 2016 - Update  / Adriano De Rosa
///
/// @version		1.0
///
///	@[Micronas]: http://www.micronas.com/        "Micronas"
///
///////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////  Includes    //////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
#include "main.h"

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////  Definitions //////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

// Some defines for debug purposes only. Refer to main.h for functional configuration 

//#define LGPIO_TRACE_SWO //  use trace SWO 
#define LGPIO_DEBUG_TIMER


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////// Static Functions	Declaration		/////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
static void MAIN_InitSystem ( void );
static int32_t MAIN_Read_ADC_TJ ( void );
 
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// Globals		///////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
uint32_t MAIN_Command_Mode_u32=0;		// set mode out of the debugger 
int32_t MAIN_TjTemperature=0;


	
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// Types declarations     //////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////


	
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// Globals		///////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

														
////////////////////////////////////////////////////////////////////////////////////////////
/// @brief void MAIN_InitSystem ( void )
/// @param void
/// @return void
///
/// Initialize the system
///
/// @date August 15th, 2016 - First Implementation
/// @date August 15th, 2016 - Update
/// 
/// @todo -/-
/// @bug  None
/// @version 1.0
////////////////////////////////////////////////////////////////////////////////////////////
void MAIN_InitSystem ( void )
{
	//////////////////////// LGPIO ///////////////////////////////////////////////////////
	LGPIO->DD = BIT_0|BIT_1; 			// LGPIO0 and 1 as debug output (push-pull) for flashing a LED
	
#ifdef LGPIO_TRACE_SWO
	 //LGPIO->AOS_b.AOS8=1; //LGPIO8
	 //LGPIO->DD_b.DD8=1;   //LGPIO8
	 //LGPIO->C0_b.C08=1;   //LGPIO8
	 //LGPIO->C1_b.C18=1;   //LGPIO8
	 LGPIO->AOS_b.AOS5=0; //LGPIO5
	 LGPIO->DD_b.DD5=1;   //LGPIO5
	 LGPIO->C0_b.C05=1;   //LGPIO5
	 LGPIO->C1_b.C15=1;   //LGPIO5
#endif		
	 
 //////////////////////// TIMER 0 ////////////////////////////////////////////////////////
 // is initlaized in timer constructor	
	
	//////////////////////// NVIC ////////////////////////////////////////////////////////
	SCB->VTOR			= 0x00200000;				// NVIC Vector Table Offset	points to start of Flash memory 
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @brief void MAIN_Nvram_Setup ( void )
/// @param void
/// @return void
///
/// NVRAM programming
///
/// @date August 15th, 2016 - First Implementation
/// @date August 15th, 2016 - Update
/// 
/// @todo -/-
/// @bug  None
/// @version 1.0
////////////////////////////////////////////////////////////////////////////////////////////
void MAIN_Nvram_Setup ( void )
{
	
	/* unlock write for page 10 */
	NVRAM->ULWA10 = 0xA02E3527;
	NVRAM->ULWB10 = ~0xA02E3527;
	/* unlock store for page 10 */
	NVRAM->ULSA10 = 0x6A432567;
	NVRAM->ULSB10 = ~0x6A432567;
	
	while(!NVRAM->CR_b.RDY)
	{
	};	/* wait for NVRAM to be ready for next write/store */

	/* switch on LDO mode */
	NVRAM->CONFIG_b.SMPS_MODE = 0;			/* select SMPS mode, 1=Buck, 0=LDO */
		
	NVRAM->CR_b.MEMSEL = NVRAM_PAGE_10; /* select page 10 for store operation */
	/* start store sequence */
	NVRAM->CR_b.STO = 1;
	while(NVRAM->CR_b.STO);	/* wait for store cycle to be finished */

}

																
////////////////////////////////////////////////////////////////////////////////////////////
/// @brief int32_t MAIN_Read_ADC_TJ ( void )
/// @param void
/// @return int32_t
///
/// Trigger a conversion to get the TJ temperature
///
/// @date August 15th, 2016 - First Implementation
/// @date August 15th, 2016 - Update
/// 
/// @todo -/-
/// @bug  None
/// @version 1.0
////////////////////////////////////////////////////////////////////////////////////////////
int32_t MAIN_Read_ADC_TJ ( void )
{
	#define ARRAY_SIZE 8	
	volatile int32_t  tj_average_over_all_queues=0;
	volatile int16_t  adc_q5,adc_q6,adc_q7,adc_q8; 
	static   int32_t  tj_array_moving_average_i32[ARRAY_SIZE]; 
	static   uint32_t array_counter_u32;	
	int32_t  tj_moving_average_i32, i;	
		
	ADC -> QCR2 = 0xa6a6a6a6; // ONLY TJ MEASUREMENT for all queues!
	ADC->CR_b.EN = 1;					// Enable the ADC 	
	ADC->CR_b.SWT = 1;				// start conversion ....
	
	while (ADC->CR_b.BUSY)
		;								// ... wait until ready 
	
	// Get queue values q5 to q8 ... 
	adc_q5 = ((short)(ADC->DR5 << 4)) / 16;
	adc_q6 = ((short)(ADC->DR6 << 4)) / 16;
	adc_q7 = ((short)(ADC->DR7 << 4)) / 16;
	adc_q8 = ((short)(ADC->DR8 << 4)) / 16;
		
	tj_average_over_all_queues = (adc_q5+adc_q6+adc_q7+adc_q8)>>2; // Divide by 4
	//tj_average_over_all_queues = (((tj_average_over_all_queues)*2717)-2603300)/10000; /* preliminary temperatue formula for HVCF-3-1 */
	//tj_average_over_all_queues = (((tj_average_over_all_queues)*26455)-25727513)/100000; // temperatue formula for HVCF-4-1 
	tj_average_over_all_queues = (((tj_average_over_all_queues)*17338)-16860783)>>16; // temperatue formula for HVCF-4-1 
	tj_array_moving_average_i32[array_counter_u32] = tj_average_over_all_queues;
	
	array_counter_u32++;
	
	// reset array counter if array filled ... 
	if(array_counter_u32 == ARRAY_SIZE) 
	{
		array_counter_u32=0;
	}
	
	// calculate moving average ...
	for(i=0;i<ARRAY_SIZE;i++)
	{
		tj_moving_average_i32 = tj_moving_average_i32 + tj_array_moving_average_i32[i];		
	}
	tj_moving_average_i32 = tj_moving_average_i32 / ARRAY_SIZE;
	
	return ( tj_moving_average_i32 );	
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @brief i32 main ( void )
/// @param void
/// @return i32
///
/// Main function
///
/// @date August 15th, 2016 - First Implementation
/// @date August 15th, 2016 - Update
/// 
/// @todo -/-
/// @bug  None
/// @version 1.0
////////////////////////////////////////////////////////////////////////////////////////////
int32_t main(void) 
{
	Timer  timer;
	Timer  timer2;
	
	// System Initialization 
	MAIN_InitSystem();
				
	//timer.start(100, Periodic);      // Start a periodic 100-ms timer.
	
#ifdef 	LGPIO_DEBUG_RETENTION
	if(((SYSCTRL->RWSR) & 0x82) == 0x82 )
		{			
			LGPIO->DO = (BIT_0|BIT_1);			// LGPIO0 and 1 = on: indicated POR + RET ->coming from RETENTION 
		}
		else
		{
			LGPIO->DO = BIT_0;			// LGPIO0 = on: indicates only POR 
		}
#endif
		
				
	// Main idle loop 
	while (1)
	{
		while (1)
    {
        MAIN_TjTemperature = MAIN_Read_ADC_TJ();
        
			  timer.start(100, Periodic);          // Start a periodic 100-ms timer.
			
			  while(timer.waitfor()!=0)            // Wait for the timer to expire.
				{													
				}			
			  LGPIO->DO_b.DO0^=1;     // Toggle LED @ LGPIO0																				
			  
				timer.cancel();
				
				timer2.start(400, Periodic);      // Start a periodic 100-ms timer.
				while(timer2.waitfor()!=0)            // Wait for the timer to expire.
				{													
				}			
			  LGPIO->DO_b.DO1^=1;     // Toggle LED @ LGPIO1
				
				timer2.cancel();
    }
	   
	}
}



