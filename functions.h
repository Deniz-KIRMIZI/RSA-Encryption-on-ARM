#include <MKL25Z4.H>
#include <stdio.h>

#define RS 0x04     /* PTA2 mask */ 
#define RW 0x10     /* PTA4 mask */ 
#define EN 0x20     /* PTA5 mask */

//Function Prototypes 
//Either use this or define the functions before main funciton
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

int get_number_from_keypad(){
		uint32_t key;
		int key_value;
		char lookup[]= {'1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'};
		key = 0;
		key_value = 0;
		while(lookup[key-1] != '#'){
			Delay(300000);	
			key=keypad_getkey();
				if((key != 0)&(lookup[key-1] != '#')){
					LCD_data(lookup[key-1]);
					key_value =  10*key_value + lookup[key-1] - '0';
					Delay(300000);
					key =0;
				}
		}
		return key_value;
}




void read_to_buffer(int* read_coeff, int len){
	char read_buffer[]= {'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'};
	int key;
	char lookup[]= {'1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'};
	int key_prev = 0;
	key = 1;
	int idx;
	idx = 0;
	
	while(keypad_getkey() == 0)//Polling, waiting to get an input
	{
	}
	while( (lookup[key-1] != '#')||(idx < len+1) ) // Take input until the # key is pressed or screen limit exceeded
	{
		key = 0;
		Delay(300000);
		while(key == 0)//Polling, waiting to get an input, don't read the same key every time.
		{
			key=keypad_getkey();
			if(key_prev == key) // Prevents reading the same key.
			{
				key = 0;
			}
			else
			{
				key_prev = key;
			}
		}
		if(key != 0)// There is an input
		{
			read_buffer[idx] = lookup[key-1];
			read_coeff[idx] = lookup[key-1]-'0';
			idx = idx + 1;
			clear_lcd();
			print_fnc(read_buffer);
		}
	}
	read_coeff[idx-1] = 0;
}

char * print_to_arr(char * buffer, float number, int precision){
	
	int significant = (int) number;
	int s = (int) number;
	number = number - (float) significant;
	for(int i =0; i < precision; i++){
			number = number*10;
	}
	int point = (int) number;
	
	int idx = 0;
	int q = 1;
	while(s > 0){
		s = s /10 ;
		idx = idx + 1;
		q = q*10;
	}
	for(int i = 0 ; i < idx ; i++){
		q = q/10;
		buffer[i] = (significant/q) + '0';
		significant =  significant-(significant/q)*q;
	}
	
	buffer[idx] = '.';
	for(int i = idx + precision ; idx < i ; i--){
		buffer[i] = (point - (point/10)*10) + '0';
		point = point /10 ;
	}
	return buffer;
}

//Turns on and off a LED connected to PORTC0
void blink(void){
    SIM->SCGC5 |= 0x0400;      /* enable clock to Port C */
    PORTB->PCR[18] = 0x100;    /* make PTB18 pin as GPIO */
	  PORTB->PCR[19] = 0x100;      /* make PTB19 pin as GPIO */
    PTB->PDDR |= (3UL << 18);    /* make PTB as output pins */
    while(1){
				
        PTB->PSOR = (3UL << 18);         /* Set PB18 */
        Delay(5000000);
        PTB->PCOR = (3UL << 18);				 /* Clear PTB18*/
			  Delay(5000000);
    }

}

void clear_lcd(void)
{	
	int i;
	LCD_command(0x80); //Start from the 1st line
	for(i = 16; i > 0; i--)
	{
			LCD_data(' '); //Clear the 1st line
	}
	LCD_command(0xC0); //Go to the 2nd line
	for(i = 16; i > 0; i--)
	{
			LCD_data(' '); //Clear the 2nd line
	}
	LCD_command(0x80);
}


void print_fnc(unsigned char *data)
{
	int i = 0 ;
	//Continue until a NULL char comes
	while(data[i] != 0x00)
	{
		LCD_data(data[i]);
		i++;
	}
}

void LCD_init(void)
{
    SIM->SCGC5 |= 0x1000;       /* enable clock to Port D */ 
    PORTD->PCR[0] = 0x100;      /* make PTD0 pin as GPIO */
    PORTD->PCR[1] = 0x100;      /* make PTD1 pin as GPIO */
    PORTD->PCR[2] = 0x100;      /* make PTD2 pin as GPIO */
    PORTD->PCR[3] = 0x100;      /* make PTD3 pin as GPIO */
    PORTD->PCR[4] = 0x100;      /* make PTD4 pin as GPIO */
    PORTD->PCR[5] = 0x100;      /* make PTD5 pin as GPIO */
    PORTD->PCR[6] = 0x100;      /* make PTD6 pin as GPIO */
    PORTD->PCR[7] = 0x100;      /* make PTD7 pin as GPIO */
    PTD->PDDR = 0xFF;           /* make PTD7-0 as output pins */
    
    SIM->SCGC5 |= 0x0200;       /* enable clock to Port A */ 
    PORTA->PCR[2] = 0x100;      /* make PTA2 pin as GPIO */
    PORTA->PCR[4] = 0x100;      /* make PTA4 pin as GPIO */
    PORTA->PCR[5] = 0x100;      /* make PTA5 pin as GPIO */
    PTA->PDDR |= 0x34;          /* make PTA5, 4, 2 as output pins */
    
    LCD_command(0x38);      /* set 8-bit data, 2-line, 5x7 font */
    LCD_command(0x01);      /* clear screen, move cursor to home */
    LCD_command(0x0F);      /* turn on display, cursor blinking */
}

/* This function waits until LCD controller is ready to
 * accept a new command/data before returns.
 */
void LCD_ready(void)
{
    uint32_t status;
    
    PTD->PDDR = 0x00;          /* PortD input */
    PTA->PCOR = RS;         /* RS = 0 for status */
    PTA->PSOR = RW;         /* R/W = 1, LCD output */
    
    do {    /* stay in the loop until it is not busy */
			  PTA->PCOR = EN;
			  Delay(500);
        PTA->PSOR = EN;     /* raise E */
        Delay(500);
        status = PTD->PDIR; /* read status register */
        PTA->PCOR = EN;
        Delay(500);			/* clear E */
    } while (status & 0x80UL);    /* check busy bit */
    
    PTA->PCOR = RW;         /* R/W = 0, LCD input */
    PTD->PDDR = 0xFF;       /* PortD output */
}

void LCD_command(unsigned char command)
{
    LCD_ready();			/* wait until LCD is ready */
    PTA->PCOR = RS | RW;    /* RS = 0, R/W = 0 */
    PTD->PDOR = command;
    PTA->PSOR = EN;         /* pulse E */
    Delay(500);
    PTA->PCOR = EN;
}

void LCD_data(unsigned char data)
{
    LCD_ready();			/* wait until LCD is ready */
    PTA->PSOR = RS;         /* RS = 1, R/W = 0 */
    PTA->PCOR = RW;
    PTD->PDOR = data;
    PTA->PSOR = EN;         /* pulse E */
    Delay(500);
    PTA->PCOR = EN;
}

/* Delay n milliseconds
 * The CPU core clock is set to MCGFLLCLK at 41.94 MHz in SystemInit().
 */

/* delay n microseconds
 * The CPU core clock is set to MCGFLLCLK at 41.94 MHz in SystemInit().
 */


void Delay(volatile unsigned int time_del) {
  while (time_del--) 
		{
  }
}


void keypad_init(void)
{
    SIM->SCGC5 |= 0x0800;       /* enable clock to Port C */ 
    PORTC->PCR[0] = 0x103;      /* make PTC0 pin as GPIO and enable pullup*/
    PORTC->PCR[1] = 0x103;      /* make PTC1 pin as GPIO and enable pullup*/
    PORTC->PCR[2] = 0x103;      /* make PTC2 pin as GPIO and enable pullup*/
    PORTC->PCR[3] = 0x103;      /* make PTC3 pin as GPIO and enable pullup*/
    PORTC->PCR[4] = 0x103;      /* make PTC4 pin as GPIO and enable pullup*/
    PORTC->PCR[5] = 0x103;      /* make PTC5 pin as GPIO and enable pullup*/
    PORTC->PCR[6] = 0x103;      /* make PTC6 pin as GPIO and enable pullup*/
    PORTC->PCR[7] = 0x103;      /* make PTC7 pin as GPIO and enable pullup*/
    PTC->PDDR = 0x00;         /* make PTC7-0 as input pins */
}




uint32_t keypad_getkey(void)
{
    uint32_t row, col;
    const char row_select[] = {0x01, 0x02, 0x04, 0x08}; /* one row is active */

    /* check to see any key pressed */
    PTC->PDDR |= 0x0F;          /* rows output */
    PTC->PCOR = 0x0F;               /* ground rows */
    Delay(500);                 /* wait for signal return */
    col =  PTC->PDIR & 0xF0UL;     /* read all columns */
    PTC->PDDR = 0;              /*  rows input */
    if (col == 0xF0UL)
        return 0;               /* no key pressed */

    /* If a key is pressed, it gets here to find out which key.
     * It activates one row at a time and read the input to see
     * which column is active. */
    for (row = 0; row < 4; row++)
    {
        PTC->PDDR = 0;                  /* disable all rows */
        PTC->PDDR |= row_select[row];   /* enable one row */
        PTC->PCOR = row_select[row];    /* drive the active row low */
        Delay(500);                     /* wait for signal to settle */
        col = PTC->PDIR & 0xF0UL;         /* read all columns */
        if (col != 0xF0UL) break;         /* if one of the input is low, some key is pressed. */
    }
    PTC->PDDR = 0;                      /* disable all rows */
    if (row == 4) 
        return 0;                       /* if we get here, no key is pressed */
 
    /* gets here when one of the rows has key pressed, check which column it is */
    if (col == 0xE0UL) return row * 4 + 1;    /* key in column 0 */
    if (col == 0xD0UL) return row * 4 + 2;    /* key in column 1 */
    if (col == 0xB0UL) return row * 4 + 3;    /* key in column 2 */
    if (col == 0x70UL) return row * 4 + 4;    /* key in column 3 */

    return 0;   /* just to be safe */
}



void parameters(int n, int m, int p, int q, int e);
int gcdExtended(int a, int b);
int exponentMod(int A, int B, int C);
void Encryption(int a, int m, int n, int b, int e);
void Decription(int a, int m, int n, int b, int e);
void intTOstring(int number, char *str);

void parameters(int n, int m, int p, int q, int e){
	p = 59;
	q = 61;
	e = 2;

	n = p*q;
	m = --p * --q;
	p++;
	q++;
}

int gcdExtended(int a, int b){
	if (a == 0)
	{
		return b;
	}
	return gcdExtended(b %a, a);
}

int exponentMod(int A, int B, int C){
  int res = 1;
  A = A % C; // Ensure b is within range 0 to m-1

  while (B > 0) {
    if (B & 1) { // Check if e is odd
      res = (res * A) % C;
    }
    A = (A * A) % C; // Square b for the next iteration
    B >>= 1; // Divide e by 2 (bitwise operation)
  }
	return res;
}
void Encryption(int a, int m, int n, int b, int e){
	a = m; 
	b = e;
	int x,y,d;
	while (gcdExtended(a, b) != 1 ){
		b++;
	}	
	e = b;
	d = e;
	while (((e*d) % m) != 1){
		d++;
	}	
	int MessageBlock1, EncryptedBlock1, A, B, C;
	A = MessageBlock1;
	B = e;
	C = n;
	EncryptedBlock1 = exponentMod(A, B, C);
}
void Decription(int a, int m, int n, int b, int e){
	a = m; 
	b = e;
	int x,y,d;
	while (gcdExtended(a, b) != 1 ){
		b++;
	}	
	e = b;
	d = e;
	while (((e*d) % m) != 1){
		d++;
	}	
	int EncryptedBlock1, DecryptedBlock1, A, B, C;
	A = EncryptedBlock1;
	B = d;
	C = n;
	DecryptedBlock1 = exponentMod(A, B, C);
}
void intTOstring(int number, char *str) {
    sprintf(str, "%08d", number);
}
/*
int main() {
    int number = 12345678; // Replace this with your 8-digit integer
    char str[9]; // 8 digits + 1 for null terminator

    intToString(number, str);

    printf("Integer: %d\nString: %s\n", number, str);

    return 0;
}
*/
