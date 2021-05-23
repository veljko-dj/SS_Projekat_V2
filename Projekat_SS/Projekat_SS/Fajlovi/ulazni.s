.section sek1 
.global g  
.extern e	 
.bss
lab0:  #komentar
.section sek2 
.word i 				 
	  
lab1: 		 
.skip 3					 
 				 
							 
							
h1:
j1:
.word 3333						
							
g:	
i:						
			
.section instr	#ukupno 23		
hallt:	halt 	#1
iret			#1
ret				#1

prekid:
int r9			#2

poz_potp:
call *r3    	#3
call *[r4]		#3
call *[r4 + 3]   #5
call *[r4 + prekid]   #5
call *prekid
call *26231
call %prekid
call prekid
call 322

operanda2:

xchg r7, r3
shl r3, r5

ldrrr:
ldr r6, ldrrr

not r3, r6
#not r3, prekid
			 		
test r0, r0					

str r7, $prekid
str r7, $3223
str r7, prekid
str r7, 222
str r7, %prekid
#str r7, r9
str r7, [r6]
str r7, [r6+31]
str r7, [r6+ prekid]


#str r7, 222 12
#str r7, 222 as
#str r7, %prekid as

#str r7, [r6]3
#str r7, [r6]s3
#str r7, [r6]+3
#str r7, [r6+ 1prekid]
#str r7, [r6 sa]
#str r7, *[r4]


.word g, 4386 	#4
.word 4660, j1 	#4

.equ sim, 31

.skip 3			#3
.word 34969		#2
.word 26231		#2
#	int  r0 
#	int 	   $sd3  # Ne moze $ sd3 , tj. razmak
#	int $33.3
#	int $as3
#	int as3
#	int 1231s
#	int 1231
#	int [r0] 
#	int [r0+sii]
#	int [r0+12.3]
#	int %asd
#	int r0
#	int *r0
#	int *[r0]
#	int *[r0+123]
#	int *[r0+asd]
#	shr r0, b
#	int $0x12 #PROVERI OVO  ! ! ! 
#	shr r0, *[r0+4.1]
	
	
#	halt  				#lc =11; sekc.size=4;
	
.end