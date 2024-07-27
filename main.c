#include <MKL25Z4.H>
#include <stdio.h>
#include <math.h>
//This is how you import a header file
#include "utils.h"

void Delay(volatile unsigned int time_del);
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_init(void);
void LCD_ready(void);
void keypad_init(void);
void print_fnc(unsigned char *data);
void clear_lcd(void);
uint32_t keypad_getkey(void);
void blink(void);
void delay(uint32_t cycles);
char * print_to_arr(char * buffer, float number, int precision);
int get_number_from_keypad();
int gcdExtended(int a, int b);
int exponentMod(int A, int B, int C);


int foo(int a, int b);
int main(void){
	int q, p, e, n, m;
	p = 59;
	q = 61;
	e = 2; 

	n = p*q;
	m = --p * --q;
	p++;
	q++;
		
	int x,y,d;
	while (gcdExtended(m, e) != 1 ){
		e++;
	}	
	d = e;
	while (((e*d) % m) != 1){
		d++;
	}	
	
	LCD_init();
	keypad_init();
	
	print_fnc("Select Mode:");
	LCD_command(0xC0); 
	print_fnc("A=>enc, D=>dec:");
	int D;
	D = get_number_from_keypad();
	clear_lcd();
	while ((D != 17) && (D != 20)){ 
		print_fnc("Wrong Key");
		LCD_command(0xC0);
		print_fnc("Enter A or D:");
		D = get_number_from_keypad();
	  clear_lcd();
	}	
	print_fnc("Enter Message:");
	LCD_command(0xC0);
	
	
	long MessageBlock, MessageBlock1, MessageBlock2;
	MessageBlock = get_number_from_keypad();
	MessageBlock2 = MessageBlock % 10000;
	MessageBlock1 = (MessageBlock - MessageBlock2) / 10000;
	clear_lcd();
	int message1;
	int message2;
	clear_lcd();
	
	if (D == 17){

		
		message1 = exponentMod(MessageBlock1, e, n);
		
		
		message2 = exponentMod(MessageBlock2, e, n);
		
		
		print_fnc("Encrypted:");
	
	}
	else if (D == 20){
		
	
		message1 = exponentMod(MessageBlock1, d, n);
		
		message2 = exponentMod(MessageBlock2, d, n);
		
		
			print_fnc("Decrypted:");
	}
		
			
			
		char strdec1[16], strdec2 [16]; 
		sprintf(strdec1, "%d", message1);
		sprintf(strdec2, "%d", message2);
		
	
		LCD_command(0xC0);
		print_fnc(strdec1);
		print_fnc(strdec2);
	
}	
