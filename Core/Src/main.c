#include "main.h"
//myname
void SystemClock_Config(void);
//andres

// comment 
int main(void)
{
  HAL_Init();

  // Enable GPIOA peripheral clock FIRST
  RCC->AHB2ENR   |=  (RCC_AHB2ENR_GPIOAEN);

  //Setting PA1 up as an output
  GPIOA->MODER &= ~(GPIO_MODER_MODE1);
  GPIOA->MODER |=  (1U << GPIO_MODER_MODE1_Pos);

  //PA1 is low initially
  GPIOA->BRR = (1U << 1);

  //Code from lab manual
  //Enable MCO, select MSI (4 MHz source)
  RCC->CFGR = ((RCC->CFGR & ~(RCC_CFGR_MCOSEL)) | (RCC_CFGR_MCOSEL_0));

  // Configure MCO output on PA8
  GPIOA->MODER   &= ~(GPIO_MODER_MODE8);		// alternate function mode
  GPIOA->MODER   |=  (2 << GPIO_MODER_MODE8_Pos);
  GPIOA->OTYPER  &= ~(GPIO_OTYPER_OT8);		// Push-pull output
  GPIOA->PUPDR   &= ~(GPIO_PUPDR_PUPD8);		// no resistor
  GPIOA->OSPEEDR |=  (GPIO_OSPEEDR_OSPEED8);		// high speed
  GPIOA->AFR[1]  &= ~(GPIO_AFRH_AFSEL8);		// select MCO function

  //Enables TIM2 timer at bit 0
  RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

  //These values create a 5 kHz with a 25% duty cycle square wave
  //using a 4 MHz input clock
  TIM2->PSC = 0;
  TIM2->ARR = 799;
  TIM2->CCR1 = 199;

  //This enables both the update interrupt source and
  //the channel-1 compare interrupt source.
  TIM2->DIER |= TIM_DIER_UIE | TIM_DIER_CC1IE;

  //This enables the TIM2 interrupt at the NVIC level
  NVIC_EnableIRQ(TIM2_IRQn);

  //This enables the timer to start counting
  //TIM2->CNT starts changing automatically
  TIM2->CR1 |= TIM_CR1_CEN;

  while (1)
  {
  }

}


	void TIM2_IRQHandler(void)
	{

		if (TIM2->SR & TIM_SR_CC1IF)
		{
		    GPIOA->BRR = (1U << 1);	// PA1 LOW
		    TIM2->SR = ~TIM_SR_CC1IF;	// clear CC1IF here
		}

		if(TIM2->SR & TIM_SR_UIF)
		{
		    GPIOA->BSRR = (1U << 1);	// PA1 HIGH
		    TIM2->SR = ~TIM_SR_UIF;	//clear UIF here
		}

	}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
