#include <project.h>
#include "math.h"
#include <stdio.h>
//* Declaramos las funciones que vamos a ejecutar

void publicar(void); 
void I2C(void); 
void sensor(void);

/* Direccion de eslavos I2C  */
#define I2C_SLAVE_ADDR1  (0x08u)
#define I2C_SLAVE_ADDR2  (0x10u)

/*Buffer de lectura y tamaño*/
#define BUFFER_SIZE     (11)//cantidad de datos a recibir
#define PACKET_SIZE     (BUFFER_SIZE)//tamaño del paquete a recibir
uint8 i2cReadBuffer1[BUFFER_SIZE];// Buffer de lectura I2C
uint8 i2cReadBuffer2[BUFFER_SIZE];// Buffer de lectura I2C

char8 buffist[20];// variable de visualizacion
float distance=0; // variable distancia
float largo = 60.5,ancho = 26,max = 45.5; //medidas del tanque, max = altura
float tanque,prom,medida; //variables par almacenar el valor en litros del tanque, promedio de uso, y la meddida invertida del tanque
float m =0,c=0, temp, valor; // m= distancia medida en uS, C= cte de velocidad del sonido respecto a la temperatura, Temp=pemperatura, Valore= resultado adc
uint8 i; // variable de cuentas 
char8 dispbuff[BUFFER_SIZE]; //variable de visualizacion

 CY_ISR(Timer_ISR_handler)
{
    temp=(((5*valor)/4095)*100)-3; //temperatura °c
    c = (0.596*temp)+331.5; //relacion velocidad del sonido
    c=(c * 100 / 1000000); // conversion a cm/us
    Timer_ClearInterrupt(Timer_INTR_MASK_CC_MATCH); // Limpiamos la interrupción
    m= Timer_ReadCounter();// datasheet del modulo HC SR04 indica que el tiempo en ir y volver de la señal es de 58us para 1cm  
    distance =m/2*c;// Como la conversion anterior de c calcula solo el tiempo en llegar la señal al objeto en el cual rebotara, esta se multiplica por dos para tener en cuenta amos espacio de tiempo
    
}

int main( void ) 
{   
    ADC_Start();
    ADC_StartConvert();
    I2CM_Start();
	Timer_Start();         				// Iinicia el timer del sensor de ultrasonido
	LCD_Start();
    UART_Start();
    LCD_DisplayOn();
    LCD_LoadCustomFonts(LCD_customFonts); 
    CyGlobalIntEnable; 
    ISR_StartEx(Timer_ISR_handler);

	for(;;) 
    {
    
    sensor();
    publicar();
    CyDelay(1000);
    I2C();
    sprintf(buffist,"%.2f,%.3f",distance,temp);
                for ( i=0; i<BUFFER_SIZE; i++)
                {
                UART_UartPutChar(buffist[i]);
                }
                UART_UartPutChar(0x2C);// se envia una coma ","
                for ( i=0; i<BUFFER_SIZE; i++) 
                {
                UART_UartPutChar(i2cReadBuffer1[i]);
                }
                UART_UartPutChar(0x2C);
                
                for ( i=0; i<BUFFER_SIZE; i++) 
                {
                UART_UartPutChar(i2cReadBuffer2[i]);
                }
                UART_UartPutChar(0x2C);
    }
	
}
void sensor(void){
        Trigger_Write(1); //flanco de subida para el sensor
        CyDelayUs(10);    //Delay de iniciar el sensor
        Trigger_Write(0); // fin de la bandera
        ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
        valor=ADC_GetResult16(0); // Resultados del ADC
        medida = max -distance;// relacion distancia con el tanque
        tanque=(medida*largo*ancho)*0.001;// calculo de volumen
        prom=(100*(max-distance))/max; // promedio de uso en el tanque
}
void publicar (void)
    {
        LCD_ClearDisplay();

         if(distance <= max || distance >= 3){ // si la distancia es aceptable 

                LCD_Position(0,0);
                sprintf(dispbuff, "%.2fcm, %.2f",distance,temp);
                LCD_PrintString(dispbuff);
                LCD_PutChar(LCD_CUSTOM_0); 
                LCD_PrintString("C");
                LCD_Position(1u,0u);
                sprintf(dispbuff, "%.2flt, %.2f",tanque,prom);                 
                LCD_PrintString(dispbuff);              
                LCD_PrintString("%");

                Led_Write(1);
                PWM_Stop();

        }
  
     if(distance >= max || distance <= 3){// si se tiene error en la medida
            PWM_Start();
            PWM_WriteCompare(127);
            LCD_ClearDisplay();
            LCD_Position(0u,5u);									// Position cursor
        	LCD_PrintString("ERROR");
            sprintf(dispbuff, "%.2fcm, %.2fcm",distance,medida);// Position cursor
            LCD_Position(1u,0u);									// Position cursor
        	LCD_PrintString(dispbuff);
        }
    }
void I2C (void){
    //Primer sensor   
    I2CM_I2CMasterReadBuf(I2C_SLAVE_ADDR1, i2cReadBuffer1, PACKET_SIZE, I2CM_I2C_MODE_COMPLETE_XFER);
    while (0u == (I2CM_I2CMasterStatus() & I2CM_I2C_MSTAT_RD_CMPLT)) {}//Se epera mientras se completa la transferencia
    (void) I2CM_I2CMasterClearStatus();
    // Fin primer sensor 
    
    //Segundo sensor 
    I2CM_I2CMasterReadBuf(I2C_SLAVE_ADDR2, i2cReadBuffer2, PACKET_SIZE, I2CM_I2C_MODE_COMPLETE_XFER);
    while (0u == (I2CM_I2CMasterStatus() & I2CM_I2C_MSTAT_RD_CMPLT)) {}// Se epera mientras se completa la transferencia
    (void) I2CM_I2CMasterClearStatus();
    // Fin segundo sensor */
}
