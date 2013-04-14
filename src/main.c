/**
  **********************************************************
  * @file    Project/main.c
  * @author  Holts
  * @version V1.0.0
  * @date    2012-12-9
  * @brief   Main program body
  **********************************************************
	*/
/* Includes ----------------------------------------------*/
#include "stm8s.h"              
#include "parameter.h"
#include "ptask.h"
#include "main.h"
#include "HD44780.h"
#include "delay.h"


/* Private typedef ---------------------------------------*/

/* Private macro -----------------------------------------*/

/* Private variables -------------------------------------*/
u8  temp_AD_H;					   // temporary registers for reading ADC result
u8  temp_AD_L;             // temporary registers for reading ADC result
u8	ADInit;						     // flag for ADC initialized
u8	ADSampRdy;				     // flag for filed of samples ready
u8  AD_samp;						   // counter of stored samples
u16 AD_sample[NUMB_SAMP];	 // store samples field 
u16 AD_avg_value;				   // average of ADC result
	
volatile unsigned char timers[MAXTASKS];
 
u8  Trg;
u8  Cont;

bool Auto = FALSE;
u8  Mode = MODE_R;
u8  Frq = SEL_1K;
short int  Range = RNG_40;
u8  Gain;

volatile bool event = TRUE;
volatile u16 ten_ms = 0;
volatile u16 sec = 0;
volatile u16 min = 0;

/* Define 6 custom characters to display bar graph*/
char STCustom[48] =
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Blank
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, // 1column  |
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, // 2columns ||
    0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, // 3columns |||
    0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, // 4columns ||||
    0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, // 5columns |||||
  };

/* Stores the sin structure in ram */
Sin_Typedef sin;
/* The counter variable for counting the Update cycles in TIM1 */
u8 counter = 0;   
/* This shows the value of the samples for the different no of samples in half
sine wave. The last value corresponds to the amplitude of the sine wave.*/
CONST u8 sample[16]={0,10,21,31,41,50,59,67,74,81,87,91,95,98,99,100};



 

  u16 i, count;
  u8 char_pos, roll;
  /* Custom characters can be 8 max. user defined characters from 0 to 7*/
  /* disp[] string is here to generate the display of custom characters on LCD*/
  /* of the bar graph using 8 characters pattern*/
  /* Example: printing "\0\0\0\0\0\0\0\0\n" will display 8 times character 0*/
  /* which is the blank character*/
  /* Example: printing "\5\5\5\5\5\5\5\5\n" will display 8 times character 5*/
  /* which is the 5 columns character*/
  char disp[] = "\0\0\0\0\0\0\0\0\n";
  char message[5][7] = {"STM8S", "Value", "Line", "8-Bit", "Micro"};



/* Private function prototypes ---------------------------*/
/**
  * @brief Count average of samples stored in the u16 field
  * @par Parameters:
  *  *p: pointer to the begin of the field
  * smp: number of samples in the field
  * @retval Average u16 value
  */
u16 u16_average(u16 *p, u8 smp) 
{ 
	u8 i;
	u16 sum;
	
	for(i=0, sum= 0; i < smp; ++i)
		sum+= *p++;		
	return sum / smp;
}

/**
  **********************************************************
  * @brief Configures clocks
  * @par Parameters: None
  * @retval void None
  * @par Required preconditions: None
  **********************************************************
  */
void CLK_Configuration(void)
{ /* Fmaster = 8MHz */
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV2);
}

/***********************************************************
  * @brief Configures GPIOs
  * @par Parameters: None
  * @retval void None
  * @par Required preconditions: None
  **********************************************************
  */
void GPIO_Configuration(void)
{
  /* GPIO reset */
  GPIO_DeInit(LCDPort);
  /* Configure LCDPort D0 output push-pull low LCD Bus*/
  GPIO_Init(LCDPort, GPIO_PIN_0, GPIO_MODE_OUT_PP_LOW_FAST);
  /* Configure LCDPort D1  output push-pull low LCD Bus*/
  GPIO_Init(LCDPort, GPIO_PIN_1, GPIO_MODE_OUT_PP_LOW_FAST);
  /* Configure LCDPort D2  output push-pull low LCD Bus*/
  GPIO_Init(LCDPort, GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_FAST);
  /* Configure LCDPort D3  output push-pull low LCD Bus*/
  GPIO_Init(LCDPort, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);
	
  /* GPIO reset */
  GPIO_DeInit(LCDControlPort);
  /* Configure LCDPort E output push-pull low LCD Enable Pin*/
  GPIO_Init(LCDControlPort, LCD_Enable, GPIO_MODE_OUT_PP_LOW_FAST);
  /* Configure LCDPort RS output push-pull low LCD RS Pin*/
  GPIO_Init(LCDControlPort, LCD_RS, GPIO_MODE_OUT_PP_LOW_FAST);
	
	/* GPIO reset */
  GPIO_DeInit(LCDPwrPort);
  /* Configure LCDPwrOnPort VDD output push-pull low LCD Power Supply*/
  GPIO_Init(LCDPwrPort, LCDPwrPin, GPIO_MODE_OUT_PP_LOW_FAST);

	/* GPIOD reset */
  //GPIO_DeInit(BUTTON_PORT);
	/* Configure PC4 (push button) as input floating */
  GPIO_Init(BUTTON_PORT, BUTTON_PIN, GPIO_MODE_IN_PU_NO_IT);
	/* Configure PD0 (LED1) as output push-pull low (led switched on) */
  //GPIO_Init(GPIOD, GPIO_PIN_0, GPIO_MODE_OUT_PP_LOW_FAST);
	
	/* ADC reset */
	GPIO_DeInit(ADC_GPIO_PORT);
	GPIO_Init(ADC_GPIO_PORT, ADC_INPUT, GPIO_MODE_IN_FL_NO_IT);
	
	/* FRQ reset */
	GPIO_DeInit(FRQ_GPIO_PORT);
	GPIO_Init(FRQ_GPIO_PORT, FRQ_GPIO_PINS, GPIO_MODE_OUT_PP_LOW_FAST);
	
	/* RANGE reset */
  GPIO_DeInit(RANGE_GPIO_PORT);   
  GPIO_Init(RANGE_GPIO_PORT, RANGE_GPIO_PINS, GPIO_MODE_OUT_PP_LOW_FAST);
}

/***********************************************************
  * @brief TIM1 init
  * @par Parameters: None
  * @retval void None
  * @par Required preconditions: None
  **********************************************************
  */
void TIM1_Config(void)
{
	TIM1_DeInit();
		
	/* Time Base configuration */
  /*
		   TIM1_Prescaler = 4   分频 8 / 4 = 2MHz
       TIM1_Period = 1999   频率 2000K /(1999 +1) = 1KHz
       TIM1_CounterMode = TIM1_COUNTERMODE_UP
			 TIM1_RepetitionCounter = 0
    */
	TIM1_TimeBaseInit(4, TIM1_COUNTERMODE_UP, AUTORELOAD, 0);
	
  // set autoreload register for trigger period
	//TIM1->ARRH= (u8)(AUTORELOAD >> 8);	 
	//TIM1->ARRL= (u8)(AUTORELOAD);			   
	//TIM1_SetAutoreload(AUTORELOAD);
	
	// set compare register for trigger period
	//TIM1->CCR1H= (u8)((AUTORELOAD-AD_STAB) >> 8);   
	//TIM1->CCR1L= (u8)(AUTORELOAD-AD_STAB);
	TIM1_SetCompare1(AUTORELOAD-AD_STAB);
	
	// auto reload register is buferred
	//TIM1->CR1|= TIM1_CR1_ARPE;	  
	TIM1_ARRPreloadConfig(ENABLE);
	
	// CC1REF is used as TRGO
	//TIM1->CR2= (4<<4) & TIM1_CR2_MMS;		            
	TIM1_SelectOutputTrigger(TIM1_TRGOSOURCE_OC1REF);
	
	// CC1REF in PWM 1 mode
	//TIM1->CCMR1= (6<<4) & TIM1_CCMR_OCM;	          
	TIM1_SelectOCxM(TIM1_CHANNEL_1, TIM1_OCMODE_PWM1);
	
	// CC1 interrupt enable
	//TIM1->IER|= TIM1_IER_CC1IE;		     
	TIM1_ITConfig(TIM1_IT_CC1, ENABLE);
	
  // CC1 negative polarity
	//TIM1->CCER1|= TIM1_CCER1_CC1P;	  
	TIM1_OC1PolarityConfig(TIM1_OCPOLARITY_LOW);
	// CC1 output enable
	//TIM1->CCER1|= TIM1_CCER1_CC1E;	  
  TIM1_CCxCmd(TIM1_CHANNEL_1, ENABLE);
		
	// synchronization of TRGO with ADC
	//TIM2->SMCR|=  TIM2_SMCR_MSM;	   
	TIM1_SelectMasterSlaveMode(ENABLE);
	
	
  /* PWM configuration */
  /*
       TIM1_OCMode = TIM1_OCMODE_PWM1
       TIM1_OutputState = TIM1_OUTPUTSTATE_ENABLE
       TIM1_Pulse = INIT_TIM1_CCR1
       TIM1_OCPolarity = TIM1_OCPOLARITY_LOW
  */
  TIM1_OC2Init(TIM1_OCMODE_PWM1, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_DISABLE,
               INIT_TIM1_CCR1, TIM1_OCPOLARITY_LOW, TIM1_OCNPOLARITY_LOW, 
               TIM1_OCIDLESTATE_RESET, TIM1_OCNIDLESTATE_RESET);
		
	/* Update Interrupt Enable */
  TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);
	
	  
  /* Enable TIM1 */
  TIM1_Cmd(ENABLE);
  
  /* Enable CC1 output*/
  TIM1_CtrlPWMOutputs(ENABLE);
	
}

/***********************************************************
  * @brief  Configure TIM4 to generate an update interrupt each 1ms 
  * @param  None
  * @retval None
	**********************************************************
  */
void TIM4_Config(void)
{
  /* TIM4 configuration:
   - TIM4CLK is set to 16 MHz, the TIM4 Prescaler is equal to 128 so the TIM1 counter
   clock used is 16 MHz / 128 = 125 000 Hz
  - With 125 000 Hz we can generate time base:
      max time base is 2.048 ms if TIM4_PERIOD = 255 --> (255 + 1) / 125000 = 2.048 ms
      min time base is 0.016 ms if TIM4_PERIOD = 1   --> (  1 + 1) / 125000 = 0.016 ms
  - In this example we need to generate a time base equal to 1 ms
   so TIM4_PERIOD = (0.001 * 125000 - 1) = 124 */

  /* Time base configuration */
  TIM4_TimeBaseInit(TIM4_PRESCALER_128, TIM4_PERIOD_MS);
  /* Clear TIM4 update flag */
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  /* Enable update interrupt */
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
  
  /* Enable TIM4 */
  TIM4_Cmd(ENABLE);
}


/***********************************************************
  * @brief ADC Init
  * @par Parameters: None
  * @retval void None
  * @par Required preconditions: None
  **********************************************************
  */
void ADC_Config( void )
{
 	//ADC1->CSR= ADC1_CSR_EOCIE | (3 & ADC1_CSR_CH);  
	// ADC EOC interrupt enable, channel 3
	ADC1_ITConfig(ADC1_IT_EOCIE, ENABLE);
	ADC1_ConversionConfig(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_3, ADC1_ALIGN_LEFT);
	
	//ADC1->CR1= 4<<4 & ADC1_CR1_SPSEL;			          
	// master clock/8, single conversion
	ADC1_PrescalerConfig(ADC1_PRESSEL_FCPU_D8);
	
	//ADC1->CR2= ADC1_CR2_EXTTRIG;					          
	// external trigger on timer 1 TRGO, left alignment
	ADC1_ExternalTriggerConfig(ADC1_EXTTRIG_TIM, ENABLE);
	
	//ADC1->TDRH= 2;									                
	// disable Schmitt trigger on AD input 9
	//ADC1->TDRL= 0;                                  
	ADC1_SchmittTriggerConfig(ADC1_SCHMITTTRIG_CHANNEL3, DISABLE);
	
}

/***********************************************************
  * @brief Key Read
  * @par Parameters: None
  * @retval void None
  * @par Required preconditions: None
  **********************************************************
  */
void KeyRead( void )
{
    //unsigned char ReadData =  GPIO_ReadInputData(KEY_GPIO_PORT)^0xFF;   
		//ReadData = ReadData & 0x0E;
		u8 ReadData  = (GPIO_ReadInputData(BUTTON_PORT)^0x80) & 0x80;
    Trg = ReadData & (ReadData ^ Cont);       
    Cont = ReadData;                 
}

/***********************************************************
  * @brief Delay before completing the action
  * @param[in] function action() to be performed once the delay past
  * @param[in] None
  * @retval void None
  * @par Required preconditions: None
  **********************************************************
  */
u8 Key_thread()
{
  PTH_BEGIN
	  while (1)
		{
		  WaitX(20);
		  if(Trg)  
			{  
				 if (Trg & BUTTON_PIN)   
				 {  
					   Range =  (Range - 1) % 4; 
						 if (Range < 0) Range = RNG_100K;
						 LCD_LOCATE(1, 10);
         		 if (Range == RNG_40) {RANGE_40;LCD_printf("%s", "40  ");}
						 if (Range == RNG_1K) {RANGE_1K;LCD_printf("%s", "1K  ");}
						 if (Range == RNG_10K) {RANGE_10K;LCD_printf("%s", "10K ");}
						 if (Range == RNG_100K) {RANGE_100K;LCD_printf("%s", "100K");}
				 }
				 if (Trg & KEY_MENU)
				 { 
				   Mode = (Mode + 1) % 3; 
					 LCD_LOCATE(1, 8);
					 if (Mode == MODE_R)  LCD_printf("%s", "R"); 
					 if (Mode == MODE_C)  LCD_printf("%s", "C"); 
					 if (Mode == MODE_L) 	LCD_printf("%s", "L"); 
		     }
				 if (Trg & KEY_FRQ)
				 { 
				   Frq = (Frq + 1) % 3; 
					 LCD_LOCATE(1, 1);
				   if (Frq == SEL_100) {FRQ_100;LCD_printf("%s", "F 100");}
					 if (Frq == SEL_1K) {FRQ_1K;LCD_printf("%s", "F 1K ");}
					 if (Frq == SEL_7K8) {FRQ_7K8;LCD_printf("%s", "F 7K8");}
				 }
         if (Trg & KEY_REDUCE) 
				 {
						 Range =  (Range - 1) % 4; 
						 if (Range < 0) Range = RNG_100K;
						 LCD_LOCATE(1, 10);
					   if (Range == RNG_40) {RANGE_40;LCD_printf("%s", "40  ");}
						 if (Range == RNG_1K) {RANGE_1K;LCD_printf("%s", "1K  ");}
						 if (Range == RNG_10K) {RANGE_10K;LCD_printf("%s", "10K ");}
						 if (Range == RNG_100K) {RANGE_100K;LCD_printf("%s", "100K");}
					}
		     if (Trg & KEY_PLUS) 
				 { 
						 Range = (Range + 1) % 4; 
						 LCD_LOCATE(1, 10);
					   if (Range == RNG_40) {RANGE_40;LCD_printf("%s", "40  ");}
						 if (Range == RNG_1K) {RANGE_1K;LCD_printf("%s", "1K  ");}
						 if (Range == RNG_10K) {RANGE_10K;LCD_printf("%s", "10K ");}
						 if (Range == RNG_100K) {RANGE_100K;LCD_printf("%s", "100K");}
					}
		  }
	  }
  PTH_END
}	

/**
  * @brief  Init Sin Struct
  * @param  None
  * @retval None
  */
static void LoadValueIntoSinStruct(void)
{
  uint8_t  offset=0,diff=0;
  
  while(offset <= (nbr_of_samples_half/2))
  {
    /*First half of upper half samples of sine wave is loaded into "value" of sin structure.*/
     sin.Value[offset] = sample[offset]; 
     offset++;                      
  }
  diff = (uint8_t)(offset - 1);
  while(offset < nbr_of_samples_half)
  {                                    
      diff--;  
      /* Second half of upper half samples of sine wave is loaded into "value" of sin structure.*/
      sin.Value[offset] = sample[diff];    
      offset++;                         
  }  
  /*Index is initialized to 0.Sin wave upper half is generated at start*/
  sin.Index = 0;                      
  sin.Up = TRUE;                      
}
 
 
/* Public functions --------------------------------------*/
/**
  * @brief Validation firmware main entry point.
  * @par Parameters: None
  * @retval void None
  */  
void main(void)
{
	/* The counter value is initialized to 0.*/
  counter = 0;
  LoadValueIntoSinStruct();           
	
	/* Init CLK */
	CLK_Configuration();
 
  /* Init GPIOs*/
  /* This function sets GPIOs pins according to LCD pins assignment in main.h*/
  GPIO_Configuration();
	
	/* BEEP calibration */
  //BEEP_LSICalibrationConfig(9000);
 
  /* TIM1 configuration */
  TIM1_Config();
  
	/* TIM4 configuration */
  TIM4_Config();
	
	/*  ADC INITIALIZATION  */
	ADC_Config();

	//GPIOC->DDR|= 0x02;						// PC.1 as push-pull outputs
	//GPIOC->CR1|= 0x02;
		
	//GPIOE->CR1&=~0x40;					  // PE.6 as a floating input
	//GPIOE->DDR&=~0x40;
	
  // init ADC variables
	AD_samp= 0;						    // number of stored samples 0
	ADInit= TRUE;             // ADC initialized 
	ADSampRdy= FALSE;         // No sample 
	
	// ADC on
	ADC1_Cmd(ENABLE);
  
	// enable all interrupts 
	enableInterrupts();				   
	
	/* Set power supply on LCD via LCD Power Pin */
  //LCD_PWRON();
  /* Min. delay to wait before initialization after LCD power ON */
  /* Value is ms units*/
  Delay(100);
  /* Initialization of LCD */
  LCD_INIT();
  /* LCD Clear Display */
  LCD_CLEAR_DISPLAY();
  /* Set @CGRAM address start */
  LCD_CMD(CGRAM_address_start);
  /* Loading 6 characters @CGRAM address from STCustom tab */
  LCD_LOAD_CGRAM(STCustom, 6);

	/* Welcome application message while delay 3000ms */
  /* Set cursor to the chosen position*/
  LCD_LOCATE(1, 1);
  /* Print string on LCD (must be ended with \n)*/
  LCD_printstring(" LCR 2.0 \n");
	LCD_LOCATE(2, 2);
  LCD_printstring("XJW Putian,2011\n");
	Delay(3000);
	LCD_CLEAR_DISPLAY();

  /* Init local variables*/
  roll = 0;
  count = 0;
	
  /* Init global variables*/
  ten_ms = 0;
  sec = 0;
  min = 0;
	
	FRQ_1K
	LCD_LOCATE(1, 1);
	LCD_printf("%s", "F 1K ");
	
	LCD_LOCATE(1, 8);
	LCD_printf("%s", "R");
	
	RANGE_40
	LCD_LOCATE(1, 10);
	LCD_printf("%s", "40  ");
	
  //*** MAIN LOOP ***	
	while (1) 
	{
		/* 读键值 */
		KeyRead();
		
		/* 键盘处理 */
		RunTask(Key_thread,1); 
				
		/* 声音处理 */
		//Beep_thread(&pt2);
 		 
		/* ADC取样　*/
		if (ADSampRdy == TRUE)  // field of ADC samples is ready?
		{	 // average of samples
			AD_avg_value= u16_average(&AD_sample[0], AD_samp); 
			
			AD_samp= 0;				        // reinitalize ADC variables
			ADSampRdy= FALSE;
					
      /* Display ADC value at the top right*/
			LCD_LOCATE(1, 13);
      /* Uses special mask*/
      LCD_printf("%4d", AD_avg_value);
		}

    /* If event true (1 sec elapsed) then display rolling messages*/
    if (event == TRUE)
      {
        event = FALSE;
        LCD_LOCATE(2, 11);
        /* Message is displayed at the bottom right*/
        /* Uses special mask*/
        LCD_printf("%5s", message[roll]);
        /* Message changes every time (5 times) then return to first word*/
        /* Table index "roll" is set accordingly*/
        roll = (roll + 1) % 5;
      }
		
		/* Display time counter Min:Sec:Ten ms at the bottom left*/
    LCD_LOCATE(2, 1);
    /* Uses special mask*/
    LCD_printf("%02d:%02d:%02d", min, sec, ten_ms);

    /* Display each 6 (0 to 5) custom characters at same LCD position*/
		//char_pos = 0;
		//while (char_pos < 8)
		//{
      //for (i = 0;i < 6;i++)
      //{
        /* Display bar graph at the top left*/
        //LCD_LOCATE(1, 1);
        //LCD_printstring(disp);
        /* Change custom character to be displayed 0->1->2->3->4->5*/
        //disp[char_pos] = (disp[char_pos] + 1);
       //}
			/* if custom character 5 reached (all columns filled) maintain it display */
      //disp[char_pos] = 5;
      /* and move to next position of bar graph*/
      //char_pos++;
	  //}
	
    /* Reset bar graph display to blank*/
    //for (i = 0;i < 8;i++)
      //disp[i] = 0;
			
    /* Blink display and wait for a while*/
    //LCD_DISP_OFF();
    //Delay(300);
		
    //LCD_DISP_ON();
    //Delay(1000);
	}
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

