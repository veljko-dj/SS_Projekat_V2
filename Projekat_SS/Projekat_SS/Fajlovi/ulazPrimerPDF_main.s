# file: main.s
.global myStart
.global myCounter
.section myCode
.equ tim_cfg, 0xFF10
myStart:
ldr r0, $0x1
str r0, tim_cfg
wait:
ldr r0, myCounter
cmp r0, 5
jne wait
halt
.section myData
myCounter:
.word 0
.end