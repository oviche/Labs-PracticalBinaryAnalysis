# CHAPTER.5 SOLUTIONS
## LEVEL.1
- Just feed the payload challenge flag to oracle to start level.2 .
 ```
 ./oracle 84b34c124b2ba5ca224af8e33b077e9e 
 ```
## LEVEL.2
- By executing the **lvl2** many times it looks like printing random hex bytes strings.
```
./lvl2
74
./lvl2
f6
./lvl2
bf
./lvl2
36
```

- So i tried **ltrace** to see what functions called and it looks like **lvl2** binary generate random number. 
```
ltrace ./lvl2  # COMMAND LINE

# OUTPUT

__libc_start_main(0x400500, 1, 0x7ffd3e3702f8, 0x400640 <unfinished ...>
time(0)                                                                      = 1591885412
srand(0x5ee23e64, 0x7ffd3e3702f8, 0x7ffd3e370308, 0x400640)                  = 0
rand(0x7ff77e7af1d0, 0x7ff77e7af740, 0x7ff77e7af1c4, 0x7ff77e7af1d0)         = 0x62e8a649
puts("f8"f8
)                                                                   = 3
```
- Let us use **objdump** To look more close on assembly code of **.text** section. From disassembly , the generated number saved to eax then anded with 0xf then used as index in array that hold the addresses of printed hex byte strings.
the number of hex byte strings is 16 because index range from 0-15 because random number anded with 0xf. 
```
objdump -M intel -j .text -d  lvl2 # COMMAND LINE

# OUTPUT
0000000000400500 <.text>:
  400500:	48 83 ec 08          	sub    rsp,0x8
  400504:	31 ff                	xor    edi,edi 
  400506:	e8 c5 ff ff ff       	call   4004d0 <time@plt>
  40050b:	89 c7                	mov    edi,eax
  40050d:	e8 ae ff ff ff       	call   4004c0 <srand@plt>
  400512:	e8 c9 ff ff ff       	call   4004e0 <rand@plt>
  400517:	99                   	cdq    
  400518:	c1 ea 1c             	shr    edx,0x1c
  40051b:	01 d0                	add    eax,edx
  40051d:	83 e0 0f             	and    eax,0xf
  400520:	29 d0                	sub    eax,edx
  400522:	48 98                	cdqe   
  400524:	48 8b 3c c5 60 10 60 	mov    rdi,QWORD PTR [rax*8+0x601060]
  40052b:	00 
  40052c:	e8 6f ff ff ff       	call   4004a0 <puts@plt>
  400531:	31 c0                	xor    eax,eax
  400533:	48 83 c4 08          	add    rsp,0x8
  400537:	c3                   	ret    

```
- So let us breakpoint at address **0x400524** then get the base address of array to dump the 16 hex byte strings using gdb.
```
gdb$ b * 0x400524
Breakpoint 1 at 0x400524
gdb$ r   # run 
gdb$ x/gx $rax*0x0+0x601060
0x601060:	0x00000000004006c4 #Base address
gdb$ x/16s 0x00000000004006c4  # print the 16 hex byte strings
0x4006c4:	"03"
0x4006c7:	"4f"
0x4006ca:	"c4"
0x4006cd:	"f6"
0x4006d0:	"a5"
0x4006d3:	"36"
0x4006d6:	"f2"
0x4006d9:	"bf"
0x4006dc:	"74"
0x4006df:	"f8"
0x4006e2:	"d6"
0x4006e5:	"d3"
0x4006e8:	"81"
0x4006eb:	"6c"
0x4006ee:	"df"
0x4006f1:	"88"
```
- Concatinate the strings in order to get the flag = 034fc4f6a536f2bf74f8d6d3816cdf88 


## LEVEL.3


