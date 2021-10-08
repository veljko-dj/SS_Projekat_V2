.global a_
.global _Abc09
.extern _b
.section text
.equ _Abc09, 0x9B

int psw
shr r2, r5

jgt %g

ldr r5, [  r5  + 0x2A]

push r5

str r2, $g

a_:.skip 0x14
c: .word 10

.word _Abc09

k:.word a_
.section data
.word 0x02, 3, 0x40
g: .word 4
.skip 2

push r3
	jeq *r2
	jeq 5
	jne *[r3]
	jne *5
	jgt *r6
	jgt *[r4 + _Abc09]
xchg r6                ,r3
pop r6

xor r4, r5

shr r3, r2

add r5  ,               r3
	
halt

int           r6

iret

ret

.word _b, a_

#$mov r4, r5




.end



