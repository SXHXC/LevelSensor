#include <project.h>
#include <math.h>
#include <stdio.h>

/* Declaramos las funciones que vamos a ejecutar en la interrución del SysTick */
void publicar(void); 
void sensor(void);

/* Buffer and packet size */
#define BUFFER_SIZE     (22)
#define PACKET_SIZE     (BUFFER_SIZE)
uint8 i2cReadBuffer[BUFFER_SIZE];
char8 buffist[BUFFER_SIZE];

float distance=0;
float largo = 70,radio=21.71332,max = 43.42664; //medidas del tanque, max = altura
float tanque,prom,medida; //variables par almacenar el valor en litros del tanque, promedio de uso, y la meddida invertida del tanque
float m =0,c=0, temp, valor; // m= distancia medida en uS, C= cte de velocidad del sonido respecto a la temperatura, Temp=pemperatura, Valore= resultado adc
uint8 i;
char8 dispbuff[BUFFER_SIZE];


CY_ISR(Timer_ISR_handler)
{

    temp=((5*valor)/4095)*100; //temperatura °c
    c = (0.596*temp)+331.5; //velocidad del sonido
    c=(c * 100 / 1000000); // conversion a cm/us
    Timer_ClearInterrupt(Timer_INTR_MASK_CC_MATCH); // Limpiamos la interrupción
    m= Timer_ReadCounter();// datasheet del modulo HC SR04 indica que el tiempo en ir y volver de la señal es de 58us para 1cm  
    distance =m/2*c;// Como la conversion anterior de c calcula solo el tiempo en llegar la señal al objeto en el cual rebotara, esta se multiplica por dos para tener en cuenta amos espacio de tiempo
}
int main( void ) 
{   /*iniccializamos los modulos del psoc*/
    ADC_Start();
    ADC_StartConvert();
    I2C_I2CSlaveInitReadBuf (i2cReadBuffer,  BUFFER_SIZE);
    I2C_Start();
    Timer_Start();
	LCD_Start();
    LCD_DisplayOn();
    LCD_LoadCustomFonts(LCD_customFonts); 
	CyGlobalIntEnable; 		
    ISR_StartEx(Timer_ISR_handler);
    
	for(;;) 
    { 
        sensor();
        publicar();
        
        sprintf(buffist,"%.2f,%.3f,%.2f,%.2f",distance,temp,tanque,prom);

                    

        if (0u != (I2C_I2CSlaveStatus() & I2C_I2C_SSTAT_WR_CMPLT))
        {   /* Defio el tamaño del paquete */
            if (PACKET_SIZE == I2C_I2CSlaveGetWriteBufSize())
            {            }
            /* Limpio el estatus del slave y el buffer de escritura */
            I2C_I2CSlaveClearWriteBuf();
            (void) I2C_I2CSlaveClearWriteStatus();
        }

        /* Cuando la lectura por l maestro esta completa */
        if (0u != (I2C_I2CSlaveStatus() & I2C_I2C_SSTAT_RD_CMPLT))
        {
            /* Limpio le buffer esclaro y el estado de lectura*/
            I2C_I2CSlaveClearReadBuf();
            (void) I2C_I2CSlaveClearReadStatus();
        }
    
    for ( i=0; i<BUFFER_SIZE; i++) {
        i2cReadBuffer[i]=buffist[i]; }
    }
}

void sensor(void){
        Trigger_Write(1);
        CyDelayUs(10);    
        Trigger_Write(0);
        CyDelay(100); // Se esperan 100ms para qu eel sensor se prepare de nuevo
        ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
        valor=ADC_GetResult16(0);
        medida = max-distance;
        tanque = largo*((acos((radio-medida)/medida))*(radio*radio)-sqrt(2*(medida*radio)-(medida*medida))*(radio-medida))*0.001;
        prom=(100*(medida))/max;
        ;
}

void publicar (void){   
    LCD_ClearDisplay();

            LCD_ClearDisplay();

         if(distance <= max || distance >= 3){

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
  
     if(distance >= max || distance <= 3){
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