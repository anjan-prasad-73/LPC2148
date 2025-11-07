#include <LPC213x.h>

#define RS (1<<8)
#define EN (1<<9)

void delay(unsigned int x){
    unsigned int i,j;
    for(i=0;i<x;i++)
        for(j=0;j<600;j++);
}

/*************** LCD FUNCTIONS ***************/
void LCD_Command(unsigned char cmd){
    IO0CLR = RS;
    IO0CLR = (0xF<<10);
    IO0SET = ((cmd & 0xF0) << 6);
    IO0SET = EN; delay(1); IO0CLR = EN;

    IO0CLR = (0xF<<10);
    IO0SET = ((cmd & 0x0F) << 10);
    IO0SET = EN; delay(1); IO0CLR = EN;
    delay(2);
}

void LCD_Data(unsigned char data){
    IO0SET = RS;
    IO0CLR = (0xF<<10);
    IO0SET = ((data & 0xF0) << 6);
    IO0SET = EN; delay(1); IO0CLR = EN;

    IO0CLR = (0xF<<10);
    IO0SET = ((data & 0x0F) << 10);
    IO0SET = EN; delay(1); IO0CLR = EN;
    delay(2);
}

void LCD_String(const char *s){
    while(*s) LCD_Data(*s++);
}

void LCD_Init(){
    IO0DIR |= RS | EN | (0xF<<10);
    delay(20);
    LCD_Command(0x33);
    LCD_Command(0x32);
    LCD_Command(0x28);
    LCD_Command(0x0C);
    LCD_Command(0x06);
    LCD_Command(0x01);
    delay(10);
}

/*************** 4x3 KEYPAD ***************/
char keymap[4][3] = {
 {'1','2','3'},
 {'4','5','6'},
 {'7','8','9'},
 {'*','0','#'}
};

char Keypad_GetKey(void){
    int row, col;

    for(row = 0; row < 4; row++){
        IO0SET = 0x0F;
        IO0CLR = (1<<row);
        delay(1);

        for(col = 0; col < 3; col++){
            if(!(IO0PIN & (1<<(4+col)))){
                delay(200);
                return keymap[row][col];
            }
        }
    }
    return 0;
}

/*************** MAIN ***************/
int main(){

    PINSEL0 = 0x00000000;
    PINSEL1 = 0x00000000;

    LCD_Init();
    LCD_Command(0x80);
    LCD_String("4x3 Keypad Demo");

    IO0DIR |= 0x0F;      // Rows output
    IO0DIR &= ~(0x70);   // Cols input
    IO0SET |= 0x70;      // INTERNAL PULL-UPS ENABLED ?

    while(1){
        char key = Keypad_GetKey();
        if(key){
            LCD_Command(0xC0);
            LCD_Data(key);
        }
    }
}
