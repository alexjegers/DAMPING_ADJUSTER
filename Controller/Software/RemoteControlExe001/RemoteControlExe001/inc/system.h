/*
 * system.h
 *
 * Created: 10/16/2022 2:54:23 PM
 *  Author: AJ992
 */ 


#ifndef SYSTEM_H_
#define SYSTEM_H_

#define PM								(*(volatile avr32_pm_t*) 0xFFFF1400)
#define PM_UNLOCK_KEY					0xAA << 24
#define PM_CPUSEL_CPUDIV_bm				1 << 7
#define PM_CPUSEL_DIV_3					2
#define PM_MCCTRL_MCSEL_OSC0_bm			1
#define PM_SR_CKRDY_bm					1 << 5

#define SCIF							(*(volatile avr32_scif_t*) 0xFFFF5800)
#define SCIF_UNLOCK_KEY					0xAA << 24
#define SCIF_OSCCTRL_STARTUP_256_bm		12 << 8
#define SCIF_PCLKSR_OSC0RDY_bm			1
#define SCIF_GCCTRL_OSCSEL_OSC0_bm		2 << 8
#define SCIF_GCCTRL_OSCSEL_RCSYS_bm		0
#define SCIF_GCCTRL_OSCSEL_CPU_bm		5 << 8
#define SCIF_GCCTRL_DIV_bp				16
#define SCIF_GCCTRL_DIVEN_bm			1 << 1
#define SCIF_GCCTRL_CEN_bm				1

#define FLASHCDW_FCR_WAITSTATE			1 << 6

#define CPU_SR							(*(volatile avr32_sr_t*) 0x00)


void systemOSC0init();
bool systemMainClockSelect();



#endif /* SYSTEM_H_ */