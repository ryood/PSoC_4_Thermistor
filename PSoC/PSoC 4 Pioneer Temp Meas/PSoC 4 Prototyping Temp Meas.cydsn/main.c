/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>
#include <stdio.h>

#define TITLE_STR1  ("PSoC 4 Thermo")
#define TITLE_STR2  ("20170116")

#define VREF            (0x7FF)
#define CH_VREF         (1u)
#define CH_THERMISTOR   (0u)

#define INTERVAL        (1000u) // us
#define DEGREE_CHR      (0xDF)  // °

int abs(int i)
{
    return (i >= 0) ? i : -i;
}

int main()
{
    char uartLine[80];
    int16 vRef, vTherm;
    uint32 rTherm;
    int16 temperautre;
    int count;
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    Opamp_1_Start();
    
    UART_1_Start();
    UART_1_UartPutString("\r\n\r\nThemistor Test..\r\n");
    
    LCD_Char_1_Start();
    LCD_Char_1_PrintString(TITLE_STR1);
    LCD_Char_1_Position(1, 0);
    LCD_Char_1_PrintString(TITLE_STR2);
        
    ADC_SAR_Seq_1_Start();
    
    CyDelay(1000);
    
    count = 0;
    for(;;)
    {
        ADC_SAR_Seq_1_StartConvert();
        while (ADC_SAR_Seq_1_IsEndConversion(ADC_SAR_Seq_1_RETURN_STATUS) == 0u) {  
        // 変換終了を待つ  
            ;  
        }
        vTherm = ADC_SAR_Seq_1_GetResult16(CH_THERMISTOR);
        vRef   = ADC_SAR_Seq_1_GetResult16(CH_VREF);
        
        rTherm = Thermistor_mid_GetResistance(vRef, vTherm);
        if (rTherm > 272800) {
            // -50℃～0℃のThermistorCalcで計算
            rTherm = Thermistor_low_GetResistance(vRef, vTherm);
            temperautre = Thermistor_low_GetTemperature(rTherm);
        } else if (rTherm < 4160) {
            // 50℃～110℃のThermistorCalcで計算
            rTherm = Thermistor_high_GetResistance(vRef, vTherm);
            temperautre = Thermistor_high_GetTemperature(rTherm);
        } else {
            // 0℃～50℃のThermistorCalcで計算
            temperautre = Thermistor_mid_GetTemperature(rTherm);
        }
        
        sprintf(uartLine, "%d,\t%d.%02d,\t%lu,\t%d,\t%d,\t%d\r\n",
            count,
            temperautre / 100,
            abs(temperautre % 100),
            rTherm,
            vTherm,
            vRef,
            vTherm+vRef
        );
        UART_1_UartPutString(uartLine);
        
        sprintf(uartLine, "%d sec", count);
        LCD_Char_1_ClearDisplay();
        LCD_Char_1_PrintString(uartLine);
        sprintf(uartLine, "%d.%02d%cC", temperautre / 100, abs(temperautre % 100), DEGREE_CHR);
        LCD_Char_1_Position(1, 0);
        LCD_Char_1_PrintString(uartLine);
        
        CyDelay(INTERVAL);
        count++;
    }
}

/* [] END OF FILE */
