    void  task1(){
    static unsigned char lc=0; switch(lc){   case 0:
    keyread()


请贴出完整代码。
最新小小调度器版本在493楼, 评测版在530楼。
http://www.amobbs.com/thread-5508723-1-1.html
	
回复

举报
	
holts2

8
主题 	

0
好友 	

287
积分

中级会员

Rank: 3Rank: 3

莫元
    142 

	
277楼
发表于 2012-12-13 13:57:56 |只看该作者
本帖最后由 holts2 于 2012-12-13 15:12 编辑

    smset 发表于 2012-12-13 13:17
    请贴出完整代码。



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
    u8  temp_AD_H;                                           // temporary registers for reading ADC result
    u8  temp_AD_L;             // temporary registers for reading ADC result
    u8        ADInit;                                                     // flag for ADC initialized
    u8        ADSampRdy;                                     // flag for filed of samples ready
    u8  AD_samp;                                                   // counter of stored samples
    u16 AD_sample[NUMB_SAMP];         // store samples field
    u16 AD_avg_value;                                   // average of ADC result
           
    u8  peak_memo;                                           // variables for peak level detector
    u8  peak_filt;                                           // variables for peak level detector*/
           
    volatile u8 currid;
    volatile u8 timers[MAXTASKS];

    u8  Trg;
    u8  Cont;

    u8  Mode = MODE_R;
    u8  Frq = SEL_100;

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

    /* Define DDS */
    //查询表中不可装载零值，否则会造成无中断产生
    u8 sinB[256] =
      {
    255,255,255,255,255,255,254,254,253,252,252,251,250,249,248,247,246,245,243,242,240,239,237,236,234,232,230,229,227,225,222,220,
    218,216,214,211,209,206,204,201,199,196,194,191,188,185,183,180,177,174,171,168,165,162,159,156,153,150,147,144,140,137,134,131,
    128,125,122,119,116,112,109,106,103,100, 97, 94, 91, 88, 85, 82, 79, 76, 73, 71, 68, 65, 62, 60, 57, 55, 52, 50, 47, 45, 42, 40,
      38, 36, 34, 31, 29, 27, 26, 24, 22, 20, 19, 17, 16, 14, 13, 11, 10,  9,  8,  7,  6,  5,  4,  4,  3,  2,  2,  1,  1,  1,  1,  1,
       1,  1,  1,  1,  1,  1,  2,  2,  3,  4,  4,  5,  6,  7,  8,  9, 10, 11, 13, 14, 16, 17, 19, 20, 22, 24, 26, 27, 29, 31, 34, 36,
      38, 40, 42, 45, 47, 50, 52, 55, 57, 60, 62, 65, 68, 71, 73, 76, 79, 82, 85, 88, 91, 94, 97,100,103,106,109,112,116,119,122,125,
    128,131,134,137,140,144,147,150,153,156,159,162,165,168,171,174,177,180,183,185,188,191,194,196,199,201,204,206,209,211,214,216,
    218,220,222,225,227,229,230,232,234,236,237,239,240,242,243,245,246,247,248,249,250,251,252,252,253,254,254,255,255,255,255,255
      };

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
    /* ------------------------------------------------------ */
    /**
      * @brief Prepare data for four LED bar of signal and peak indicator
      * @par Parameters:
      * val: Level of the mesured signal [0-4]
      * @retval 4 bits (low nibble) of the composite bar graph information
      */
    u8 signal_and_peak_level(u8 val) {
            u8 signal;
            u8 peak;
              
            switch(val) {
                    case 0: peak= 0; signal=  0; break;        //set peak and signal levels
                    case 1: peak= 1; signal=  1; break;
                    case 2: peak= 2; signal=  3; break;
                    case 3: peak= 4; signal=  7; break;
                    case 4: peak= 8; signal= 15; break;
                    default: peak= signal= 15;
            };
            if(peak_filt == 0) {                                                                        // slow fall of peak level indicator
                    if(peak_memo) {
                            peak_memo>>= 1;
                            peak_filt= PEAK_FILTER;
                    };
            }
            else
                    --peak_filt;
            if(peak >= peak_memo) {                                       // check the highest level value
                    peak_memo= peak;                                                 // and copy it to peak indicator
                    peak_filt= PEAK_FILTER;                               // with fall speed refresh
            };
            return (signal | peak_memo);                     // return bar graph information
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
    { /* Fmaster = 16MHz */
      CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
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
                       TIM1_Prescaler = 8   分频 16/8 = 2000 KHz
           TIM1_Period = 8000      频率 2000 KHz /(7999 + 1) = 250KHz
           TIM1_CounterMode = TIM1_COUNTERMODE_UP
                             TIM1_RepetitionCounter = 0
        */
                    TIM1_TimeBaseInit(8, TIM1_COUNTERMODE_UP, AUTORELOAD - 1, 0);
                   
                    /* Channel 2 Configuration in PWM mode */
        /*
           TIM1_OCMode = TIM1_OCMODE_PWM2
           TIM1_OutputState = TIM1_OUTPUTSTATE_ENABLE
           TIM1_OutputNState = TIM1_OUTPUTNSTATE_ENABLE
           TIM1_Pulse = DutyCycle
           TIM1_OCPolarity = TIM1_OCPOLARITY_LOW
           TIM1_OCNPolarity = TIM1_OCNPOLARITY_HIGH
           TIM1_OCIdleState = TIM1_OCIDLESTATE_SET
           TIM1_OCNIdleState = TIM1_OCIDLESTATE_RESET
        */
                    TIM1_OC1Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_ENABLE, FRQ_100HZ/2,
                   TIM1_OCPOLARITY_LOW, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_SET,
                   TIM1_OCNIDLESTATE_RESET);
                    TIM1_OC2Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_ENABLE, FRQ_100HZ/2,
                   TIM1_OCPOLARITY_LOW, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_SET,
                   TIM1_OCNIDLESTATE_RESET);                                         
                    TIM1_OC3Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_ENABLE, FRQ_100HZ/2,
                   TIM1_OCPOLARITY_LOW, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_SET,
                   TIM1_OCNIDLESTATE_RESET);
        TIM1_OC4Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, FRQ_100HZ/2, TIM1_OCPOLARITY_LOW,
                   TIM1_OCIDLESTATE_SET);
                                                                           
                    /* TIM1 counter enable */
        TIM1_Cmd(ENABLE);
                     
                    /* TIM1 Main Output Enable */
        TIM1_CtrlPWMOutputs(ENABLE);
    }

    /***********************************************************
      * @brief TIM2 init
      * @par Parameters: None
      * @retval void None
      * @par Required preconditions: None
      **********************************************************
      */
    void TIM2_Config(void)
    {
        TIM2_DeInit();
                   
                    /* Time Base configuration  
                       TIM1_Prescaler = 8   分频16/8 = 2 MHz
           TIM1_Period = 20      频率 2000 K /(20-1) = 100KHz
        */
                    TIM2_TimeBaseInit(TIM2_PRESCALER_8, AD_STAB);
                    /* Channel 2 Configuration in PWM mode */
        /*
           TIM1_OCMode = TIM1_OCMODE_PWM2
           TIM1_OutputState = TIM1_OUTPUTSTATE_ENABLE
           TIM1_OutputNState = TIM1_OUTPUTNSTATE_ENABLE
           TIM1_Pulse = DutyCycle
           TIM1_OCPolarity = TIM1_OCPOLARITY_LOW
           TIM1_OCNPolarity = TIM1_OCNPOLARITY_HIGH
           TIM1_OCIdleState = TIM1_OCIDLESTATE_SET
           TIM1_OCNIdleState = TIM1_OCIDLESTATE_RESET
        */
                    TIM1_OC1Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_ENABLE, FRQ_100HZ/2,
                   TIM1_OCPOLARITY_LOW, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_SET,
                   TIM1_OCNIDLESTATE_RESET);
                    TIM1_OC2Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_ENABLE, FRQ_100HZ/2,
                   TIM1_OCPOLARITY_LOW, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_SET,
                   TIM1_OCNIDLESTATE_RESET);                                         
                    TIM1_OC3Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_ENABLE, FRQ_100HZ/2,
                   TIM1_OCPOLARITY_LOW, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_SET,
                   TIM1_OCNIDLESTATE_RESET);
        TIM1_OC4Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, FRQ_100HZ/2, TIM1_OCPOLARITY_LOW,
                   TIM1_OCIDLESTATE_SET);
                           
        /* auto reload register is buferred */
        TIM2_ARRPreloadConfig(ENABLE);
                    /* TIM1 counter enable */
        TIM1_Cmd(ENABLE);
                     
                    /* TIM1 Main Output Enable */
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
    void Key_thread()
    {
      PTH_BEGIN
              KeyRead();
                    if (Trg)
                    {
                      WaitX(20);
                      KeyRead();
                      if(Cont)  
                            {  
                                     if (Cont & BUTTON_PIN)   
               { Mode = (Mode + 1) % 3; }
                                     if (Cont & KEY_FRQ)
                                             { Frq = (Frq + 1) % 3; }
             if (Cont & KEY_REDUCE)
                 { Mode = MODE_R; }
                         if (Cont & KEY_PLUS)
                { Mode = MODE_R; }
                      }
                    }
      PTH_END
    }       




    /* Public functions --------------------------------------*/
    /**
      * @brief Validation firmware main entry point.
      * @par Parameters: None
      * @retval void None
      */  
    void main(void)
    {
            /* Init CLK */
            CLK_Configuration();

      /* Init GPIOs*/
      /* This function sets GPIOs pins according to LCD pins assignment in main.h*/
      GPIO_Configuration();
           
            /* BEEP calibration */
      //BEEP_LSICalibrationConfig(9000);

      //TIM1_Config();
      
            /* TIM4 configuration */
      TIM4_Config();
           

            //GPIOC->DDR|= 0x02;                                                // PC.1 as push-pull outputs
            //GPIOC->CR1|= 0x02;
                   
            //GPIOE->CR1&=~0x40;                                          // PE.6 as a floating input
            //GPIOE->DDR&=~0x40;
           
      //         *** ADC INITIALIZATION ***
           
            // set autoreload register for trigger period
            //TIM1->ARRH= (u8)(AUTORELOAD >> 8);         
            //TIM1->ARRL= (u8)(AUTORELOAD);                          
            TIM1_SetAutoreload(AUTORELOAD);
           
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
           
            //TIM1->BKR|= TIM1_BKR_MOE;                                                                                               
            TIM1_CtrlPWMOutputs(ENABLE);
           
            // synchronization of TRGO with ADC
            //TIM2->SMCR|=  TIM2_SMCR_MSM;          
            TIM1_SelectMasterSlaveMode(ENABLE);
           
            // timer 1 enable
            //TIM1->CR1|= TIM1_CR1_CEN;                                                      
            TIM1_Cmd(ENABLE);       
                   
                   
            ADC1->CSR= ADC1_CSR_EOCIE | (3 & ADC1_CSR_CH);  // ADC EOC interrupt enable, channel 3
            ADC1->CR1= 4<<4 & ADC1_CR1_SPSEL;                                  // master clock/8, single conversion
            ADC1->CR2= ADC1_CR2_EXTTRIG;                                                  // external trigger on timer 1 TRGO, left alignment
            ADC1->TDRH= 2;                                                                                        // disable Schmitt trigger on AD input 9
            ADC1->TDRL= 0;                                  //

      // init ADC variables
            AD_samp= 0;                                                    // number of stored samples 0
            ADInit= TRUE;             // ADC initialized
            ADSampRdy= FALSE;         // No sample
           
            ADC1->CR1|= ADC1_CR1_ADON;   // ADC on

            enableInterrupts();                                   // enable all interrupts
           
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
           
           
      //*** MAIN LOOP ***       
            while (1)
            {
                    /* 键盘处理 */
                    RunTask(Key_thread,1);
                                   
                    /* 声音处理 */
                    //Beep_thread(&pt2);
                     
                    /* ADC取样　*/
                    if (ADSampRdy == TRUE)  // field of ADC samples is ready?
                    {         // average of samples
                            AD_avg_value= u16_average(&AD_sample[0], AD_samp);
                           
                            AD_samp= 0;                                        // reinitalize ADC variables
                            ADSampRdy= FALSE;
                           
                            // setting LED status       
                            //leds= signal_and_peak_level((u8)((AD_avg_value + 128) / 256));
                           
          /* Display ADC value at the top right*/
                            LCD_LOCATE(1, 11);
          /* Uses special mask*/
          LCD_printf("%5d", AD_avg_value);
                    }
                   
                    if (Frq == SEL_100)
                    {
                            LCD_LOCATE(1, 1);
                            LCD_printf("%s", "F 100");
                    }
                    if (Frq == SEL_1K)
                    {
                            LCD_LOCATE(1, 1);
                            LCD_printf("%s", "F 1K ");
                    }
        if (Frq == SEL_7K8)
                    {
                            LCD_LOCATE(1, 1);
                            LCD_printf("%s", "F 7K8");
                    }
                   
                    if (Mode == MODE_R)
                    {
                            LCD_LOCATE(1, 10);
                            LCD_printf("%s", "R");
                    }
                    if (Mode == MODE_C)
                    {
                            LCD_LOCATE(1, 10);
                            LCD_printf("%s", "C");
                    }
                    if (Mode == MODE_L)
                    {
                            LCD_LOCATE(1, 10);
                            LCD_printf("%s", "L");
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



