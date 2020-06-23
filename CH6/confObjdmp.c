#include "stdio.h"


int main(){

char arr[13];
asm __volatile__(".byte 0xeb,0x1,0xc7 ");
arr[0]='H';
asm __volatile__(".byte 0xeb,0x03,0xc7,0xc7,0x45 ");
arr[1]='E';
arr[2]='L';
arr[3]='L';
asm __volatile__(".byte 0xeb,0x02,0xc7,0x45 ");
arr[4]='0';
arr[5]=' ';
arr[6]='W';
asm __volatile__(".byte 0xeb,0x02,0xc7,0x45 ");
arr[7]='O';
arr[8]='R';
asm __volatile__(".byte 0xeb,0x02,0xc7,0x45");
arr[9]='L';
arr[10]='D';
arr[11]='!';
asm __volatile__(".byte 0xeb,0x02,0xc7,0x45 ");
arr[12]='\n';

for(int i=0;i<13;i++){
asm __volatile__(".byte 0xeb,0x2,0xc7,0x45;mov %%dil , %0;"
                :
                :"r"(arr[i])
                :"rdi"
                );

asm __volatile__("call %0;"
                :
                :"r"(&putchar)
                :
                );
}


return 0;
}
