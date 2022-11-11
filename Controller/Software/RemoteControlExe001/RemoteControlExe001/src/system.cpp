/*
 * system.cpp
 *
 * Created: 10/12/2022 2:53:02 PM
 *  Author: AJ992
 */ 

#include <avr32/io.h>
#include <stdint.h>
#include "system.h"
#include "io.h"
/*
Function:
Params:
Returns:
Description:
*/


/*
Function: systemOSC0init
Params: none
Returns: none
Description: enables OSC0 and sets GCLOC 2 to the CPU clock divided by 512.
*/
void systemOSC0init()
{
	SCIF.unlock = (SCIF_UNLOCK_KEY) | 0x24;
	SCIF.oscctrl[0] = SCIF_OSCCTRL_STARTUP_256_bm					//OSC0 256 startup cycles.
						| (AVR32_SCIF_OSCCTRL0_GAIN_G3 << 1)		//Gain G3.
						| AVR32_SCIF_OSCCTRL_OSCEN_MASK;			//Enable.
	while ((SCIF.pclksr & SCIF_PCLKSR_OSC0RDY_bm) == 0){}
	
	SCIF.gcctrl[2] = SCIF_GCCTRL_OSCSEL_CPU_bm						//CPU clock as source for generic clock.
					| SCIF_GCCTRL_DIVEN_bm							//Divider enable.
					| 255 << SCIF_GCCTRL_DIV_bp						//Divide by 2*(1+255).
					| SCIF_GCCTRL_CEN_bm;							//Enable generic clock.
	
	PORTA.pmr0c = CLK_OUT_PIN_bm;
	PORTA.pmr1s = CLK_OUT_PIN_bm;
	ioSetPinPeripheral(&PORTA, CLK_OUT_PIN_bm);					//Give peripheral control.
}


/*
Function: systemMainClockSelect
Params: none
Returns: true: if OSC0 is ready and selected as main clock.
		 false: if OSC0 is not ready and will not be selected as main clock.
Description: Enables a divider of 2 for CPU, PBA, and PBB clock. Sets OSC0 as main clock.
*/
bool systemMainClockSelect()
{
	/*
	Enable 1 wait cycle for reads from flash because we are 
	going to be running faster than 33MHz.
	*/
	AVR32_FLASHCDW.fcr = FLASHCDW_FCR_WAITSTATE;
	
	if ((SCIF.pclksr & SCIF_PCLKSR_OSC0RDY_bm) != 0)
	{
		/*Select OSC0 as source for main clock*/
		PM.unlock = (PM_UNLOCK_KEY) | 0x00;								//Unlock MCCTRL.
		PM.mcctrl = PM_MCCTRL_MCSEL_OSC0_bm;							//Select OSC0 as main clock.
		
		while ((PM.sr & PM_SR_CKRDY_bm) == 0){}
			
		return true;
	}
	else
	{
		return false;
	}
}