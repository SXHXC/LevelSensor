#include <project.h>
#include "math.h"
#include <stdio.h>

/* Declaramos las funciones que vamos a ejecutar */
void publicar(void); 
void I2C(void); 
void sensor(void);
void HMI(char obj[],char an[],char data[],char cou[]);
void procesar(void);


/* I2C slave address */
#define I2C_SLAVE_ADDR1  (0x08u)
#define I2C_SLAVE_ADDR2  (0x10u)

/* Tamaño del Buffer packet*/
#define BUFFER_SIZE     (22)
#define PACKET_SIZE     (BUFFER_SIZE)
uint8 i2cReadBuffer1[BUFFER_SIZE];
uint8 i2cReadBuffer2[BUFFER_SIZE];
float buffist[BUFFER_SIZE];

float distance=0;
float largo = 60.5,ancho = 26,max = 45.5; //medidas del tanque, max = altura
float tanque,prom,medida,temp;
char tanque2[6],prom2[6],medida2[6],temp2[6],tanque3[6],prom3[6],medida3[6],temp3[6]; //variables par almacenar el valor en litros del tanque, promedio de uso, y la meddida invertida del tanque
float m =0,c=0, valor; // m= distancia medida en uS, C= cte de velocidad del sonido respecto a la temperatura, Temp=pemperatura, Valore= resultado adc
uint8 i;
char8 dispbuff[BUFFER_SIZE];

 CY_ISR(Timer_ISR_handler)
{ 
    temp=(((5*valor)/4095)*100)-3; //temperatura °c
    c = (0.596*temp)+331.5; //velocidad del sonido
    c=(c * 100 / 1000000); // conversion a cm/us
    Timer_ClearInterrupt(Timer_INTR_MASK_CC_MATCH); // Limpiamos la interrupción
    m= Timer_ReadCounter();// datasheet del modulo HC SR04 indica que el tiempo en ir y volver de la señal es de 58us para 1cm  
    distance =m/2*c;// Como la conversion anterior de c calcula solo el tiempo en llegar la señal al objeto en el cual rebotara, esta se multiplica por dos para tener en cuenta amos espacio de tiempo
       prom=(100*(max-distance))/max;
    }

int main( void ) 
{   
    /* Iniciamos los modulos*/
    ADC_Start();
    ADC_StartConvert();
    I2CM_Start();
	Timer_Start();  // Iinicia el timer del sensor de ultrasonido
	LCD_Start();	      // Start LCD 
    LCD_DisplayOn();
    UART_Start();
    LCD_LoadCustomFonts(LCD_customFonts); 
    CyGlobalIntEnable; 	// Uncomment this line to enable global interrupts.
    ISR_StartEx(Timer_ISR_handler);

    
	for(;;) 
    {   
        sensor();
        publicar();
        procesar();
        CyDelay(500);
        I2C();
        
        sprintf(dispbuff, "%.2f", tanque);
        HMI("t0.txt=","M= ",dispbuff,"lt");
        sprintf(dispbuff, "%.2f", temp);
        HMI("t1.txt=","S1=", dispbuff,"lt");
        sprintf(dispbuff, "%.f", prom);
        HMI("j0.val=",dispbuff,NULL,NULL);
        
        sprintf(dispbuff, "%.2f", distance);
        HMI("t2.txt=","M= ",tanque2,"lt");
        sprintf(dispbuff, "%.6s", i2cReadBuffer1);
        HMI("t3.txt=","S1=", temp2,"lt");
        sprintf(dispbuff, "%.6s", i2cReadBuffer2);
        HMI("j1.val=",prom2,NULL,NULL);
        
        sprintf(dispbuff, "%.2f", temp);
        HMI("t4.txt=","M= ",tanque3,"c");
        sprintf(dispbuff, "%.2f", temp);
        HMI("t5.txt=","S1=", temp3,"c");
        sprintf(dispbuff, "%.2f", temp);
        HMI("j2.val=",prom3,NULL,NULL);

    }
	
}
void sensor(void){
        Trigger_Write(1);
        CyDelayUs(10);    
        Trigger_Write(0);

        ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
        valor=ADC_GetResult16(0);
        medida = max -distance;
        tanque=(medida*largo*ancho)*0.001;
        prom=(100*(max-distance))/max;
}

void publicar (void)
    {   
        
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


void HMI (char obj[],char an[],char data[],char cou[]){
        UART_UartPutChar(0xFF);
        UART_UartPutChar(0xFF);
        UART_UartPutChar(0xFF);
        UART_UartPutString(obj);
        UART_UartPutChar(0x22);
        UART_UartPutString(an);
        UART_UartPutString(data);
        UART_UartPutString(cou);
        UART_UartPutChar(0x22);
        UART_UartPutChar(0xFF);
        UART_UartPutChar(0xFF);
        UART_UartPutChar(0xFF);
}
void procesar (){
    int a=0;
    for (i=0;i<BUFFER_SIZE;i++){
        if (i2cReadBuffer1[i]== 0x2C){
            a++;    
        }
        else{
            switch(a){
            case 0 :
                medida2[i]=i2cReadBuffer1[i];
                break;
                
            case 1 :
                temp2[i]=i2cReadBuffer1[i];
                break;
                
            case 2 :
                tanque2[i]=i2cReadBuffer1[i];
                break;
                
            case 3 :
                if (i2cReadBuffer1[i]== 0x2E){
                    a=6;
                    break;
                }
                prom2[i]=i2cReadBuffer1[i];
                a=0;
                break;
                
            default :
                a=0;
                ;
            }
            
        }
    }
        for (i=0;i<BUFFER_SIZE;i++){
        if (i2cReadBuffer2[i]== 0x2C){
            a++;    
        }
        else{
            switch(a){
            case 0 :
                medida3[i]=i2cReadBuffer2[i];
                break;
                
            case 1 :
                temp3[i]=i2cReadBuffer2[i];
                break;
                
            case 2 :
                tanque3[i]=i2cReadBuffer2[i];
                break;
                
            case 3 :
                prom3[i]=i2cReadBuffer2[i];
                a=0;
                break;
                
            default :
                a=0;
                ;
            }
            
        }
    }
    
}