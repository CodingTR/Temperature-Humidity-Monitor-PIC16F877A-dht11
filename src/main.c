/*****************************************************************
    = Istanbul Medipol University Spring 2025 Microprocessor Project =
    =      PIC16F877A Temperature and Humidity Sensor using DHT11    =
    =                        64210027 -- COE                         =
    =                        64230114 -- COE                         =
*****************************************************************/

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "pic16_lcd_library.h"

#pragma config FOSC = HS  
#pragma config WDTE = OFF 
#pragma config PWRTE = ON 
#pragma config BOREN = ON 
#pragma config LVP = OFF  
#pragma config CPD = OFF  
#pragma config WRT = OFF  
#pragma config CP = OFF   

#define DH11_DATA_BIT   PORTDbits.RD5
#define DH11_DATA_TRIS  TRISDbits.TRISD5

uint8_t dht11_data[40];

uint8_t dht11_rh;
uint8_t dht11_temp;

uint8_t dht11_measurement();
void start_measurement();
uint8_t button_click();

void main()
{
    LCDInit(LS_NONE);

    // For dht
    TRISCbits.TRISC2=0; 
    PORTCbits.RC2=1;

    // For led
    TRISAbits.TRISA0=0; 
    TRISCbits.TRISC5=0; 

    __delay_ms(50);

    LCDWriteStringXY(0,0,"Microprocessor");
    LCDWriteStringXY(0,1,"Project MEDIPOL");

    __delay_ms(500);
    LCDClear();

    LCDWriteStringXY(0,0,"Abdullah64210027");
    LCDWriteStringXY(0,1,"Zuhair 64230114");

    __delay_ms(500);
    LCDClear();

    while(1)
    {
        if(button_click()){
            start_measurement();
        }
        else{
            LCDClear();
            LCDWriteStringXY(0,0,"Click Button to");
            LCDWriteStringXY(0,1,"check..");
        }
        
        __delay_ms(500);
    }    
}

void start_measurement(){
    LCDClear();

    if(!dht11_measurement())
    {
        LCDClear();
        LCDWriteStringXY(0,0,"Error!");
        LCDWriteStringXY(0,1,"No Sensor Found");
    }
    else
    {
        char temperature[32];
        char humidity[32];

        sprintf(temperature, "Temp: %dC", dht11_temp);
        sprintf(humidity, "Humidity: %d", dht11_rh);        
        
        LCDWriteStringXY(0,0,temperature);
        LCDWriteStringXY(0,1,humidity);

        if((dht11_temp > 32)){
            // Turn Green led off
            PORTCbits.RC5=0;

            // Turn Red led on
            PORTAbits.RA0=1;
            __delay_ms(300);
            PORTAbits.RA0=0;
            __delay_ms(300);
            PORTAbits.RA0=1;
        }
        else{
            // Turn Green led on
            PORTCbits.RC5=1;

            // Turn Red led off
            PORTAbits.RA0=0;
            __delay_ms(300);
        }

        PORTCbits.RC5=0;
        PORTAbits.RA0=0;
    }

    __delay_ms(500);
}

uint8_t button_click()
{
    if(PORTDbits.RD6 == 1){
        // Return true if button clicked
        return 1;
    }
    else{
        PORTAbits.RA0=0;
        return 0;
    }
}


uint8_t dht11_measurement()
{
    for(uint8_t i=0;i<40;i++) dht11_data[i]=0;
    
    DH11_DATA_BIT=0;
    DH11_DATA_TRIS=0;

    __delay_ms(20);
    
    DH11_DATA_TRIS=1;

    uint8_t counter=0;

    //wait for falling edge
    while(DH11_DATA_BIT)
    {
        counter++;
        __delay_us(1);

        if(counter==80)
            return 0;
    }

    //wait for rising edge
    while(!DH11_DATA_BIT)
    {
        counter++;
        __delay_us(1);

        if(counter==180)
            return 0;
    }

    //wait for falling edge
    while(DH11_DATA_BIT)
    {
        counter++;
        __delay_us(1);

        if(counter==80)
            return 0;
    }


    for(uint8_t i=0;i<40;i++)
    {
        //wait for rising edge
        while(!(PORTD & (1<<5)));
        

        //Setup Timer1
        T1CKPS0=1;  //Prescaller = 1:2
	    TMR1L=0x00;  //Init counter
        TMR1H=0x00;
        TMR1ON=1;   //Stat timer

        //wait for falling edge
        while((PORTD & (1<<5)));

        TMR1ON=0;

        uint16_t time=TMR1L;
        time=time | (TMR1H<<8);

        time=time*2;
        
        if(time>55 && time <70)
        {
            //bit is 0
            dht11_data[i]=0;
        }
        else if(time>150)
        {
            //bit is 1
            dht11_data[i]=1;

        }  
    }

    dht11_rh=dht11_temp=0;

    for(uint8_t i=0;i<8;i++)
    {
        if(dht11_data[i]==1)
            dht11_rh|=(1<<(7-i));
    }

    for(uint8_t i=0;i<8;i++)
    {
        if(dht11_data[16+i]==1)
            dht11_temp|=(1<<(7-i));
    }
    
    return 1;
}