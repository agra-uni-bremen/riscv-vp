andi s4, s4, 255
lui zero, 262148
slli zero, zero, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi s4, s4, 64
#scddc_change

sc.ddc ca7, (s4)
andi s3, s3, 255
lui zero, 262148
slli zero, zero, 1
add s3, zero, s3

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi s3, s3, 64
#scddc_change

sc.ddc csp, (s3)
andi sp, sp, 255
lui zero, 262148
slli zero, zero, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi sp, sp, 64
#scddc_change

sc.ddc csp, (sp)
andi tp, tp, 255
lui a7, 262148
slli a7, a7, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi tp, tp, 64
#scddc_change

sc.ddc cnull, (tp)
andi a6, a6, 255
lui ra, 262148
slli ra, ra, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi a6, a6, 64
#scddc_change

sc.ddc cra, (a6)
andi a6, a6, 255
lui zero, 262148
slli zero, zero, 1

#sccap_change
auipcc ca6, 0
li t5, 32000
csetaddr ca6, ca6, t5
#sccap_change

sc.cap cgp, (ca6)
andi a7, a7, 255
lui ra, 262148
slli ra, ra, 1
add a7, ra, a7

#sccap_change
auipcc ca7, 0
li t5, 32000
csetaddr ca7, ca7, t5
#sccap_change

sc.cap cs3, (ca7)
cspecialrw csp, mscratchc, cgp

#jalr.cap_change
auipcc cnull, 0
cincoffsetimm cnull, cnull, 12
#jalr.cap_change

jalr.cap csp, cnull
cspecialrw cs4, mscratchc, cs3
cspecialrw ctp, mscratchc, cs2
andi zero, zero, 255
lui a7, 262148
slli a7, a7, 1

#sccap_change
auipcc cnull, 0
li t5, 32000
csetaddr cnull, cnull, t5
#sccap_change

sc.cap ca6, (cnull)
lui s3, 262148
slli s3, s3, 1
csetaddr cs3, cs3, s3
ccleartag cs3, cgp

#sc_change
auipcc cs3, 0
li t5, 32000
csetaddr cs3, cs3, t5
#sc_change

sc cgp, 0(cs3)
cincoffset cs3, cs3, 16

#sc_change
auipcc cs3, 0
li t5, 32000
csetaddr cs3, cs3, t5
#sc_change

sc cgp, 0(cs3)
cincoffset cs3, cs3, 16

#sc_change
auipcc cs3, 0
li t5, 32000
csetaddr cs3, cs3, t5
#sc_change

sc cs3, 0(cs3)
cincoffset cs3, cs3, 16

#sc_change
auipcc cs3, 0
li t5, 32000
csetaddr cs3, cs3, t5
#sc_change

sc cs3, 0(cs3)
cincoffset cs3, cs3, 16

#sc_change
auipcc cs3, 0
li t5, 32000
csetaddr cs3, cs3, t5
#sc_change

sc cgp, 0(cs3)
cincoffset cs3, cs3, 16
cincoffset cs3, cs3, -80
cloadtags s3, (cs3)
cspecialrw ctp, pcc, cs4
andi a7, a7, 255
lui s4, 262148
slli s4, s4, 1
add a7, s4, a7

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi a7, a7, 64
#scddc_change

sc.ddc csp, (a7)
andi tp, tp, 255
lui a6, 262148
slli a6, a6, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi tp, tp, 64
#scddc_change

sc.ddc csp, (tp)
andi s2, s2, 255
lui ra, 262148
slli ra, ra, 1

#sccap_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
#sccap_change

sc.cap cs4, (cs2)
cspecialr cs9, pcc
li s10, 1
csetflags cs9, cs9, s10
cspecialw mtcc, cs9

#jalr.cap_change
auipcc cs9, 0
cincoffsetimm cs9, cs9, 12
#jalr.cap_change

jr.cap cs9
andi s2, s2, 255
lui s3, 262148
slli s3, s3, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi s2, s2, 64
#scddc_change

sc.ddc ca7, (s2)
andi ra, ra, 255
lui s2, 262148
slli s2, s2, 1
add ra, s2, ra

#sccap_change
auipcc cra, 0
li t5, 32000
csetaddr cra, cra, t5
#sccap_change

sc.cap cs2, (cra)
andi sp, sp, 255
lui s4, 262148
slli s4, s4, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi sp, sp, 64
#scddc_change

sc.ddc cs3, (sp)
andi gp, gp, 255
lui zero, 262148
slli zero, zero, 1
add gp, zero, gp

#sccap_change
auipcc cgp, 0
li t5, 32000
csetaddr cgp, cgp, t5
#sccap_change

sc.cap cs3, (cgp)
andi s2, s2, 255
lui s4, 262148
slli s4, s4, 1

#sccap_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
#sccap_change

sc.cap cra, (cs2)
andi gp, gp, 255
lui gp, 262148
slli gp, gp, 1
add gp, gp, gp

#sccap_change
auipcc cgp, 0
li t5, 32000
csetaddr cgp, cgp, t5
#sccap_change

sc.cap cnull, (cgp)
cspecialr cs3, pcc
li s2, 0
slli s2, s2, 11
mv s2, s2
slli s2, s2, 11
mv s2, s2
slli s2, s2, 11
mv s2, s2
slli s2, s2, 11
addi s2, s2, 7
slli s2, s2, 8
addi s2, s2, 37
cincoffset cs3, cs3, s2
li s2, 0
slli s2, s2, 11
mv s2, s2
slli s2, s2, 11
mv s2, s2
slli s2, s2, 11
addi s2, s2, 1
slli s2, s2, 11
addi s2, s2, 162
slli s2, s2, 8
addi s2, s2, 24
csetbounds cs3, cs3, s2

#jalr.cap_change
auipcc cs3, 0
cincoffsetimm cs3, cs3, 12
#jalr.cap_change

jalr.cap cs3, cs3
andi s3, s3, 255
lui sp, 262148
slli sp, sp, 1
add s3, sp, s3

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi s3, s3, 64
#scddc_change

sc.ddc cra, (s3)
andi s4, s4, 255
lui zero, 262148
slli zero, zero, 1

#sccap_change
auipcc cs4, 0
li t5, 32000
csetaddr cs4, cs4, t5
#sccap_change

sc.cap cnull, (cs4)
andi a6, a6, 255
lui tp, 262148
slli tp, tp, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi a6, a6, 64
#scddc_change

sc.ddc cgp, (a6)
andi gp, gp, 255
lui s4, 262148
slli s4, s4, 1
add gp, s4, gp

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi gp, gp, 64
#scddc_change

sc.ddc cs3, (gp)
andi a6, a6, 255
lui a6, 262148
slli a6, a6, 1

#sccap_change
auipcc ca6, 0
li t5, 32000
csetaddr ca6, ca6, t5
#sccap_change

sc.cap ca6, (ca6)
andi ra, ra, 255
lui zero, 262148
slli zero, zero, 1
add ra, zero, ra

#sccap_change
auipcc cra, 0
li t5, 32000
csetaddr cra, cra, t5
#sccap_change

sc.cap ca6, (cra)
cspecialrw cra, pcc, cs4
crrl s2, s4
li s2, -3
candperm cgp, cgp, s2
cseal ctp, ctp, ctp

#cinvoke_change
auipcc ctp, 0
cincoffsetimm ctp, ctp, 12
#cinvoke_change

cinvoke ctp, cgp
cmove ct6, cra
andi a6, a6, 255
lui a7, 262148
slli a7, a7, 1

#sccap_change
auipcc ca6, 0
li t5, 32000
csetaddr ca6, ca6, t5
#sccap_change

sc.cap cgp, (ca6)
andi s3, s3, 255
lui s3, 262148
slli s3, s3, 1
add s3, s3, s3

#sccap_change
auipcc cs3, 0
li t5, 32000
csetaddr cs3, cs3, t5
#sccap_change

sc.cap ca7, (cs3)
andi s3, s3, 255
lui s3, 262148
slli s3, s3, 1
add s3, s3, s3

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi s3, s3, 64
#scddc_change

sc.ddc ca7, (s3)
andi s3, s3, 255
lui zero, 262148
slli zero, zero, 1
add s3, zero, s3

#sccap_change
auipcc cs3, 0
li t5, 32000
csetaddr cs3, cs3, t5
#sccap_change

sc.cap cs4, (cs3)
andi a7, a7, 255
lui ra, 262148
slli ra, ra, 1
add a7, ra, a7

#sccap_change
auipcc ca7, 0
li t5, 32000
csetaddr ca7, ca7, t5
#sccap_change

sc.cap cra, (ca7)

#cinvoke_change
auipcc cs3, 0
cincoffsetimm cs3, cs3, 12
#cinvoke_change

cinvoke cs3, ca7
andi s4, s4, 255
lui s2, 262148
slli s2, s2, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi s4, s4, 64
#scddc_change

sc.ddc cs4, (s4)
andi zero, zero, 255
lui gp, 262148
slli gp, gp, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi zero, zero, 64
#scddc_change

sc.ddc cs2, (zero)
andi a6, a6, 255
lui s3, 262148
slli s3, s3, 1

#sccap_change
auipcc ca6, 0
li t5, 32000
csetaddr ca6, ca6, t5
#sccap_change

sc.cap cs2, (ca6)
andi a7, a7, 255
lui ra, 262148
slli ra, ra, 1
add a7, ra, a7

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi a7, a7, 64
#scddc_change

sc.ddc cs3, (a7)
andi a7, a7, 255
lui s3, 262148
slli s3, s3, 1
add a7, s3, a7

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi a7, a7, 64
#scddc_change

sc.ddc csp, (a7)
andi a7, a7, 255
lui s4, 262148
slli s4, s4, 1
add a7, s4, a7

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi a7, a7, 64
#scddc_change

sc.ddc ca6, (a7)
andi zero, zero, 255
lui a7, 262148
slli a7, a7, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi zero, zero, 64
#scddc_change

sc.ddc cra, (zero)
andi s3, s3, 255
lui sp, 262148
slli sp, sp, 1
add s3, sp, s3

#sccap_change
auipcc cs3, 0
li t5, 32000
csetaddr cs3, cs3, t5
#sccap_change

sc.cap cs3, (cs3)
andi ra, ra, 255
lui a6, 262148
slli a6, a6, 1
add ra, a6, ra

#sccap_change
auipcc cra, 0
li t5, 32000
csetaddr cra, cra, t5
#sccap_change

sc.cap cgp, (cra)
andi s3, s3, 255
lui zero, 262148
slli zero, zero, 1
add s3, zero, s3

#sccap_change
auipcc cs3, 0
li t5, 32000
csetaddr cs3, cs3, t5
#sccap_change

sc.cap cs3, (cs3)
andi gp, gp, 255
lui zero, 262148
slli zero, zero, 1
add gp, zero, gp

#sccap_change
auipcc cgp, 0
li t5, 32000
csetaddr cgp, cgp, t5
#sccap_change

sc.cap ca6, (cgp)
andi zero, zero, 255
lui gp, 262148
slli gp, gp, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi zero, zero, 64
#scddc_change

sc.ddc ca7, (zero)
andi s4, s4, 255
lui tp, 262148
slli tp, tp, 1

#sccap_change
auipcc cs4, 0
li t5, 32000
csetaddr cs4, cs4, t5
#sccap_change

sc.cap ctp, (cs4)
andi sp, sp, 255
lui gp, 262148
slli gp, gp, 1

#sccap_change
auipcc csp, 0
li t5, 32000
csetaddr csp, csp, t5
#sccap_change

sc.cap ca7, (csp)
andi a6, a6, 255
lui zero, 262148
slli zero, zero, 1

#sccap_change
auipcc ca6, 0
li t5, 32000
csetaddr ca6, ca6, t5
#sccap_change

sc.cap csp, (ca6)
andi gp, gp, 255
lui ra, 262148
slli ra, ra, 1
add gp, ra, gp

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi gp, gp, 64
#scddc_change

sc.ddc cgp, (gp)
andi a6, a6, 255
lui zero, 262148
slli zero, zero, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi a6, a6, 64
#scddc_change

sc.ddc cs4, (a6)
andi tp, tp, 255
lui ra, 262148
slli ra, ra, 1

#sccap_change
auipcc ctp, 0
li t5, 32000
csetaddr ctp, ctp, t5
#sccap_change

sc.cap csp, (ctp)
andi s4, s4, 255
lui sp, 262148
slli sp, sp, 1

#sccap_change
auipcc cs4, 0
li t5, 32000
csetaddr cs4, cs4, t5
#sccap_change

sc.cap cs3, (cs4)
andi zero, zero, 255
lui a7, 262148
slli a7, a7, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi zero, zero, 64
#scddc_change

sc.ddc ca7, (zero)
andi a6, a6, 255
lui s2, 262148
slli s2, s2, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi a6, a6, 64
#scddc_change

sc.ddc cnull, (a6)
andi s2, s2, 255
lui s4, 262148
slli s4, s4, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi s2, s2, 64
#scddc_change

sc.ddc cs4, (s2)
andi s3, s3, 255
lui zero, 262148
slli zero, zero, 1
add s3, zero, s3

#sccap_change
auipcc cs3, 0
li t5, 32000
csetaddr cs3, cs3, t5
#sccap_change

sc.cap ca7, (cs3)
andi s2, s2, 255
lui sp, 262148
slli sp, sp, 1

#sccap_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
#sccap_change

sc.cap cs4, (cs2)
andi tp, tp, 255
lui s3, 262148
slli s3, s3, 1

#sccap_change
auipcc ctp, 0
li t5, 32000
csetaddr ctp, ctp, t5
#sccap_change

sc.cap cnull, (ctp)
cspecialrw cgp, mscratchc, cgp

#jalr.cap_change
auipcc cnull, 0
cincoffsetimm cnull, cnull, 12
#jalr.cap_change

jalr.cap cgp, cnull
cspecialrw ctp, mscratchc, csp
cspecialrw cra, mscratchc, cgp
andi sp, sp, 255
lui s2, 262148
slli s2, s2, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi sp, sp, 64
#scddc_change

sc.ddc cnull, (sp)
andi s4, s4, 255
lui tp, 262148
slli tp, tp, 1

#sccap_change
auipcc cs4, 0
li t5, 32000
csetaddr cs4, cs4, t5
#sccap_change

sc.cap cs2, (cs4)
andi s3, s3, 255
lui s3, 262148
slli s3, s3, 1
add s3, s3, s3

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi s3, s3, 64
#scddc_change

sc.ddc csp, (s3)
andi tp, tp, 255
lui a6, 262148
slli a6, a6, 1

#sccap_change
auipcc ctp, 0
li t5, 32000
csetaddr ctp, ctp, t5
#sccap_change

sc.cap cs3, (ctp)
andi s4, s4, 255
lui s3, 262148
slli s3, s3, 1
sh s4, 0(cnull)
andi a6, a6, 255
lui gp, 262148
slli gp, gp, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi a6, a6, 64
#scddc_change

sc.ddc cs2, (a6)
andi a6, a6, 255
lui a6, 262148
slli a6, a6, 1

#sccap_change
auipcc ca6, 0
li t5, 32000
csetaddr ca6, ca6, t5
#sccap_change

sc.cap csp, (ca6)
andi sp, sp, 255
lui gp, 262148
slli gp, gp, 1

#sccap_change
auipcc csp, 0
li t5, 32000
csetaddr csp, csp, t5
#sccap_change

sc.cap cgp, (csp)
andi a6, a6, 255
lui ra, 262148
slli ra, ra, 1
lc.ddc ca6, (a6)
andi s2, s2, 255
lui a6, 262148
slli a6, a6, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi s2, s2, 64
#scddc_change

sc.ddc cnull, (s2)
andi sp, sp, 255
lui s2, 262148
slli s2, s2, 1

#sccap_change
auipcc csp, 0
li t5, 32000
csetaddr csp, csp, t5
#sccap_change

sc.cap cs2, (csp)
andi s4, s4, 255
lui s3, 262148
slli s3, s3, 1

#sccap_change
auipcc cs4, 0
li t5, 32000
csetaddr cs4, cs4, t5
#sccap_change

sc.cap ca7, (cs4)
andi tp, tp, 255
lui a7, 262148
slli a7, a7, 1

#sccap_change
auipcc ctp, 0
li t5, 32000
csetaddr ctp, ctp, t5
#sccap_change

sc.cap csp, (ctp)
andi a7, a7, 255
lui a7, 262148
slli a7, a7, 1
add a7, a7, a7

#sccap_change
auipcc ca7, 0
li t5, 32000
csetaddr ca7, ca7, t5
#sccap_change

sc.cap cs3, (ca7)
andi s4, s4, 255
lui tp, 262148
slli tp, tp, 1

#sccap_change
auipcc cs4, 0
li t5, 32000
csetaddr cs4, cs4, t5
#sccap_change

sc.cap ctp, (cs4)
andi s4, s4, 255
lui s4, 262148
slli s4, s4, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi s4, s4, 64
#scddc_change

sc.ddc cgp, (s4)
cspecialr cs11, pcc
li t3, 0
csetflags cs11, cs11, t3
cspecialw mtcc, cs11

#jalr.cap_change
auipcc cs11, 0
cincoffsetimm cs11, cs11, 12
#jalr.cap_change

jr.cap cs11
andi s4, s4, 255
lui ra, 262148
slli ra, ra, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi s4, s4, 64
#scddc_change

sc.ddc cs4, (s4)
andi a6, a6, 255
lui ra, 262148
slli ra, ra, 1

#sccap_change
auipcc ca6, 0
li t5, 32000
csetaddr ca6, ca6, t5
#sccap_change

sc.cap ca7, (ca6)
andi s4, s4, 255
lui zero, 262148
slli zero, zero, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi s4, s4, 64
#scddc_change

sc.ddc ca7, (s4)
andi ra, ra, 255
lui a7, 262148
slli a7, a7, 1
add ra, a7, ra

#sccap_change
auipcc cra, 0
li t5, 32000
csetaddr cra, cra, t5
#sccap_change

sc.cap cra, (cra)
andi ra, ra, 255
lui a6, 262148
slli a6, a6, 1
add ra, a6, ra

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi ra, ra, 64
#scddc_change

sc.ddc ca7, (ra)
cgettag tp, ctp
andi ra, ra, 255
lui a7, 262148
slli a7, a7, 1
add ra, a7, ra

#sccap_change
auipcc cra, 0
li t5, 32000
csetaddr cra, cra, t5
#sccap_change

sc.cap cs3, (cra)
andi s2, s2, 255
lui a7, 262148
slli a7, a7, 1

#sccap_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
#sccap_change

sc.cap cs3, (cs2)
andi s2, s2, 255
lui sp, 262148
slli sp, sp, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi s2, s2, 64
#scddc_change

sc.ddc cs2, (s2)
andi zero, zero, 255
lui a6, 262148
slli a6, a6, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi zero, zero, 64
#scddc_change

sc.ddc csp, (zero)
andi ra, ra, 255
lui s4, 262148
slli s4, s4, 1
add ra, s4, ra

#sccap_change
auipcc cra, 0
li t5, 32000
csetaddr cra, cra, t5
#sccap_change

sc.cap cnull, (cra)
andi s3, s3, 255
lui sp, 262148
slli sp, sp, 1
add s3, sp, s3

#sccap_change
auipcc cs3, 0
li t5, 32000
csetaddr cs3, cs3, t5
#sccap_change

sc.cap csp, (cs3)
andi gp, gp, 255
lui a6, 262148
slli a6, a6, 1
add gp, a6, gp

#sccap_change
auipcc cgp, 0
li t5, 32000
csetaddr cgp, cgp, t5
#sccap_change

sc.cap cs3, (cgp)
andi s2, s2, 255
lui zero, 262148
slli zero, zero, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi s2, s2, 64
#scddc_change

sc.ddc cs3, (s2)
andi sp, sp, 255
lui zero, 262148
slli zero, zero, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi sp, sp, 64
#scddc_change

sc.ddc ca6, (sp)
andi zero, zero, 255
lui gp, 262148
slli gp, gp, 1

#sccap_change
auipcc cnull, 0
li t5, 32000
csetaddr cnull, cnull, t5
#sccap_change

sc.cap cnull, (cnull)
andi ra, ra, 255
lui s3, 262148
slli s3, s3, 1
add ra, s3, ra

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi ra, ra, 64
#scddc_change

sc.ddc cgp, (ra)
cspecialrw csp, mscratchc, csp

#jalr.cap_change
auipcc cnull, 0
cincoffsetimm cnull, cnull, 12
#jalr.cap_change

jalr.cap csp, cnull
cspecialrw ca6, mscratchc, cs4
cspecialrw cs2, mscratchc, csp
andi a7, a7, 255
lui a6, 262148
slli a6, a6, 1
add a7, a6, a7

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi a7, a7, 64
#scddc_change

sc.ddc cnull, (a7)
andi tp, tp, 255
lui a7, 262148
slli a7, a7, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi tp, tp, 64
#scddc_change

sc.ddc ctp, (tp)
andi sp, sp, 255
lui s4, 262148
slli s4, s4, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi sp, sp, 64
#scddc_change

sc.ddc cs2, (sp)
andi s4, s4, 255
lui a6, 262148
slli a6, a6, 1

#sccap_change
auipcc cs4, 0
li t5, 32000
csetaddr cs4, cs4, t5
#sccap_change

sc.cap cra, (cs4)
cgettag zero, cnull
andi a7, a7, 255
lui gp, 262148
slli gp, gp, 1
add a7, gp, a7

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi a7, a7, 64
#scddc_change

sc.ddc cs3, (a7)
andi a6, a6, 255
lui gp, 262148
slli gp, gp, 1

#sccap_change
auipcc ca6, 0
li t5, 32000
csetaddr ca6, ca6, t5
#sccap_change

sc.cap csp, (ca6)
andi s2, s2, 255
lui s4, 262148
slli s4, s4, 1

#sccap_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
#sccap_change

sc.cap ca7, (cs2)
andi ra, ra, 255
lui a6, 262148
slli a6, a6, 1
add ra, a6, ra

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi ra, ra, 64
#scddc_change

sc.ddc cra, (ra)
andi a6, a6, 255
lui gp, 262148
slli gp, gp, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi a6, a6, 64
#scddc_change

sc.ddc cra, (a6)
andi s4, s4, 255
lui s4, 262148
slli s4, s4, 1

#sccap_change
auipcc cs4, 0
li t5, 32000
csetaddr cs4, cs4, t5
#sccap_change

sc.cap ctp, (cs4)
andi gp, gp, 255
lui zero, 262148
slli zero, zero, 1
add gp, zero, gp

#sccap_change
auipcc cgp, 0
li t5, 32000
csetaddr cgp, cgp, t5
#sccap_change

sc.cap ca6, (cgp)
andi s3, s3, 255
lui a7, 262148
slli a7, a7, 1
add s3, a7, s3

#sccap_change
auipcc cs3, 0
li t5, 32000
csetaddr cs3, cs3, t5
#sccap_change

sc.cap cs4, (cs3)
andi tp, tp, 255
lui s3, 262148
slli s3, s3, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi tp, tp, 64
#scddc_change

sc.ddc ca6, (tp)
andi s4, s4, 255
lui gp, 262148
slli gp, gp, 1

#sccap_change
auipcc cs4, 0
li t5, 32000
csetaddr cs4, cs4, t5
#sccap_change

sc.cap cra, (cs4)
andi s3, s3, 255
lui gp, 262148
slli gp, gp, 1
add s3, gp, s3
sb s3, 0(ca7)
andi sp, sp, 255
lui s3, 262148
slli s3, s3, 1

#sccap_change
auipcc csp, 0
li t5, 32000
csetaddr csp, csp, t5
#sccap_change

sc.cap ca6, (csp)
andi sp, sp, 255
lui zero, 262148
slli zero, zero, 1

#sccap_change
auipcc csp, 0
li t5, 32000
csetaddr csp, csp, t5
#sccap_change

sc.cap csp, (csp)
andi s3, s3, 255
lui tp, 262148
slli tp, tp, 1
add s3, tp, s3

#sccap_change
auipcc cs3, 0
li t5, 32000
csetaddr cs3, cs3, t5
#sccap_change

sc.cap ca6, (cs3)
andi s2, s2, 255
lui a6, 262148
slli a6, a6, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi s2, s2, 64
#scddc_change

sc.ddc ca7, (s2)
andi s4, s4, 255
lui gp, 262148
slli gp, gp, 1

#sccap_change
auipcc cs4, 0
li t5, 32000
csetaddr cs4, cs4, t5
#sccap_change

sc.cap cra, (cs4)
andi ra, ra, 255
lui a7, 262148
slli a7, a7, 1
add ra, a7, ra

#sccap_change
auipcc cra, 0
li t5, 32000
csetaddr cra, cra, t5
#sccap_change

sc.cap cs4, (cra)
cgethigh zero, ca6
andi gp, gp, 255
lui a6, 262148
slli a6, a6, 1
add gp, a6, gp

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi gp, gp, 64
#scddc_change

sc.ddc ca7, (gp)
andi s2, s2, 255
lui a7, 262148
slli a7, a7, 1

#sccap_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
#sccap_change

sc.cap cgp, (cs2)
lui a6, 262148
slli a6, a6, 1
csetaddr ca7, ca7, a6
ccleartag ca6, csp

#sc_change
auipcc ca7, 0
li t5, 32000
csetaddr ca7, ca7, t5
#sc_change

sc ca6, 0(ca7)
cincoffset ca7, ca7, 16

#sc_change
auipcc ca7, 0
li t5, 32000
csetaddr ca7, ca7, t5
#sc_change

sc csp, 0(ca7)
cincoffset ca7, ca7, 16

#sc_change
auipcc ca7, 0
li t5, 32000
csetaddr ca7, ca7, t5
#sc_change

sc ca6, 0(ca7)
cincoffset ca7, ca7, 16

#sc_change
auipcc ca7, 0
li t5, 32000
csetaddr ca7, ca7, t5
#sc_change

sc csp, 0(ca7)
cincoffset ca7, ca7, 16

#sc_change
auipcc ca7, 0
li t5, 32000
csetaddr ca7, ca7, t5
#sc_change

sc csp, 0(ca7)
cincoffset ca7, ca7, 16
cincoffset ca7, ca7, -80
cloadtags a6, (ca7)
andi gp, gp, 255
lui gp, 262148
slli gp, gp, 1
add gp, gp, gp

#sccap_change
auipcc cgp, 0
li t5, 32000
csetaddr cgp, cgp, t5
#sccap_change

sc.cap ca6, (cgp)
andi s3, s3, 255
lui sp, 262148
slli sp, sp, 1
add s3, sp, s3

#sccap_change
auipcc cs3, 0
li t5, 32000
csetaddr cs3, cs3, t5
#sccap_change

sc.cap cra, (cs3)
andi tp, tp, 255
lui a6, 262148
slli a6, a6, 1

#sccap_change
auipcc ctp, 0
li t5, 32000
csetaddr ctp, ctp, t5
#sccap_change

sc.cap ca6, (ctp)
andi tp, tp, 255
lui a6, 262148
slli a6, a6, 1

#sccap_change
auipcc ctp, 0
li t5, 32000
csetaddr ctp, ctp, t5
#sccap_change

sc.cap cs2, (ctp)
andi gp, gp, 255
lui gp, 262148
slli gp, gp, 1
add gp, gp, gp

#sccap_change
auipcc cgp, 0
li t5, 32000
csetaddr cgp, cgp, t5
#sccap_change

sc.cap cra, (cgp)
andi s2, s2, 255
lui gp, 262148
slli gp, gp, 1

#sccap_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
#sccap_change

sc.cap ca7, (cs2)
andi s4, s4, 255
lui a6, 262148
slli a6, a6, 1

#sccap_change
auipcc cs4, 0
li t5, 32000
csetaddr cs4, cs4, t5
#sccap_change

sc.cap ca6, (cs4)
andi s4, s4, 255
lui s3, 262148
slli s3, s3, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi s4, s4, 64
#scddc_change

sc.ddc cgp, (s4)
andi ra, ra, 255
lui tp, 262148
slli tp, tp, 1
add ra, tp, ra

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi ra, ra, 64
#scddc_change

sc.ddc cs4, (ra)
andi gp, gp, 255
lui zero, 262148
slli zero, zero, 1
add gp, zero, gp

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi gp, gp, 64
#scddc_change

sc.ddc cra, (gp)
andi ra, ra, 255
lui s3, 262148
slli s3, s3, 1
add ra, s3, ra

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi ra, ra, 64
#scddc_change

sc.ddc cs3, (ra)
andi s4, s4, 255
lui sp, 262148
slli sp, sp, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi s4, s4, 64
#scddc_change

sc.ddc ca7, (s4)
andi s4, s4, 255
lui s3, 262148
slli s3, s3, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi s4, s4, 64
#scddc_change

sc.ddc cs4, (s4)
cgetoffset a6, csp
andi ra, ra, 255
lui gp, 262148
slli gp, gp, 1
add ra, gp, ra

#sccap_change
auipcc cra, 0
li t5, 32000
csetaddr cra, cra, t5
#sccap_change

sc.cap csp, (cra)
andi ra, ra, 255
lui s4, 262148
slli s4, s4, 1
add ra, s4, ra

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi ra, ra, 64
#scddc_change

sc.ddc cs3, (ra)
andi s3, s3, 255
lui ra, 262148
slli ra, ra, 1
add s3, ra, s3

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi s3, s3, 64
#scddc_change

sc.ddc ctp, (s3)
andi ra, ra, 255
lui a7, 262148
slli a7, a7, 1
add ra, a7, ra

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi ra, ra, 64
#scddc_change

sc.ddc cra, (ra)
andi a6, a6, 255
lui s4, 262148
slli s4, s4, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi a6, a6, 64
#scddc_change

sc.ddc ca7, (a6)
andi gp, gp, 255
lui sp, 262148
slli sp, sp, 1
add gp, sp, gp

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi gp, gp, 64
#scddc_change

sc.ddc ca7, (gp)
cspecialr ct4, pcc
li t5, 0
csetflags ct4, ct4, t5
cspecialw mtcc, ct4

#jalr.cap_change
auipcc ct4, 0
cincoffsetimm ct4, ct4, 12
#jalr.cap_change

jr.cap ct4
andi gp, gp, 255
lui zero, 262148
slli zero, zero, 1
add gp, zero, gp

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi gp, gp, 64
#scddc_change

sc.ddc cnull, (gp)
andi gp, gp, 255
lui a7, 262148
slli a7, a7, 1
add gp, a7, gp

#sccap_change
auipcc cgp, 0
li t5, 32000
csetaddr cgp, cgp, t5
#sccap_change

sc.cap cnull, (cgp)
andi gp, gp, 255
lui tp, 262148
slli tp, tp, 1
add gp, tp, gp
lb zero, 0(cgp)
andi gp, gp, 255
lui zero, 262148
slli zero, zero, 1
add gp, zero, gp

#sccap_change
auipcc cgp, 0
li t5, 32000
csetaddr cgp, cgp, t5
#sccap_change

sc.cap cra, (cgp)
andi gp, gp, 255
lui sp, 262148
slli sp, sp, 1
add gp, sp, gp

#sccap_change
auipcc cgp, 0
li t5, 32000
csetaddr cgp, cgp, t5
#sccap_change

sc.cap cra, (cgp)
andi a6, a6, 255
lui ra, 262148
slli ra, ra, 1

#sccap_change
auipcc ca6, 0
li t5, 32000
csetaddr ca6, ca6, t5
#sccap_change

sc.cap csp, (ca6)
andi a6, a6, 255
lui gp, 262148
slli gp, gp, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi a6, a6, 64
#scddc_change

sc.ddc cra, (a6)
andi s3, s3, 255
lui a6, 262148
slli a6, a6, 1
add s3, a6, s3

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi s3, s3, 64
#scddc_change

sc.ddc cra, (s3)
andi a7, a7, 255
lui gp, 262148
slli gp, gp, 1
add a7, gp, a7

#sccap_change
auipcc ca7, 0
li t5, 32000
csetaddr ca7, ca7, t5
#sccap_change

sc.cap cs4, (ca7)
andi s2, s2, 255
lui ra, 262148
slli ra, ra, 1

#sccap_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
#sccap_change

sc.cap cs2, (cs2)
andi s3, s3, 255
lui a6, 262148
slli a6, a6, 1
add s3, a6, s3

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi s3, s3, 64
#scddc_change

sc.ddc cs2, (s3)
andi zero, zero, 255
lui sp, 262148
slli sp, sp, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi zero, zero, 64
#scddc_change

sc.ddc cs3, (zero)
andi a7, a7, 255
lui ra, 262148
slli ra, ra, 1
add a7, ra, a7

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi a7, a7, 64
#scddc_change

sc.ddc ctp, (a7)
andi a7, a7, 255
lui tp, 262148
slli tp, tp, 1
add a7, tp, a7

#sccap_change
auipcc ca7, 0
li t5, 32000
csetaddr ca7, ca7, t5
#sccap_change

sc.cap csp, (ca7)
andi tp, tp, 255
lui zero, 262148
slli zero, zero, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi tp, tp, 64
#scddc_change

sc.ddc ca6, (tp)
andi gp, gp, 255
lui s3, 262148
slli s3, s3, 1
add gp, s3, gp

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi gp, gp, 64
#scddc_change

sc.ddc ca6, (gp)
lui zero, 262148
slli zero, zero, 1
csetaddr cs4, cs4, zero
ccleartag cnull, cs2

#sc_change
auipcc cs4, 0
li t5, 32000
csetaddr cs4, cs4, t5
#sc_change

sc cnull, 0(cs4)
cincoffset cs4, cs4, 16

#sc_change
auipcc cs4, 0
li t5, 32000
csetaddr cs4, cs4, t5
#sc_change

sc cs2, 0(cs4)
cincoffset cs4, cs4, 16

#sc_change
auipcc cs4, 0
li t5, 32000
csetaddr cs4, cs4, t5
#sc_change

sc cs2, 0(cs4)
cincoffset cs4, cs4, 16

#sc_change
auipcc cs4, 0
li t5, 32000
csetaddr cs4, cs4, t5
#sc_change

sc cnull, 0(cs4)
cincoffset cs4, cs4, 16

#sc_change
auipcc cs4, 0
li t5, 32000
csetaddr cs4, cs4, t5
#sc_change

sc cnull, 0(cs4)
cincoffset cs4, cs4, 16
cincoffset cs4, cs4, -80
cloadtags s4, (cs4)
lui sp, 262148
slli sp, sp, 1
csetaddr cnull, cnull, sp
ccleartag csp, cgp

#sc_change
auipcc cnull, 0
li t5, 32000
csetaddr cnull, cnull, t5
#sc_change

sc csp, 0(cnull)
cincoffset cnull, cnull, 16

#sc_change
auipcc cnull, 0
li t5, 32000
csetaddr cnull, cnull, t5
#sc_change

sc csp, 0(cnull)
cincoffset cnull, cnull, 16

#sc_change
auipcc cnull, 0
li t5, 32000
csetaddr cnull, cnull, t5
#sc_change

sc csp, 0(cnull)
cincoffset cnull, cnull, 16

#sc_change
auipcc cnull, 0
li t5, 32000
csetaddr cnull, cnull, t5
#sc_change

sc cgp, 0(cnull)
cincoffset cnull, cnull, 16

#sc_change
auipcc cnull, 0
li t5, 32000
csetaddr cnull, cnull, t5
#sc_change

sc csp, 0(cnull)
cincoffset cnull, cnull, 16
cincoffset cnull, cnull, -80
cloadtags zero, (cnull)
andi ra, ra, 255
lui zero, 262148
slli zero, zero, 1
add ra, zero, ra

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi ra, ra, 64
#scddc_change

sc.ddc cs2, (ra)
andi s4, s4, 255
lui s2, 262148
slli s2, s2, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi s4, s4, 64
#scddc_change

sc.ddc cs3, (s4)
andi sp, sp, 255
lui a7, 262148
slli a7, a7, 1

#sccap_change
auipcc csp, 0
li t5, 32000
csetaddr csp, csp, t5
#sccap_change

sc.cap csp, (csp)
cgettag s2, cs2
andi zero, zero, 255
lui s4, 262148
slli s4, s4, 1

#sccap_change
auipcc cnull, 0
li t5, 32000
csetaddr cnull, cnull, t5
#sccap_change

sc.cap cra, (cnull)
andi ra, ra, 255
lui s4, 262148
slli s4, s4, 1
add ra, s4, ra

#sccap_change
auipcc cra, 0
li t5, 32000
csetaddr cra, cra, t5
#sccap_change

sc.cap cra, (cra)
andi tp, tp, 255
lui s2, 262148
slli s2, s2, 1

#sccap_change
auipcc ctp, 0
li t5, 32000
csetaddr ctp, ctp, t5
#sccap_change

sc.cap cs4, (ctp)
andi a6, a6, 255
lui a7, 262148
slli a7, a7, 1

#sccap_change
auipcc ca6, 0
li t5, 32000
csetaddr ca6, ca6, t5
#sccap_change

sc.cap ctp, (ca6)
andi s3, s3, 255
lui a6, 262148
slli a6, a6, 1
add s3, a6, s3

#sccap_change
auipcc cs3, 0
li t5, 32000
csetaddr cs3, cs3, t5
#sccap_change

sc.cap cs4, (cs3)
andi sp, sp, 255
lui gp, 262148
slli gp, gp, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi sp, sp, 64
#scddc_change

sc.ddc cs4, (sp)
andi a7, a7, 255
lui sp, 262148
slli sp, sp, 1
add a7, sp, a7

#sccap_change
auipcc ca7, 0
li t5, 32000
csetaddr ca7, ca7, t5
#sccap_change

sc.cap cgp, (ca7)
andi a7, a7, 255
lui s2, 262148
slli s2, s2, 1
add a7, s2, a7

#sccap_change
auipcc ca7, 0
li t5, 32000
csetaddr ca7, ca7, t5
#sccap_change

sc.cap cra, (ca7)
andi ra, ra, 255
lui s4, 262148
slli s4, s4, 1
add ra, s4, ra

#sccap_change
auipcc cra, 0
li t5, 32000
csetaddr cra, cra, t5
#sccap_change

sc.cap ca7, (cra)
andi s3, s3, 255
lui tp, 262148
slli tp, tp, 1
add s3, tp, s3

#sccap_change
auipcc cs3, 0
li t5, 32000
csetaddr cs3, cs3, t5
#sccap_change

sc.cap cra, (cs3)
andi s2, s2, 255
lui a7, 262148
slli a7, a7, 1

#sccap_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
#sccap_change

sc.cap ca7, (cs2)
andi gp, gp, 255
lui zero, 262148
slli zero, zero, 1
add gp, zero, gp

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi gp, gp, 64
#scddc_change

sc.ddc cnull, (gp)
andi s2, s2, 255
lui gp, 262148
slli gp, gp, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi s2, s2, 64
#scddc_change

sc.ddc cgp, (s2)
andi s4, s4, 255
lui s2, 262148
slli s2, s2, 1

#sccap_change
auipcc cs4, 0
li t5, 32000
csetaddr cs4, cs4, t5
#sccap_change

sc.cap cgp, (cs4)
andi gp, gp, 255
lui gp, 262148
slli gp, gp, 1
add gp, gp, gp

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi gp, gp, 64
#scddc_change

sc.ddc ca6, (gp)
andi gp, gp, 255
lui a7, 262148
slli a7, a7, 1
add gp, a7, gp

#sccap_change
auipcc cgp, 0
li t5, 32000
csetaddr cgp, cgp, t5
#sccap_change

sc.cap ctp, (cgp)
andi a7, a7, 255
lui tp, 262148
slli tp, tp, 1
add a7, tp, a7

#sccap_change
auipcc ca7, 0
li t5, 32000
csetaddr ca7, ca7, t5
#sccap_change

sc.cap cnull, (ca7)
lui zero, 262148
slli zero, zero, 1
csetaddr ctp, ctp, zero
ccleartag cnull, cs3

#sc_change
auipcc ctp, 0
li t5, 32000
csetaddr ctp, ctp, t5
#sc_change

sc cnull, 0(ctp)
cincoffset ctp, ctp, 16

#sc_change
auipcc ctp, 0
li t5, 32000
csetaddr ctp, ctp, t5
#sc_change

sc cnull, 0(ctp)
cincoffset ctp, ctp, 16

#sc_change
auipcc ctp, 0
li t5, 32000
csetaddr ctp, ctp, t5
#sc_change

sc cnull, 0(ctp)
cincoffset ctp, ctp, 16

#sc_change
auipcc ctp, 0
li t5, 32000
csetaddr ctp, ctp, t5
#sc_change

sc cnull, 0(ctp)
cincoffset ctp, ctp, 16

#sc_change
auipcc ctp, 0
li t5, 32000
csetaddr ctp, ctp, t5
#sc_change

sc cs3, 0(ctp)
cincoffset ctp, ctp, 16
cincoffset ctp, ctp, -80
cloadtags ra, (ctp)
andi zero, zero, 255
lui a6, 262148
slli a6, a6, 1

#sccap_change
auipcc cnull, 0
li t5, 32000
csetaddr cnull, cnull, t5
#sccap_change

sc.cap csp, (cnull)
andi s4, s4, 255
lui s4, 262148
slli s4, s4, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi s4, s4, 64
#scddc_change

sc.ddc cnull, (s4)
cgettag tp, ctp
andi s4, s4, 255
lui sp, 262148
slli sp, sp, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi s4, s4, 64
#scddc_change

sc.ddc cnull, (s4)
andi a7, a7, 255
lui ra, 262148
slli ra, ra, 1
add a7, ra, a7

#sccap_change
auipcc ca7, 0
li t5, 32000
csetaddr ca7, ca7, t5
#sccap_change

sc.cap cs4, (ca7)
andi s2, s2, 255
lui tp, 262148
slli tp, tp, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi s2, s2, 64
#scddc_change

sc.ddc ca6, (s2)
andi s2, s2, 255
lui gp, 262148
slli gp, gp, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi s2, s2, 64
#scddc_change

sc.ddc cgp, (s2)
andi gp, gp, 255
lui s3, 262148
slli s3, s3, 1
add gp, s3, gp

#sccap_change
auipcc cgp, 0
li t5, 32000
csetaddr cgp, cgp, t5
#sccap_change

sc.cap ctp, (cgp)
andi a6, a6, 255
lui s4, 262148
slli s4, s4, 1

#sccap_change
auipcc ca6, 0
li t5, 32000
csetaddr ca6, ca6, t5
#sccap_change

sc.cap cs3, (ca6)
lui s2, 262148
slli s2, s2, 1
csetaddr cra, cra, s2
ccleartag cs2, cra

#sc_change
auipcc cra, 0
li t5, 32000
csetaddr cra, cra, t5
#sc_change

sc cra, 0(cra)
cincoffset cra, cra, 16

#sc_change
auipcc cra, 0
li t5, 32000
csetaddr cra, cra, t5
#sc_change

sc cra, 0(cra)
cincoffset cra, cra, 16

#sc_change
auipcc cra, 0
li t5, 32000
csetaddr cra, cra, t5
#sc_change

sc cs2, 0(cra)
cincoffset cra, cra, 16

#sc_change
auipcc cra, 0
li t5, 32000
csetaddr cra, cra, t5
#sc_change

sc cra, 0(cra)
cincoffset cra, cra, 16

#sc_change
auipcc cra, 0
li t5, 32000
csetaddr cra, cra, t5
#sc_change

sc cs2, 0(cra)
cincoffset cra, cra, 16
cincoffset cra, cra, -80
cloadtags zero, (cra)
andi gp, gp, 255
lui a7, 262148
slli a7, a7, 1
add gp, a7, gp

#sccap_change
auipcc cgp, 0
li t5, 32000
csetaddr cgp, cgp, t5
#sccap_change

sc.cap ca7, (cgp)
andi s4, s4, 255
lui a7, 262148
slli a7, a7, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi s4, s4, 64
#scddc_change

sc.ddc cra, (s4)
andi s2, s2, 255
lui a7, 262148
slli a7, a7, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi s2, s2, 64
#scddc_change

sc.ddc cra, (s2)
andi s3, s3, 255
lui s2, 262148
slli s2, s2, 1
add s3, s2, s3

#sccap_change
auipcc cs3, 0
li t5, 32000
csetaddr cs3, cs3, t5
#sccap_change

sc.cap ctp, (cs3)
andi s4, s4, 255
lui ra, 262148
slli ra, ra, 1

#sccap_change
auipcc cs4, 0
li t5, 32000
csetaddr cs4, cs4, t5
#sccap_change

sc.cap cgp, (cs4)
andi zero, zero, 255
lui sp, 262148
slli sp, sp, 1

#sccap_change
auipcc cnull, 0
li t5, 32000
csetaddr cnull, cnull, t5
#sccap_change

sc.cap ca7, (cnull)
andi a6, a6, 255
lui tp, 262148
slli tp, tp, 1

#sccap_change
auipcc ca6, 0
li t5, 32000
csetaddr ca6, ca6, t5
#sccap_change

sc.cap cs3, (ca6)
andi s3, s3, 255
lui zero, 262148
slli zero, zero, 1
add s3, zero, s3

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi s3, s3, 64
#scddc_change

sc.ddc ca7, (s3)
andi ra, ra, 255
lui ra, 262148
slli ra, ra, 1
add ra, ra, ra

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi ra, ra, 64
#scddc_change

sc.ddc ctp, (ra)
andi ra, ra, 255
lui ra, 262148
slli ra, ra, 1
add ra, ra, ra

#sccap_change
auipcc cra, 0
li t5, 32000
csetaddr cra, cra, t5
#sccap_change

sc.cap cnull, (cra)
andi s4, s4, 255
lui gp, 262148
slli gp, gp, 1

#sccap_change
auipcc cs4, 0
li t5, 32000
csetaddr cs4, cs4, t5
#sccap_change

sc.cap cra, (cs4)
andi s2, s2, 255
lui s4, 262148
slli s4, s4, 1

#sccap_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
#sccap_change

sc.cap cs2, (cs2)
andi sp, sp, 255
lui zero, 262148
slli zero, zero, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi sp, sp, 64
#scddc_change

sc.ddc csp, (sp)
andi a6, a6, 255
lui a7, 262148
slli a7, a7, 1

#scddc_change
auipcc cs2, 0
li t5, 32000
csetaddr cs2, cs2, t5
cspecialw ddc, cs2
andi a6, a6, 64
#scddc_change

sc.ddc cgp, (a6)
andi tp, tp, 255
lui s2, 262148
slli s2, s2, 1
lh sp, 0(ctp)
cspecialr ctp, pcc
li a7, 0
slli a7, a7, 11
mv a7, a7
slli a7, a7, 11
mv a7, a7
slli a7, a7, 11
mv a7, a7
slli a7, a7, 11
mv a7, a7
slli a7, a7, 8
addi a7, a7, 211
cincoffset ctp, ctp, a7
li a7, 0
slli a7, a7, 11
mv a7, a7
slli a7, a7, 11
mv a7, a7
slli a7, a7, 11
addi a7, a7, 1
slli a7, a7, 11
addi a7, a7, 265
slli a7, a7, 8
addi a7, a7, 91
csetbounds ctp, ctp, a7

#jalr.cap_change
auipcc ctp, 0
cincoffsetimm ctp, ctp, 12
#jalr.cap_change

jalr.cap ctp, ctp
