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

#define DH11_DATA_BIT   PORTCbits.RC5
#define DH11_DATA_TRIS  TRISCbits.TRISC5

#define TIMER0_PRESCALER 256
#define OVERFLOWS_PER_SECOND 76
#define TOTAL_SECONDS 60 // 60 seconds = 1 minute

uint8_t dht11_data[40];
uint8_t dht11_rh;
uint8_t dht11_temp;

volatile uint16_t seconds_counter = 0;
volatile uint8_t overflow_counter = 0;
volatile uint8_t measurement_flag = 0;

uint8_t dht11_measurement(void);
void start_measurement(void);
void timer0_init(void);
void display_countdown(void);

void interrupt isr(void)
{
    if (INTCONbits.TMR0IF) {
        // Clean the interrupt flag
        INTCONbits.TMR0IF = 0;
        // Adjust the reload value so that we get better accuracy
        TMR0 = 100;
        
        // Increment the overflow counter before checking
        overflow_counter++;
        if(overflow_counter >= OVERFLOWS_PER_SECOND) {
            overflow_counter = 0;
            seconds_counter++;
            
            if(seconds_counter >= TOTAL_SECONDS) {
                // Here it means that 1 minute has passed..

                // We set back the seconds counter to 0 to reset the counter
                seconds_counter = 0;

                // Set the measurement flag to be true so that a measurement happens
                measurement_flag = 1;
            }
        }
    }
}

void timer0_init(void)
{
    // Timer0 configuration for 20MHz frequency
    OPTION_REGbits.T0CS = 0;     // Internal clock
    OPTION_REGbits.PSA = 0;      // Prescaler assigned
    OPTION_REGbits.PS = 0b111;   // 1:256 prescaler
    TMR0 = 100;                  // Initial value
    INTCONbits.TMR0IF = 0;       // Clear interrupt flag
    INTCONbits.TMR0IE = 1;       // Enable Timer0 interrupt
}

void main(void)
{
    // Initialize lcd before starting
    LCDInit(LS_NONE);

    // Configure the TRIS for each port before starting
    // Red LED output (0)
    TRISAbits.TRISA0 = 0;

    // Green LED output (0)
    TRISCbits.TRISC6 = 0;

    // DHT11 data input (1)
    TRISCbits.TRISC5 = 1;
    
    // Call timer0_init to initialize Timer0
    timer0_init();
    
    // Enable interrupts
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;

    // Display the introduction screen
    __delay_ms(50);
    LCDWriteStringXY(0,0,"Microprocessor");
    LCDWriteStringXY(0,1,"Project MEDIPOL");
    __delay_ms(1000);
    
    LCDClear();
    LCDWriteStringXY(0,0,"Abdullah64210027");
    LCDWriteStringXY(0,1,"Zuhair 64230114");
    __delay_ms(1000);
    
    // Set the flag to true at the beginning to make initial measurement
    measurement_flag = 1;

    while(1)
    {
        // Inside a while loop, we do the measurement only when the measurement flag is set to true,
        // It is set to true only when the timer of 1 minute ends inside the isr function.
        if(measurement_flag) {
            measurement_flag = 0;
            start_measurement();
        }
        
        display_countdown();
        __delay_ms(200);
    }
}

void display_countdown(void)
{
    uint16_t remaining = TOTAL_SECONDS - seconds_counter;
    uint8_t minutes = remaining / 60;
    uint8_t seconds = remaining % 60;
    
    char time_str[16];
    sprintf(time_str, "Next in: %02d:%02d", minutes, seconds);
    
    LCDWriteStringXY(0,0,"Temp/Humidity");
    LCDWriteStringXY(0,1,time_str);
}

void start_measurement(void)
{
    LCDClear();
    
    if(!dht11_measurement()) {
        LCDClear();
        LCDWriteStringXY(0,0,"Measurement");
        LCDWriteStringXY(0,1,"Error!");
        __delay_ms(1000);
        return;
    }

    // Display results
    char temperature[16], humidity[16];
    sprintf(temperature, "Temp: %dC", dht11_temp);
    sprintf(humidity, "Humid: %d%%", dht11_rh);
    
    LCDClear();
    LCDWriteStringXY(0,0,temperature);
    LCDWriteStringXY(0,1,humidity);

    // LED control
    if(dht11_temp > 26) {
        // Turn of the Green led
        PORTCbits.RC6 = 0;

        // Turn on the Red led, with some delay to make it blink
        PORTAbits.RA0 = 1;
        __delay_ms(300);
        PORTAbits.RA0 = 0;
        __delay_ms(300);
        PORTAbits.RA0 = 1;
    } else {
        // Turn on the Green led and turn of the Red led
        PORTCbits.RC6 = 1;
        PORTAbits.RA0 = 0;
    }
    
    __delay_ms(1000);
    PORTCbits.RC6 = 0;
    PORTAbits.RA0 = 0;
}

uint8_t dht11_measurement()
{
    for(uint8_t i=0;i<40;i++) dht11_data[i]=0;
    
    DH11_DATA_BIT=0;
    DH11_DATA_TRIS=0;

    __delay_ms(20);
    
    DH11_DATA_TRIS=1;

    uint8_t counter=0;

    // Wait for falling edge
    while(DH11_DATA_BIT)
    {
        counter++;
        __delay_us(1);

        if(counter==80)
            return 0;
    }

    // Wait for rising edge
    while(!DH11_DATA_BIT)
    {
        counter++;
        __delay_us(1);

        if(counter==180)
            return 0;
    }

    // Wait for falling edge
    while(DH11_DATA_BIT)
    {
        counter++;
        __delay_us(1);

        if(counter==80)
            return 0;
    }


    for(uint8_t i=0;i<40;i++)
    {
        // Wait for rising edge
        while(!(PORTC & (1<<5)));
        

        // Setup Timer1
        T1CKPS0=1;  //  Prescaller = 1:2
        TMR1L=0x00;  // Init counter
        TMR1H=0x00;
        TMR1ON=1;   // Stat timer

        // Wait for falling edge
        while((PORTC & (1<<5)));

        TMR1ON=0;

        uint16_t time=TMR1L;
        time=time | (TMR1H<<8);

        time=time*2;
        
        if(time>55 && time <70)
        {
            // bit is 0
            dht11_data[i]=0;
        }
        else if(time>150)
        {
            // bit is 1
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
