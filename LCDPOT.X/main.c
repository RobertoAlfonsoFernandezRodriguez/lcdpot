/*
 * File:   main.c
 * Author: Roberto Fernandez
 *
 * Created on 17 de marzo de 2024, 11:39 AM
 */


#include <xc.h>
#include "config.h"
#include"LCD-XC4.h"
#define _XTAL_FREQ 20000000
char* num[10]={"0","1","2","3","4","5","6","7","8","9"};
char* numchar[]= {"0","0","0","0"};
int valorpot= 54;
int n;
char* gato ={"kira"};
//gato =numchar;
void tochar(int);
void ADC_init(void); // Inicializar los modulos ADC
int ADC_read(int);

void main(void) {
    ADC_init();
    mOPEN_LCD;
    mLCD_HOME;
    while(1)
    {
    mLCD_CLEAR;
    numchar[0]="0";
    numchar[1]="0";
    numchar[2]="0";
    numchar[3]="0";
    valorpot = ADC_read(0);
    tochar(valorpot);
    text_display("POT: ");
    text_display(numchar[0]);
    text_display(numchar[1]);
    text_display(numchar[2]);
    text_display(numchar[3]);
    //mCURSOR_LINE2;
    //mCURSOR_FWD;
    //mCURSOR_BACK;
    //text_display(gato);
    __delay_ms(1000);
    }
    
    return;
}

void tochar(int digito)
{ do
    if(digito >= 1000)
    {
        numchar[0]="1";
        digito =digito%1000;
        
        
    }
    else if(digito>=100)
    {
        numchar[1] =num[digito/100];
        digito =digito%100;
    }
    else if(digito>=10)
    {
        numchar[2] =num[digito/10];
        digito =digito%10;
    }
    else if(digito>=1)
    {
        numchar[3] =num[digito];
        digito =digito%1;
    }
while(digito!=0);
        
}

void ADC_init(void){
    TRISA = 0xFF; // Todos son entradas
    ADCON1 = 0x0D; // VSS, VDD Y dos entradas analógicas A0 y A1
    ADCON2 = 0xB5; // justificado a la derecha con 16 TAD, y FOSC/16
    ADRESH = 0; // Limpiamos el registro
    ADRESL = 0; // Limpiamos el registro
}

int ADC_read(int chanel){
    int digital = 0;
   
    // canal 0 es seleccionado 0000 y ADC está apagado
    ADCON0 = (ADCON0 & 0b11000011)|((chanel << 2) & 0b00111100);
   
    // habilitar el ADC y empezar la conversión
    ADCON0 |=((1<<ADON)| (1 << GO));
   
    //Esperar para el término de la conversión
    while(ADCON0bits.GO_nDONE==1);
   
    // Combinamos los 8 bits LSB y los 2 bits MSB
    digital = (ADRESH*256)|(ADRESL);
   
    return digital;
}


