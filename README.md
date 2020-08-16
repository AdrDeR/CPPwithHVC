# CPPwithHVC
How to use C++ code with Embedded HVC4223F HV Microcontroller
This is just the first test how to work with github and to create branches.
The CPP project for HVC 4223F using the KEIL MDK IDE is useful to understand how to use C++ with this kind of embedded controller.

Using C++ with classes and KEIL MDK:
=====================================
Add compiler option --cpp11 
Where? Into  under project options "c/c++" in field  misc controls (=compiler option)

NVIC and Classes. What must be considered?
==========================================

IMPORTANT Note: By activating the C++ Compiler Option --cpp11 t IRQ Handler was not linkes anymore. After usoing the extern "C" statemments the IRQHandler Adresse is linked again.  

extern "C" void TIM0_IRQHandler() {
    	uint32_t tmp_pending_flags_u32;		
		
	tmp_pending_flags_u32 = (TIM0->EIPND & 0x000FFF);	 // Read and store temporary the enabled interrupts pending flags 
	TIM0->EIPND = tmp_pending_flags_u32;							 // Clear the enabled interrupts pending flag
}

Die Funktion TIM0_IRQHandler() wird aufgerufen über einen Funktionspointer der in der Klasse Timer deklariert wird!
 
#  void (*Timer0Interrupt)(void);   // Function pointer -> This pointer will be redirected to TIM0_IRQHandler() !
 
 
 
 
Timer0Interrupt wird dann "umgebogen" auf TIM0_IRQHandler()!

See also under this reference: 
--> https://developer.mbed.org/questions/69315/NVIC-Set-Vector-in-class/

The basic idea: 
Make a static member function for each of the IRQs you want to use (So if only this timer, one is sufficient). Also make a static pointer to an object of your class for each of those static irq functions. Use SetVector with your static function, and in your non-static code when the interrupt is set, make the static pointer point to your current class object (eg, the 'this' pointer).
Then your interrupt code can use this pointer to call the correct function. In semi-pseudo code it looks like:

ClassA *myIRQ_objectpointer;
 
 ClassA::ClassA() {
  myIRQ_objectpointer = this;
    NVIC_SetVector(TIMER2_IRQn, (uint32_t) &ClassA::myIRQ);
    NVIC_EnableIRQ(TIMER2_IRQn);
}
 
void ClassA::irq_function() {
  //Your code here
}
 
static void ClassA::myIRQ() {
  myIRQ_objectpointer->irq_function()
}


Followed this nice tutorial on github https://guides.github.com/activities/hello-world/
