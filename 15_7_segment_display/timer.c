#include <LPC214x.h>

void delay(unsigned int t){
    unsigned int i,j;
    for(i=0;i<t;i++)
        for(j=0;j<6000;j++);
}

unsigned char seg_code[10] = { 
    0x3F, //0
    0x06, //1
    0x5B, //2
    0x4F, //3
    0x66, //4
    0x6D, //5
    0x7D, //6
    0x07, //7
    0x7F, //8
    0x6F  //9
};

int main(){
    PINSEL0 = 0x00000000;   // GPIO
    PINSEL1 = 0x00000000;
    
    IO0DIR |= (0x7F << 16); // P0.16–P0.22 as output

    while(1){
        for(int i=0;i<10;i++){
            IO0CLR = (0x7F << 16);      // clear previous segments
            IO0SET = (seg_code[i] << 16); // output new value
            delay(500);
        }
    }
}
