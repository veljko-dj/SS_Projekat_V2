.section sek1  
.extern e	 
.bss
lab0:  #komentar
.section sek2 
.word i 		#2			 
	  
lab1: 		 
.skip 3			#3			 
 				 
		 
							
h1:
j1:
.word 0x5678	#2 			
							
g:	
i:						#7			
			
.section instr	
hallt:	halt 	#1					#00
iret			#1					#20
ret				#1					#40

.global g

prekid:					#3
int r9			#2					#10 9F 
 
poz_potp:				#5
call *r3    	#3					#30 F3 01
call *[r4]		#3					#30 F4 02
call *[r4 + 0x8]#5					#30 F4 03 00 08
call *[r4 + prekid] #5				#30 F4 03 00 03 (OFFSET(prekid))
call *prekid	#5					#30 F0 04 00 03 (OFFSET(prekid))
call *0x6677	#5					#30 F0 04 66 77
call %prekid	#5					#30 F7 03 00 01 (OFFSET(prekid)-2)
call poz_potp	#5					#30 F0 00 00 05
call 322		#5					#30 F0 00 01 42

operanda2:				#46	

xchg r7, r3		#2					#60 73
shl r3, r5		#2					#90 35

ldrrr:					#50
ldr r6, ldrrr	#5					#A0 60 04 00 32 (0x32==50)

not r3, r6		#2					#80 36
#not r3, prekid
			 		
test r0, r0		#2					#84 00			

str r7, $prekid #5					#B0 70 00 00 03 (OFFSET(prekid))
str r7, $3223	#5 					#B0 70 00 0C 97 
str r7, prekid	#5					#BO 70 04 00 03
str r7, 222		#5					#BO 70 04 00 DE
str r5, %prekid	#5					#BO 57 03 00 03
#str r7, r9
str r7, [r6]	#3					#BO 76 02 
str r7, [r6+31]	#5					#BO 76 03 00 1f
str r7, [r6+ prekid]#5				#BO 76 03 00 03


#str r7, 222 12
#str r7, 222 as
#str r7, %prekid as

#str r7, [r6]3
#str r7, [r6]s3
#str r7, [r6]+3
#str r7, [r6+ 1prekid]
#str r7, [r6 sa]
#str r7, *[r4]


.word g, 4386 	#4					#00 07 22 11 little endian
.word 4660, j1 	#4					#34 12 05 00 little endian

.equ sim, 31

.skip 3			#3					#00 00 00
.word 34969		#2					#99 88
.word 26231		#2					#77 66
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