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

#define VREF            (0x7FF)
#define CH_VREF         (1u)
#define CH_THERMISTOR   (0u)

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
        
    ADC_SAR_Seq_1_Start();
    
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
        
        rTherm = Thermistor_1_GetResistance(vRef, vTherm);
        temperautre = Thermistor_1_GetTemperature(rTherm);
        
        sprintf(uartLine, "%d.%d,\t%d.%d,\t%lu,\t%d,\t%d,\t%d\r\n",
            count / 10,
            count % 10,
            temperautre / 100,
            temperautre % 100,
            rTherm,
            vTherm,
            vRef,
            vTherm+vRef
        );
        UART_1_UartPutString(uartLine);
        
        CyDelay(100);
        count++;
    }
}

/* [] END OF FILE */
