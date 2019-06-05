//Configuration Settings



//PLL Prescaler Selection bits:
#pragma config PLLDIV = 5		//PLLDIV = 1 No prescale (4 MHz oscillator input drives PLL directly)
								//PLLDIV = 2 Divide by 2 (8 MHz oscillator input)
								//PLLDIV = 3 Divide by 3 (12 MHz oscillator input)
								//PLLDIV = 4 Divide by 4 (16 MHz oscillator input)
								//PLLDIV = 5 Divide by 5 (20 MHz oscillator input)
								//PLLDIV = 6 Divide by 6 (24 MHz oscillator input)
								//PLLDIV = 10 Divide by 10 (40 MHz oscillator input)
								//PLLDIV = 12 Divide by 12 (48 MHz oscillator input)*/
    
//CPU System Clock Postscaler:
#pragma config CPUDIV = OSC1_PLL2		//CPUDIV = OSC1_PLL2 [OSC1/OSC2 Src: /1][96 MHz PLL Src: /2]
										//CPUDIV = OSC2_PLL3 [OSC1/OSC2 Src: /2][96 MHz PLL Src: /3]
										//CPUDIV = OSC3_PLL4 [OSC1/OSC2 Src: /3][96 MHz PLL Src: /4]
										//CPUDIV = OSC4_PLL6 [OSC1/OSC2 Src: /4][96 MHz PLL Src: /6]

//USB Clock Selection bit (used in Full Speed USB mode only;UCFG:FSEN = 1):
#pragma config USBDIV=2			//USBDIV = 1 USB clock source comes directly from the primary oscillator block with no postscale
								//USBDIV = 2 USB clock source comes from the 96 MHz PLL divided by 2

//Oscillator Selection bits:
#pragma config FOSC=HS	        //FOSC = XT_XT XT oscillator, XT used by USB
								//FOSC = XTPLL_XT XT oscillator, PLL enabled, XT used by USB
								//FOSC = ECIO_EC External clock, port function on RA6, EC used by USB
								//FOSC = EC_EC External clock, CLKOUT on RA6, EC used by USB
								//FOSC = ECPLLIO_EC External clock, PLL enabled, port function on RA6, EC used by USB
								//FOSC = ECPLL_EC External clock, PLL enabled, CLKOUT on RA6, EC used by USB
								//FOSC = INTOSCIO_EC Internal oscillator, port function on RA6, EC used by USB
								//FOSC = INTOSC_EC Internal oscillator, CLKOUT on RA6, EC used by USB
								//FOSC = INTOSC_XT Internal oscillator, XT used by USB
								//FOSC = INTOSC_HS Internal oscillator, HS used by USB
								//FOSC = HS HS oscillator, HS used by USB
								//FOSC = HSPLL_HS HS oscillator, PLL enabled, HS used by USB

//Fail-Safe Clock Monitor Enable bit:
#pragma config FCMEN = ON 		//FCMEM = OFF Fail-Safe Clock Monitor disabled
								//FCMEM = ON Fail-Safe Clock Monitor enabled

//Internal/External Oscillator Switchover bit:
#pragma config IESO=OFF			//IESO = OFF Oscillator Switchover mode disabled
								//IESO = ON Oscillator Switchover mode enabled

//Power-up Timer Enable bit:
#pragma config PWRT=ON			//PWRT = ON PWRT enabled
								//PWRT = OFF PWRT disabled

//Brown-out Reset Enable bits:
#pragma config BOR=ON			//BOR = OFF Brown-out Reset disabled in hardware and software
								//BOR = SOFT Brown-out Reset enabled and controlled by software (SBOREN is enabled)
								//BOR = ON_ACTIVE Brown-out Reset enabled in hardware only and disabled in Sleep mode (SBOREN is disabled)
								//BOR = ON Brown-out Reset enabled in hardware only (SBOREN is disabled)

//Brown-out Voltage bits:
#pragma config BORV=3			//BORV = 0 Maximum setting
								//BORV = 1
								//BORV = 2
								//BORV = 3 Minimum setting

//USB Voltage Regulator Enable bit:
#pragma config VREGEN=OFF		//VREGEN = OFF USB voltage regulator disabled
								//VREGEN = ON USB voltage regulator enabled

//Watchdog Timer Enable bit:
#pragma config WDT=ON			//WDT = OFF HW Disabled - SW Controlled
								//WDT = ON HW Enabled - SW Disabled

//Watchdog Timer Postscale Select bits:
#pragma config WDTPS=32768		//WDTPS = 1 1:1
								//WDTPS = 2 1:2
								//WDTPS = 4 1:4
								//WDTPS = 8 1:8
								//WDTPS = 16 1:16
								//WDTPS = 32 1:32
								//WDTPS = 64 1:64
								//WDTPS = 128 1:128
								//WDTPS = 256 1:256
								//WDTPS = 512 1:512
								//WDTPS = 1024 1:1024
								//WDTPS = 2048 1:2048
								//WDTPS = 4096 1:4096
								//WDTPS = 8192 1:8192
								//WDTPS = 16384 1:16384
								//WDTPS = 32768 1:32768

//MCLR Pin Enable bit:
#pragma config MCLRE=ON		    //MCLRE = OFF RE3 input pin enabled; MCLR disabled
								//MCLRE = ON MCLR pin enabled; RE3 input pin disabled

//Low-Power Timer 1 Oscillator Enable bit:
#pragma config LPT1OSC=OFF		//LPT1OSC = OFF Timer1 configured for higher power operation
								//LPT1OSC = ON Timer1 configured for low-power operation
//PORTB A/D Enable bit:
#pragma config PBADEN=ON		//PBADEN = OFF PORTB<4:0> pins are configured as digital I/O on Reset
								//PBADEN = ON PORTB<4:0> pins are configured as analog input channels on Reset

//CCP2 MUX bit:
#pragma config CCP2MX=OFF		//CCP2MX = OFF CCP2 input/output is multiplexed with RB3
								//CCP2MX = ON CCP2 input/output is multiplexed with RC1

//Stack Full/Underflow Reset Enable bit:
#pragma config STVREN=ON		//STVREN = OFF Stack full/underflow will not cause Reset
								//STVREN = ON Stack full/underflow will cause Reset

//Single-Supply ICSP Enable bit:
#pragma config LVP=OFF			//LVP = OFF Single-Supply ICSP disabled
								//LVP = ON Single-Supply ICSP enabled

//Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit:
#pragma config ICPRT=OFF 		//ICPRT = OFF ICPORT disabled
								//ICPRT = ON ICPORT enabled

//Extended Instruction Set Enable bit:
#pragma config XINST=OFF		//XINST = OFF Instruction set extension and Indexed Addressing mode disabled (Legacy mode)
								//XINST = ON Instruction set extension and Indexed Addressing mode enabled

//Background Debugger Enable bit:
#pragma config DEBUG=ON			//DEBUG = ON Background debugger enabled, RB6 and RB7 are dedicated to In-Circuit Debug
								//DEBUG = OFF Background debugger disabled, RB6 and RB7 configured as general purpose I/O pins

//Code Protection bit Block 0:
#pragma config CP0=ON			//CP0 = ON Block 0 (000800-001FFFh) code-protected
								//CP0 = OFF Block 0 (000800-001FFFh) not code-protected

//Code Protection bit Block 1:
#pragma config CP1=ON			//CP1 = ON Block 1 (002000-003FFFh) code-protected
								//CP1 = OFF Block 1 (002000-003FFFh) not code-protected

//Code Protection bit Block 2:
#pragma config CP2=ON			//CP2 = ON Block 2 (004000-005FFFh) code-protected
								//CP2 = OFF Block 2 (004000-005FFFh) not code-protected

//Code Protection bit Block 3:
#pragma config CP3=OFF			//CP3 = ON Block 3 (006000-007FFFh) code-protected
								//CP3 = OFF Block 3 (006000-007FFFh) not code-protected

//Boot Block Code Protection bit:
#pragma config CPB=OFF			//CPB = ON Boot block (000000-0007FFh) code-protected
								//CPB = OFF Boot block (000000-0007FFh) not code-protected

//Data EEPROM Code Protection bit:
#pragma config CPD=OFF			//CPD = ON Data EEPROM code-protected
								//CPD = OFF Data EEPROM not code-protected

//Write Protection bit Block 0:
#pragma config WRT0=OFF			//WRT0 = ON Block 0 (000800-001FFFh) write-protected
								//WRT0 = OFF Block 0 (000800-001FFFh) not write-protected

//Write Protection bit Block 1:
#pragma config WRT1=OFF			//WRT1 = ON Block 1 (002000-003FFFh) write-protected
								//WRT1 = OFF Block 1 (002000-003FFFh) not write-protected

//Write Protection bit Block 2:
#pragma config WRT2=OFF			//WRT2 = ON Block 2 (004000-005FFFh) write-protected
								//WRT2 = OFF Block 2 (004000-005FFFh) not write-protected

//Write Protection bit Block 3:
#pragma config WRT3=OFF			//WRT3 = ON Block 3 (006000-007FFFh) write-protected
								//WRT3 = OFF Block 3 (006000-007FFFh) not write-protected

//Boot Block Write Protection bit:
#pragma config WRTB=OFF			//WRTB = ON Configuration registers (300000-3000FFh) write-protected
								//WRTB = OFF Configuration registers (300000-3000FFh) not write-protected

//Configuration Register Write Protection bit:
#pragma config WRTC=OFF			//WRTC = ON Boot block (000000-0007FFh) write-protected
								//WRTC = OFF Boot block (000000-0007FFh) not write-protected

//Data EEPROM Write Protection bit:
#pragma config WRTD=OFF			//WRTD = ON Data EEPROM write-protected
								//WRTD = OFF Data EEPROM not write-protected

//Table Read Protection bit Block 0:
#pragma config EBTR0=OFF		//EBTR0 = ON Block 0 (000800-001FFFh) protected from table reads executed in other blocks
								//EBTR0 = OFF Block 0 (000800-001FFFh) not protected from table reads executed in other blocks

//Table Read Protection bit Block 1:
#pragma config EBTR1=OFF		//EBTR1 = ON Block 1 (002000-003FFFh) protected from table reads executed in other blocks
								//EBTR1 = OFF Block 1 (002000-003FFFh) not protected from table reads executed in other blocks

//Table Read Protection bit Block 2:
#pragma config EBTR2=OFF		//EBTR2 = ON Block 2 (004000-005FFFh) protected from table reads executed in other blocks
								//EBTR2 = OFF Block 2 (004000-005FFFh) not protected from table reads executed in other blocks

//Table Read Protection bit Block 3:
#pragma config EBTR3=OFF		//EBTR3 = ON Block 3 (006000-007FFFh) protected from table reads executed in other blocks
								//EBTR3 = OFF Block 3 (006000-007FFFh) not protected from table reads executed in other blocks

//Boot Block Table Read Protection:
								//EBTRB = OFF Boot block (000000-0007FFh) not protected from table reads executed in other blocks
#pragma config EBTRB=OFF		//EBTRB = ON Boot block (000000-0007FFh) protected from table reads executed in other blocks
