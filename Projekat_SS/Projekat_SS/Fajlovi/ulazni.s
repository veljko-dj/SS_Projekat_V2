.section sek1
.global g1
.global g 
.extern e1	, e2
.global g2, g3  
.extern e	 
.bss
lab0:  #komentar
.word i 				#lc+=2
	ret	 				#lc+=1 ->sek1.size=3;
lab1:
.section sek2			#lc=3;
.word ha, h2			#lc=7;
.skip 3					#lc=10;

iret					#lc=11; ->sek2.size=11    
							#OVO NE VALJA, STAVLJAS 11 a realno je 8
							# ALI to i radi tvoj program, stavi size=8 lepo
.section sek3			#

.equ sim, 31

.skip 3
	int  r0 
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
	int *[r0+123]
#	int *[r0+asd]
	shr r0, b
	int $0x12 #PROVERI OVO  ! ! ! 
	shr r0, *[r0+4.1]
	
	
	halt  				#lc =11; sekc.size=4;
	
.end