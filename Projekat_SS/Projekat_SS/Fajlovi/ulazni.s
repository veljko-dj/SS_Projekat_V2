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