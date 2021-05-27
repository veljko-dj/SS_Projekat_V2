.section blaBla

.equ x1, 0x34f1

.skip 10
.word g1
.word e
g3:

.global g1, g3, g_4		# ovo je kriticna pozicija za global
						# Imas g1 i iznad i ispod
.extern e

.word e
g1:
g_4:

.word g1

not r4

.section novaS


halt

int r0

.section blaBla
add r4,r6


.end