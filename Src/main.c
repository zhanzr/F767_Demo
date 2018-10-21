/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f7xx_hal.h"
#include "dma.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <stdlib.h>

#include "jansson.h"  
#include "cJSON.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
typedef struct str_poi_record
{
    const char *precision;
    double lat;
    double lon;
    const char *address;
    const char *city;
    const char *state;
    const char *zip;
    const char *country;
}poi_record;

/* Our "days of the week" array: */
const char *str_weekdays[7] =
{
		"Sunday",
		"Monday",
		"Tuesday",
		"Wednesday",
		"Thursday",
		"Friday",
		"Saturday"
};

/* Our matrix: */
const int numbers[3][3] =
{
		{0, -1, 0},
		{1, 0, 0},
		{0 ,0, 1}
};

/* Our "gallery" item: */
const int ids[4] = { 116, 943, 234, 38793 };
/* Our array of "records": */
const poi_record fields[2] =
{
		{
				"zip",
				37.7668,
				-1.223959e+2,
				"",
				"SAN FRANCISCO",
				"CA",
				"94107",
				"US"
		},
		{
				"zip",
				37.371991,
				-1.22026e+2,
				"",
				"SUNNYVALE",
				"CA",
				"94085",
				"US"
		}
};

void test_jansson(void)
{
	printf("Jansson Version:%s\n", JANSSON_VERSION);
	
	json_t* root;		
	char* str_dump;	
	
	//Object Video Creating Test
	{
		json_t* fmt;
    root = json_object();
		json_object_set_new(root, "name", json_string("Jack (\"Bee\") Nimble"));
		json_object_set_new(root, "format", fmt=json_object());
		json_object_set_new(fmt,  "type", json_string("rect"));
    json_object_set_new(fmt, "width", json_integer(1920));
    json_object_set_new(fmt, "height", json_integer(1080));
    json_object_set_new(fmt, "interlace", json_boolean(false));
    json_object_set_new(fmt, "frame rate", json_integer(24));		
		
		str_dump = json_dumps( root, 0 );
		printf( str_dump );
		printf("\n");
		free( str_dump );
		json_decref( root );		
	}
	
	//1-Dim Array of String Test
	{	
    root = json_array();

    for(size_t i=0; i<7; ++i ) 
		{       
			json_array_append_new( root, json_string(str_weekdays[i]) );
    }
		
		str_dump = json_dumps( root, 0 );
		printf( str_dump );
		printf("\n");
		free( str_dump );
		json_decref( root );		
	}
	
	//2-Dim Array of Integer Test
	{	
    root = json_array();

    for(size_t i=0; i<3; ++i ) 
		{  
			json_t* jarr2 = json_array();
			for(size_t j=0; j<3; ++j ) 
			{
				json_array_append_new( jarr2, json_integer((json_int_t)numbers[i][j]));
			}
			json_array_append_new( root, jarr2);
    }
		
		str_dump = json_dumps( root, 0 );
		printf( str_dump );
		printf("\n");
		free( str_dump );
		json_decref( root );		
	}
	
	//Object Gallery Creating Test
	{
		json_t* img;
		json_t* thm;
		
    root = json_object();
		json_object_set_new(root, "Image", img=json_object());
		json_object_set_new(img, "Width", json_integer(800));
		json_object_set_new(img, "Height", json_integer(600));
		json_object_set_new(img,  "Title", json_string("View from 15th Floor"));
    json_object_set_new(img, "Thumbnail", thm=json_object());
    json_object_set_new(thm, "Url", json_string("http:/*www.example.com/image/481989943"));
		json_object_set_new(thm, "Height", json_integer(125));
		json_object_set_new(thm, "Width", json_string("100"));
		
		json_t* jarr = json_array();		
		for(size_t i=0; i<sizeof(ids)/sizeof(ids[0]); ++i)
		{
			json_array_append_new( jarr, json_integer((json_int_t)ids[i]));
		}
    json_object_set_new(img, "IDs", jarr);		
		
		str_dump = json_dumps( root, 0 );
		printf( str_dump );
		printf("\n");
		free( str_dump );
		json_decref( root );				
	}
	
	//Array of "records" Test
	{
    root = json_array();
		
    for (size_t i = 0; i < 2; i++)
    {
				json_t* item = json_object();	
			
        json_object_set_new(item, "precision", json_string(fields[i].precision));
        json_object_set_new(item, "Latitude", json_real(fields[i].lat));
        json_object_set_new(item, "Longitude", json_real(fields[i].lon));
        json_object_set_new(item, "Address", json_string(fields[i].address));
        json_object_set_new(item, "City", json_string(fields[i].city));
        json_object_set_new(item, "State", json_string(fields[i].state));
        json_object_set_new(item, "Zip", json_string(fields[i].zip));
        json_object_set_new(item, "Country", json_string(fields[i].country));
			
				json_array_append_new( root, item);			
    }
		
		str_dump = json_dumps( root, 0 );
		printf( str_dump );
		printf("\n");
		free( str_dump );
		json_decref( root );				
	}
	
	//null Test
	{
		root = json_object();
    volatile double zero = 0.0;
		
		json_object_set_new(root, "number", json_null());
		
		str_dump = json_dumps( root, 0 );
		printf( str_dump );
		printf("\n");
		free( str_dump );
		json_decref( root );				
	}
}

/* Create a bunch of objects as demonstration. */
static int print_preallocated(cJSON *root)
{
    /* declarations */
    char *out = NULL;
    char *buf = NULL;
    char *buf_fail = NULL;
    size_t len = 0;
    size_t len_fail = 0;

    /* formatted print */
    out = cJSON_Print(root);

    /* create buffer to succeed */
    /* the extra 5 bytes are because of inaccuracies when reserving memory */
    len = strlen(out) + 5;
    buf = (char*)malloc(len);
    if (buf == NULL)
    {
        printf("Failed to allocate memory.\n");
        exit(1);
    }

    /* create buffer to fail */
    len_fail = strlen(out);
    buf_fail = (char*)malloc(len_fail);
    if (buf_fail == NULL)
    {
        printf("Failed to allocate memory.\n");
        exit(1);
    }

    /* Print to buffer */
    if (!cJSON_PrintPreallocated(root, buf, (int)len, 1)) {
        printf("cJSON_PrintPreallocated failed!\n");
        if (strcmp(out, buf) != 0) {
            printf("cJSON_PrintPreallocated not the same as cJSON_Print!\n");
            printf("cJSON_Print result:\n%s\n", out);
            printf("cJSON_PrintPreallocated result:\n%s\n", buf);
        }
        free(out);
        free(buf_fail);
        free(buf);
        return -1;
    }

    /* success */
    printf("%s\n", buf);

    /* force it to fail */
    if (cJSON_PrintPreallocated(root, buf_fail, (int)len_fail, 1)) {
        printf("cJSON_PrintPreallocated failed to show error with insufficient memory!\n");
        printf("cJSON_Print result:\n%s\n", out);
        printf("cJSON_PrintPreallocated result:\n%s\n", buf_fail);
        free(out);
        free(buf_fail);
        free(buf);
        return -1;
    }

    free(out);
    free(buf_fail);
    free(buf);
    return 0;
}

/* Create a bunch of objects as demonstration. */
static void create_objects(void)
{
    /* declare a few. */
    cJSON *root = NULL;
    cJSON *fmt = NULL;
    cJSON *img = NULL;
    cJSON *thm = NULL;
    cJSON *fld = NULL;
    int i = 0;

    volatile double zero = 0.0;

    /* Here we construct some JSON standards, from the JSON site. */

    /* Our "Video" datatype: */
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("Jack (\"Bee\") Nimble"));
    cJSON_AddItemToObject(root, "format", fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "type", "rect");
    cJSON_AddNumberToObject(fmt, "width", 1920);
    cJSON_AddNumberToObject(fmt, "height", 1080);
    cJSON_AddFalseToObject (fmt, "interlace");
    cJSON_AddNumberToObject(fmt, "frame rate", 24);

    /* Print to text */
    if (print_preallocated(root) != 0) {
        cJSON_Delete(root);
        exit(EXIT_FAILURE);
    }
    cJSON_Delete(root);

    /* Our "days of the week" array: */
    root = cJSON_CreateStringArray(str_weekdays, 7);

    if (print_preallocated(root) != 0) {
        cJSON_Delete(root);
        exit(EXIT_FAILURE);
    }
    cJSON_Delete(root);

    /* Our matrix: */
    root = cJSON_CreateArray();
    for (i = 0; i < 3; i++)
    {
        cJSON_AddItemToArray(root, cJSON_CreateIntArray(numbers[i], 3));
    }

    /* cJSON_ReplaceItemInArray(root, 1, cJSON_CreateString("Replacement")); */

    if (print_preallocated(root) != 0) {
        cJSON_Delete(root);
        exit(EXIT_FAILURE);
    }
    cJSON_Delete(root);

    /* Our "gallery" item: */
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "Image", img = cJSON_CreateObject());
    cJSON_AddNumberToObject(img, "Width", 800);
    cJSON_AddNumberToObject(img, "Height", 600);
    cJSON_AddStringToObject(img, "Title", "View from 15th Floor");
    cJSON_AddItemToObject(img, "Thumbnail", thm = cJSON_CreateObject());
    cJSON_AddStringToObject(thm, "Url", "http:/*www.example.com/image/481989943");
    cJSON_AddNumberToObject(thm, "Height", 125);
    cJSON_AddStringToObject(thm, "Width", "100");
    cJSON_AddItemToObject(img, "IDs", cJSON_CreateIntArray(ids, 4));

    if (print_preallocated(root) != 0) {
        cJSON_Delete(root);
        exit(EXIT_FAILURE);
    }
    cJSON_Delete(root);

    /* Our array of "records": */
    root = cJSON_CreateArray();
    for (i = 0; i < 2; i++)
    {
        cJSON_AddItemToArray(root, fld = cJSON_CreateObject());
        cJSON_AddStringToObject(fld, "precision", fields[i].precision);
        cJSON_AddNumberToObject(fld, "Latitude", fields[i].lat);
        cJSON_AddNumberToObject(fld, "Longitude", fields[i].lon);
        cJSON_AddStringToObject(fld, "Address", fields[i].address);
        cJSON_AddStringToObject(fld, "City", fields[i].city);
        cJSON_AddStringToObject(fld, "State", fields[i].state);
        cJSON_AddStringToObject(fld, "Zip", fields[i].zip);
        cJSON_AddStringToObject(fld, "Country", fields[i].country);
    }

    /* cJSON_ReplaceItemInObject(cJSON_GetArrayItem(root, 1), "City", cJSON_CreateIntArray(ids, 4)); */

    if (print_preallocated(root) != 0) {
        cJSON_Delete(root);
        exit(EXIT_FAILURE);
    }
    cJSON_Delete(root);

    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "number", 1.0 / zero);

    if (print_preallocated(root) != 0) {
        cJSON_Delete(root);
        exit(EXIT_FAILURE);
    }
    cJSON_Delete(root);
}

void 	test_cJSON(void)
{
	printf("cJSON Version: %s\n", cJSON_Version());

	/* Now some samplecode for building objects concisely: */
	create_objects();		
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */
  /* Enable I-Cache-------------------------------------------------------------*/
  SCB_EnableICache();
  /* Enable D-Cache-------------------------------------------------------------*/
  SCB_EnableDCache();
  /* MCU Configuration----------------------------------------------------------*/
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  /* USER CODE BEGIN Init */
  /* USER CODE END Init */
  /* Configure the system clock */
  SystemClock_Config();
  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  /* USER CODE BEGIN 2 */

	printf("%u MHz Test cJSON\n",
	SystemCoreClock);
	
	test_jansson();
	
	test_cJSON();	
	/* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Activate the Over-Drive mode 
    */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
