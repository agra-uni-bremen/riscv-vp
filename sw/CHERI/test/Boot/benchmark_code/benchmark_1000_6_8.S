andi	s3, s3, 255
lui	s3, 262148
slli	s3, s3, 1
add	s3, s3, s3
sc.ddc	csp, (s3)
andi	sp, sp, 255
lui	s3, 262148
slli	s3, s3, 1
sc.ddc	cs2, (sp)
lui	ra, 262148
slli	ra, ra, 1
csetaddr	cs3, cs3, ra
ccleartag	cra, cs2
sc	cra, 0(cs3)
cincoffset	cs3, cs3, 16
sc	cs2, 0(cs3)
cincoffset	cs3, cs3, 16
sc	cs2, 0(cs3)
cincoffset	cs3, cs3, 16
sc	cra, 0(cs3)
cincoffset	cs3, cs3, 16
sc	cra, 0(cs3)
cincoffset	cs3, cs3, 16
cincoffset	cs3, cs3, -80
cloadtags	sp, (cs3)
andi	sp, sp, 255
lui	sp, 262148
slli	sp, sp, 1
sc.cap	cgp, (csp)
andi	s3, s3, 255
lui	sp, 262148
slli	sp, sp, 1
add	s3, sp, s3
lbu	a6, 0(cs3)
cgettag	s4, cs4
cgettag	s3, cs3
andi	a6, a6, 255
lui	gp, 262148
slli	gp, gp, 1
sc.cap	csp, (ca6)
csetbounds	cs2, ca6, 21
li	tp, 0
slli	tp, tp, 12
csetoffset	cs2, cs2, tp
cincoffset	cs2, cs2, 29
lui	s2, 262148
slli	s2, s2, 1
csetaddr	cs3, cs3, s2
ccleartag	cs2, csp
sc	cs2, 0(cs3)
cincoffset	cs3, cs3, 16
sc	cs2, 0(cs3)
cincoffset	cs3, cs3, 16
sc	cs2, 0(cs3)
cincoffset	cs3, cs3, 16
sc	csp, 0(cs3)
cincoffset	cs3, cs3, 16
sc	cs2, 0(cs3)
cincoffset	cs3, cs3, 16
cincoffset	cs3, cs3, -80
cloadtags	s3, (cs3)
cseal	cgp, cgp, csp
cseal	cs3, cs3, csp
cinvoke	cgp, cs3
cmove	ct6, cra
bne	ra, gp, 0x10c3c
cgettag	sp, csp
cspecialrw	csp, ddc, csp
cspecialrw	ca7, mscratchc, cgp
cgettag	a7, ca7
andi	tp, tp, 255
lui	gp, 262148
slli	gp, gp, 1
sc.cap	ca7, (ctp)
andi	s2, s2, 255
lui	a7, 262148
slli	a7, a7, 1
sc.ddc	cra, (s2)
cgettag	a6, ca6
cgettag	a7, ca7
andi	s4, s4, 255
lui	gp, 262148
slli	gp, gp, 1
sc.ddc	cs4, (s4)
cspecialrw	cs3, pcc, cs4
andi	a7, a7, 255
lui	zero, 262148
slli	zero, zero, 1
add	a7, zero, a7
sc.cap	cs2, (ca7)
li	zero, -3
candperm	ca6, ca6, zero
cseal	cs3, cs3, cgp
cseal	ca6, ca6, cgp
cinvoke	cs3, ca6
cmove	ct6, cra
cspecialr	csp, pcc
li	s2, 0
slli	s2, s2, 11
mv	s2, s2
slli	s2, s2, 11
mv	s2, s2
slli	s2, s2, 11
mv	s2, s2
slli	s2, s2, 11
addi	s2, s2, 4
slli	s2, s2, 8
addi	s2, s2, 173
cincoffset	csp, csp, s2
li	s2, 0
slli	s2, s2, 11
mv	s2, s2
slli	s2, s2, 11
mv	s2, s2
slli	s2, s2, 11
addi	s2, s2, 1
slli	s2, s2, 11
addi	s2, s2, 1529
slli	s2, s2, 8
addi	s2, s2, 172
csetbounds	csp, csp, s2
jalr.cap	csp, csp
cgettag	zero, cnull
lui	tp, 841909
li	gp, -3
candperm	cs4, cs4, gp
cseal	cra, cra, cs2
cseal	cs4, cs4, cs2
cinvoke	cra, cs4
cmove	ct6, cra
andi	zero, zero, 255
lui	zero, 262148
slli	zero, zero, 1
sc.ddc	cgp, (zero)
jalr	cs3, -694(ca7)
lui	gp, 262148
slli	gp, gp, 1
csetaddr	csp, csp, gp
ccleartag	cgp, cs3
sc	cs3, 0(csp)
cincoffset	csp, csp, 16
sc	cgp, 0(csp)
cincoffset	csp, csp, 16
sc	cs3, 0(csp)
cincoffset	csp, csp, 16
sc	cs3, 0(csp)
cincoffset	csp, csp, 16
sc	cgp, 0(csp)
cincoffset	csp, csp, 16
cincoffset	csp, csp, -80
cloadtags	tp, (csp)
andi	sp, sp, 255
lui	ra, 262148
slli	ra, ra, 1
sc.cap	ca6, (csp)
and	s3, s3, a6
cgettag	s4, cs4
andi	tp, tp, 255
lui	s2, 262148
slli	s2, s2, 1
sc.ddc	csp, (tp)
cgettag	sp, csp
csetbounds	cs2, ca6, 4
li	a7, 3
slli	a7, a7, 12
csetoffset	cs2, cs2, a7
cincoffset	cs2, cs2, -564
cspecialr	cs6, pcc
li	s7, 1
csetflags	cs6, cs6, s7
cspecialw	mtcc, cs6
jr.cap	cs6
cgettag	s2, cs2
ccleartag	cs4, ca6
cspecialr	cs8, pcc
li	s9, 1
csetflags	cs8, cs8, s9
cspecialw	mtcc, cs8
jr.cap	cs8
andi	a6, a6, 255
lui	s2, 262148
slli	s2, s2, 1
sc.cap	ca6, (ca6)
andi	zero, zero, 255
lui	tp, 262148
slli	tp, tp, 1
sc.ddc	cs2, (zero)
cgettag	ra, cra
andi	sp, sp, 255
lui	a6, 262148
slli	a6, a6, 1
sc.ddc	cra, (sp)
cspecialr	cs3, pcc
li	s4, -1025
candperm	cs3, cs3, s4
cspecialw	mtcc, cs3
jalr.cap	cs3, cnull
andi	a6, a6, 255
lui	gp, 262148
slli	gp, gp, 1
sc.cap	cs4, (ca6)
andi	s4, s4, 255
lui	a6, 262148
slli	a6, a6, 1
sc.cap	cs2, (cs4)
andi	s4, s4, 255
lui	a7, 262148
slli	a7, a7, 1
sc.ddc	ctp, (s4)
csetboundsexact	cnull, cgp, zero
cspecialr	ct3, pcc
li	t4, 0
csetflags	ct3, ct3, t4
cspecialw	mtcc, ct3
jr.cap	ct3
cgettag	ra, cra
andi	a6, a6, 255
lui	a6, 262148
slli	a6, a6, 1
sc.cap	ctp, (ca6)
li	s2, -3
candperm	cs2, cs2, s2
cseal	cs3, cs3, ca7
cseal	cs2, cs2, ca7
cinvoke	cs3, cs2
cmove	ct6, cra
andi	sp, sp, 255
lui	a7, 262148
slli	a7, a7, 1
sc.ddc	cnull, (sp)
csetbounds	cs2, ca7, 7
nop
slli	zero, zero, 12
csetoffset	cs2, cs2, zero
cincoffset	cs2, cs2, 17
cspecialr	cgp, pcc
li	a6, -1025
candperm	cgp, cgp, a6
cspecialw	mtcc, cgp
jalr.cap	cgp, cnull
andi	s4, s4, 255
lui	sp, 262148
slli	sp, sp, 1
sc.cap	ca7, (cs4)
cspecialr	ct3, pcc
li	t4, 0
csetflags	ct3, ct3, t4
cspecialw	mtcc, ct3
jr.cap	ct3
andi	gp, gp, 255
lui	a6, 262148
slli	a6, a6, 1
add	gp, a6, gp
sc.ddc	cra, (gp)
cgettag	s3, cs3
cspecialrw	cgp, mscratchc, ca6
jalr.cap	cgp, cnull
cspecialrw	ca6, mscratchc, ctp
cspecialrw	ca7, mscratchc, cs4
cspecialrw	ca7, mtcc, cs2
cgettag	zero, cnull
cspecialr	cs9, pcc
li	s10, 1
csetflags	cs9, cs9, s10
cspecialw	mtcc, cs9
jr.cap	cs9
andi	a7, a7, 255
lui	a6, 262148
slli	a6, a6, 1
add	a7, a6, a7
sc.cap	cra, (ca7)
csetbounds	csp, cra, 29
li	a7, 0
slli	a7, a7, 12
csetoffset	csp, csp, a7
cincoffset	csp, csp, 12
andi	a7, a7, 255
lui	s3, 262148
slli	s3, s3, 1
add	a7, s3, a7
sc.ddc	cs4, (a7)
lui	gp, 262148
slli	gp, gp, 1
csetaddr	csp, csp, gp
ccleartag	cgp, ctp
sc	ctp, 0(csp)
cincoffset	csp, csp, 16
sc	ctp, 0(csp)
cincoffset	csp, csp, 16
sc	ctp, 0(csp)
cincoffset	csp, csp, 16
sc	ctp, 0(csp)
cincoffset	csp, csp, 16
sc	cgp, 0(csp)
cincoffset	csp, csp, 16
cincoffset	csp, csp, -80
cloadtags	zero, (csp)
cspecialr	ct3, pcc
li	t4, 1
csetflags	ct3, ct3, t4
cspecialw	mtcc, ct3
jr.cap	ct3
<unknown>
lui	ra, 262148
slli	ra, ra, 1
csetaddr	csp, csp, ra
ccleartag	cra, cs4
sc	cs4, 0(csp)
cincoffset	csp, csp, 16
sc	cra, 0(csp)
cincoffset	csp, csp, 16
sc	cs4, 0(csp)
cincoffset	csp, csp, 16
sc	cs4, 0(csp)
cincoffset	csp, csp, 16
sc	cs4, 0(csp)
cincoffset	csp, csp, 16
cincoffset	csp, csp, -80
cloadtags	tp, (csp)
ori	s2, tp, 397
cspecialr	cgp, pcc
nop
slli	zero, zero, 11
nop
slli	zero, zero, 11
nop
slli	zero, zero, 11
nop
slli	zero, zero, 11
li	zero, 9
slli	zero, zero, 8
li	zero, 71
cincoffset	cgp, cgp, zero
nop
slli	zero, zero, 11
nop
slli	zero, zero, 11
nop
slli	zero, zero, 11
nop
slli	zero, zero, 11
li	zero, 1168
slli	zero, zero, 8
li	zero, 38
csetbounds	cgp, cgp, zero
jalr.cap	cgp, cgp
cgettag	s4, cs4
andi	gp, gp, 255
lui	a7, 262148
slli	a7, a7, 1
add	gp, a7, gp
sc.ddc	cnull, (gp)
andi	zero, zero, 255
lui	s3, 262148
slli	s3, s3, 1
lb	sp, 0(cnull)
andi	ra, ra, 255
lui	s4, 262148
slli	s4, s4, 1
add	ra, s4, ra
sc.cap	csp, (cra)
andi	zero, zero, 255
lui	s4, 262148
slli	s4, s4, 1
lb	a6, 0(cnull)
csetbounds	cra, ca6, 7
li	gp, 2
slli	gp, gp, 12
csetoffset	cra, cra, gp
cincoffset	cra, cra, 832
andi	gp, gp, 255
lui	zero, 262148
slli	zero, zero, 1
add	gp, zero, gp
sc.ddc	ca6, (gp)
cgettag	gp, cgp
lui	s2, 262148
slli	s2, s2, 1
csetaddr	cra, cra, s2
ccleartag	cs2, ca7
sc	ca7, 0(cra)
cincoffset	cra, cra, 16
sc	cs2, 0(cra)
cincoffset	cra, cra, 16
sc	cs2, 0(cra)
cincoffset	cra, cra, 16
sc	ca7, 0(cra)
cincoffset	cra, cra, 16
sc	cs2, 0(cra)
cincoffset	cra, cra, 16
cincoffset	cra, cra, -80
cloadtags	s4, (cra)
csetflags	cs4, cs2, ra
csetbounds	ctp, cra, 13
li	a6, 0
slli	a6, a6, 12
csetoffset	ctp, ctp, a6
cincoffset	ctp, ctp, -485
andi	a6, a6, 255
lui	a7, 262148
slli	a7, a7, 1
lw	s3, 0(ca6)
andi	a6, a6, 255
lui	tp, 262148
slli	tp, tp, 1
sc.ddc	cs3, (a6)
cgettag	gp, cgp
csetbounds	cs4, cnull, 17
li	gp, 1
slli	gp, gp, 12
csetoffset	cs4, cs4, gp
cincoffset	cs4, cs4, 1820
andi	a7, a7, 255
lui	a7, 262148
slli	a7, a7, 1
add	a7, a7, a7
sc.cap	ctp, (ca7)
andi	ra, ra, 255
lui	gp, 262148
slli	gp, gp, 1
add	ra, gp, ra
sc.cap	cs3, (cra)
andi	s2, s2, 255
lui	zero, 262148
slli	zero, zero, 1
sc.cap	cra, (cs2)
cspecialrw	ca6, mscratchc, cgp
jalr.cap	ca6, cnull
cspecialrw	ca6, mscratchc, cra
cspecialrw	cra, mscratchc, cs3
cspecialr	ct3, pcc
li	t4, 1
csetflags	ct3, ct3, t4
cspecialw	mtcc, ct3
jr.cap	ct3
andi	zero, zero, 255
lui	s4, 262148
slli	s4, s4, 1
sc.ddc	ctp, (zero)
cspecialrw	cs3, mscratchc, csp
jalr.cap	cs3, cnull
cspecialrw	cs3, mscratchc, ca6
cspecialrw	ca6, mscratchc, ctp
andi	a6, a6, 255
lui	gp, 262148
slli	gp, gp, 1
sc.cap	cra, (ca6)
lui	gp, 262148
slli	gp, gp, 1
csetaddr	ca7, ca7, gp
ccleartag	cgp, cra
sc	cra, 0(ca7)
cincoffset	ca7, ca7, 16
sc	cgp, 0(ca7)
cincoffset	ca7, ca7, 16
sc	cgp, 0(ca7)
cincoffset	ca7, ca7, 16
sc	cgp, 0(ca7)
cincoffset	ca7, ca7, 16
sc	cgp, 0(ca7)
cincoffset	ca7, ca7, 16
cincoffset	ca7, ca7, -80
cloadtags	zero, (ca7)
andi	s2, s2, 255
lui	s3, 262148
slli	s3, s3, 1
sc.cap	ctp, (cs2)
andi	s2, s2, 255
lui	a7, 262148
slli	a7, a7, 1
sc.ddc	cra, (s2)
cspecialw	mscratchc, cra
jr.cap	cnull
cspecialrw	ca6, mscratchc, cs3
cspecialrw	csp, mscratchc, ca7
andi	zero, zero, 255
lui	s2, 262148
slli	s2, s2, 1
sc.cap	cs3, (cnull)
cspecialw	mtdc, ctp
andi	a7, a7, 255
lui	s3, 262148
slli	s3, s3, 1
add	a7, s3, a7
sc.cap	cs3, (ca7)
andi	tp, tp, 255
lui	a6, 262148
slli	a6, a6, 1
lc.ddc	cs2, (tp)
andi	a6, a6, 255
lui	sp, 262148
slli	sp, sp, 1
sc.cap	cgp, (ca6)
cgetlen	a6, ca7
andi	gp, gp, 255
lui	ra, 262148
slli	ra, ra, 1
add	gp, ra, gp
sc.cap	csp, (cgp)
lui	s3, 262148
slli	s3, s3, 1
csetaddr	cs2, cs2, s3
ccleartag	cs3, cra
sc	cra, 0(cs2)
cincoffset	cs2, cs2, 16
sc	cra, 0(cs2)
cincoffset	cs2, cs2, 16
sc	cra, 0(cs2)
cincoffset	cs2, cs2, 16
sc	cra, 0(cs2)
cincoffset	cs2, cs2, 16
sc	cra, 0(cs2)
cincoffset	cs2, cs2, 16
cincoffset	cs2, cs2, -80
cloadtags	s4, (cs2)
csetboundsexact	cnull, cs2, s3
andi	s2, s2, 255
lui	a7, 262148
slli	a7, a7, 1
sc.cap	cnull, (cs2)
li	a7, -3
candperm	csp, csp, a7
cseal	cs2, cs2, cra
cseal	csp, csp, cra
cinvoke	cs2, csp
cmove	ct6, cra
bgeu	s3, sp, 0x109c6
cspecialr	cgp, pcc
nop
slli	zero, zero, 11
nop
slli	zero, zero, 11
nop
slli	zero, zero, 11
nop
slli	zero, zero, 11
li	zero, 13
slli	zero, zero, 8
li	zero, 192
cincoffset	cgp, cgp, zero
nop
slli	zero, zero, 11
nop
slli	zero, zero, 11
nop
slli	zero, zero, 11
li	zero, 1
slli	zero, zero, 11
li	zero, 602
slli	zero, zero, 8
li	zero, 84
csetbounds	cgp, cgp, zero
jalr.cap	cgp, cgp
andi	tp, tp, 255
lui	s2, 262148
slli	s2, s2, 1
sc.ddc	cs2, (tp)
cgettag	gp, cgp
cspecialr	cs9, pcc
li	s10, 0
csetflags	cs9, cs9, s10
cspecialw	mtcc, cs9
jr.cap	cs9
lw	ra, 1605(cgp)
andi	ra, ra, 255
lui	zero, 262148
slli	zero, zero, 1
add	ra, zero, ra
sc.ddc	ca6, (ra)
cspecialr	cgp, pcc
li	tp, 0
slli	tp, tp, 11
mv	tp, tp
slli	tp, tp, 11
mv	tp, tp
slli	tp, tp, 11
mv	tp, tp
slli	tp, tp, 11
addi	tp, tp, 14
slli	tp, tp, 8
addi	tp, tp, 18
cincoffset	cgp, cgp, tp
li	tp, 0
slli	tp, tp, 11
mv	tp, tp
slli	tp, tp, 11
mv	tp, tp
slli	tp, tp, 11
addi	tp, tp, 1
slli	tp, tp, 11
addi	tp, tp, 1310
slli	tp, tp, 8
addi	tp, tp, 106
csetbounds	cgp, cgp, tp
jalr.cap	cgp, cgp
cspecialr	cs9, pcc
li	s10, 0
csetflags	cs9, cs9, s10
cspecialw	mtcc, cs9
jr.cap	cs9
cspecialr	cs10, pcc
li	s11, 1
csetflags	cs10, cs10, s11
cspecialw	mtcc, cs10
jr.cap	cs10
andi	a7, a7, 255
lui	zero, 262148
slli	zero, zero, 1
add	a7, zero, a7
sc.ddc	cgp, (a7)
cspecialr	cs3, mscratchc
jalr.cap	cs3, cnull
cspecialrw	ca7, mscratchc, cs2
cspecialrw	ctp, mscratchc, cs2
beq	s3, s3, 0x1277e <.text+0x16aa>
cgettag	ra, cra
csetbounds	cnull, ca6, 29
li	gp, 0
slli	gp, gp, 12
csetoffset	cnull, cnull, gp
cincoffset	cnull, cnull, 27
csetbounds	ctp, ctp, 21
li	s2, 0
slli	s2, s2, 12
csetoffset	ctp, ctp, s2
cincoffset	ctp, ctp, 1515
cgettag	s4, cs4
cgetflags	s4, ca7
andi	a7, a7, 255
lui	s2, 262148
slli	s2, s2, 1
add	a7, s2, a7
sc.ddc	ctp, (a7)
cspecialrw	cra, mscratchc, cs4
jalr.cap	cnull
cspecialrw	ca6, mscratchc, cs3
cspecialrw	cs3, mscratchc, cs3
cspecialr	cs4, pcc
li	sp, -1025
candperm	cs4, cs4, sp
cspecialw	mtcc, cs4
jalr.cap	cs4, cnull
andi	gp, gp, 255
lui	ra, 262148
slli	ra, ra, 1
add	gp, ra, gp
sc.cap	ca6, (cgp)
andi	s3, s3, 255
lui	s3, 262148
slli	s3, s3, 1
add	s3, s3, s3
sc.ddc	cnull, (s3)
andi	a7, a7, 255
lui	zero, 262148
slli	zero, zero, 1
add	a7, zero, a7
sc.cap	cs4, (ca7)
lui	s2, 205358
li	s3, -3
candperm	cnull, cnull, s3
cseal	ca6, ca6, cs3
cseal	cnull, cnull, cs3
cinvoke	ca6, cnull
cmove	ct6, cra
cgettag	sp, csp
lui	ra, 262148
slli	ra, ra, 1
csetaddr	cgp, cgp, ra
ccleartag	cra, ctp
sc	ctp, 0(cgp)
cincoffset	cgp, cgp, 16
sc	cra, 0(cgp)
cincoffset	cgp, cgp, 16
sc	cra, 0(cgp)
cincoffset	cgp, cgp, 16
sc	ctp, 0(cgp)
cincoffset	cgp, cgp, 16
sc	cra, 0(cgp)
cincoffset	cgp, cgp, 16
cincoffset	cgp, cgp, -80
cloadtags	gp, (cgp)
cgettag	a6, ca6
sh	a7, 485(cnull)
slli	gp, s4, 27
andi	a6, a6, 255
lui	sp, 262148
slli	sp, sp, 1
sc.ddc	cs3, (a6)
andi	sp, sp, 255
lui	s3, 262148
slli	s3, s3, 1
lc.ddc	cs4, (sp)
andi	sp, sp, 255
lui	gp, 262148
slli	gp, gp, 1
sc.cap	cgp, (csp)
csetbounds	cra, ca7, 24
li	s2, 2
slli	s2, s2, 12
csetoffset	cra, cra, s2
cincoffset	cra, cra, -854
andi	tp, tp, 255
lui	tp, 262148
slli	tp, tp, 1
sc.ddc	cnull, (tp)
cspecialrw	ctp, mscratchc, cs3
jalr.cap	ctp, cnull
cspecialrw	cs4, mscratchc, ca6
cspecialrw	csp, mscratchc, cra
addi	zero, tp, -3
candperm	cra, cra, tp
li	tp, -3
candperm	ctp, ctp, tp
cseal	cra, cra, cgp
cseal	ctp, ctp, cgp
cinvoke	cra, ctp
cmove	ct6, cra
cgettag	s3, cs3
andi	gp, gp, 255
lui	zero, 262148
slli	zero, zero, 1
add	gp, zero, gp
lc.ddc	cs2, (gp)
andi	s4, s4, 255
lui	a7, 262148
slli	a7, a7, 1
sc.cap	cra, (cs4)
csetbounds	ca6, csp, 10
li	s3, 0
slli	s3, s3, 12
csetoffset	ca6, ca6, s3
cincoffset	ca6, ca6, 31
lui	tp, 262148
slli	tp, tp, 1
csetaddr	cgp, cgp, tp
ccleartag	ctp, cra
sc	cra, 0(cgp)
cincoffset	cgp, cgp, 16
sc	ctp, 0(cgp)
cincoffset	cgp, cgp, 16
sc	ctp, 0(cgp)
cincoffset	cgp, cgp, 16
sc	ctp, 0(cgp)
cincoffset	cgp, cgp, 16
sc	cra, 0(cgp)
cincoffset	cgp, cgp, 16
cincoffset	cgp, cgp, -80
cloadtags	a7, (cgp)
csetbounds	cs3, cs2, 10
li	zero, 1
slli	zero, zero, 12
csetoffset	cs3, cs3, zero
cincoffset	cs3, cs3, 878
ebreak
li	tp, -3
candperm	ca6, ca6, tp
cseal	ca6, ca6, cgp
cinvoke	ca6, ca6
cmove	ct6, cra
cspecialr	ca6, pcc
li	sp, 0
slli	sp, sp, 11
mv	sp, sp
slli	sp, sp, 11
mv	sp, sp
slli	sp, sp, 11
mv	sp, sp
slli	sp, sp, 11
addi	sp, sp, 6
slli	sp, sp, 8
addi	sp, sp, 72
cincoffset	ca6, ca6, sp
li	sp, 0
slli	sp, sp, 11
mv	sp, sp
slli	sp, sp, 11
mv	sp, sp
slli	sp, sp, 11
addi	sp, sp, 1
slli	sp, sp, 11
addi	sp, sp, 1143
slli	sp, sp, 8
addi	sp, sp, 176
csetbounds	ca6, ca6, sp
jalr.cap	ca6, ca6
lui	sp, 262148
slli	sp, sp, 1
csetaddr	cra, cra, sp
ccleartag	csp, ca6
sc	ca6, 0(cra)
cincoffset	cra, cra, 16
sc	csp, 0(cra)
cincoffset	cra, cra, 16
sc	csp, 0(cra)
cincoffset	cra, cra, 16
sc	csp, 0(cra)
cincoffset	cra, cra, 16
sc	ca6, 0(cra)
cincoffset	cra, cra, 16
cincoffset	cra, cra, -80
cloadtags	s2, (cra)
andi	s3, s3, 255
lui	tp, 262148
slli	tp, tp, 1
add	s3, tp, s3
lw	s3, 0(cs3)
cspecialrw	cs4, mscratchc, cs4
jalr.cap	cs4, cnull
cspecialrw	ca6, mscratchc, ca6
cspecialrw	cs4, mscratchc, cs2
sra	s3, a6, a7
andi	zero, zero, 255
lui	ra, 262148
slli	ra, ra, 1
sc.cap	csp, (cnull)
jr	-462(cgp)
andi	ra, ra, 255
lui	a7, 262148
slli	a7, a7, 1
add	ra, a7, ra
sh	ra, 0(cgp)
andi	sp, sp, 255
lui	gp, 262148
slli	gp, gp, 1
sc.cap	ctp, (csp)
lui	gp, 262148
slli	gp, gp, 1
csetaddr	ca7, ca7, gp
ccleartag	cgp, ca7
sc	ca7, 0(ca7)
cincoffset	ca7, ca7, 16
sc	ca7, 0(ca7)
cincoffset	ca7, ca7, 16
sc	cgp, 0(ca7)
cincoffset	ca7, ca7, 16
sc	ca7, 0(ca7)
cincoffset	ca7, ca7, 16
sc	cgp, 0(ca7)
cincoffset	ca7, ca7, 16
cincoffset	ca7, ca7, -80
cloadtags	sp, (ca7)
andi	s4, s4, 255
lui	zero, 262148
slli	zero, zero, 1
sc.ddc	cnull, (s4)
cgettag	s3, cs3
li	tp, -3
candperm	cs4, cs4, tp
li	tp, -257
candperm	cra, cra, tp
li	tp, -257
candperm	cs4, cs4, tp
cseal	cra, cra, ca7
cseal	cs4, cs4, ca7
cinvoke	cra, cs4
cmove	ct6, cra
andi	ra, ra, 255
lui	a7, 262148
slli	a7, a7, 1
add	ra, a7, ra
sb	ra, 0(cnull)
andi	s4, s4, 255
lui	zero, 262148
slli	zero, zero, 1
sc.cap	ca6, (cs4)
andi	a7, a7, 255
lui	gp, 262148
slli	gp, gp, 1
add	a7, gp, a7
sc.cap	cs2, (ca7)
andi	sp, sp, 255
lui	zero, 262148
slli	zero, zero, 1
sc.cap	cra, (csp)
csetbounds	ca7, cgp, 3
li	a7, 1
slli	a7, a7, 12
csetoffset	ca7, ca7, a7
cincoffset	ca7, ca7, 1362
cspecialr	cs10, pcc
li	s11, 1
csetflags	cs10, cs10, s11
cspecialw	mtcc, cs10
jr.cap	cs10
lui	gp, 262148
slli	gp, gp, 1
csetaddr	cgp, cgp, gp
ccleartag	cgp, ctp
sc	ctp, 0(cgp)
cincoffset	cgp, cgp, 16
sc	cgp, 0(cgp)
cincoffset	cgp, cgp, 16
sc	cgp, 0(cgp)
cincoffset	cgp, cgp, 16
sc	cgp, 0(cgp)
cincoffset	cgp, cgp, 16
sc	ctp, 0(cgp)
cincoffset	cgp, cgp, 16
cincoffset	cgp, cgp, -80
cloadtags	zero, (cgp)
andi	ra, ra, 255
lui	ra, 262148
slli	ra, ra, 1
add	ra, ra, ra
sc.cap	ctp, (cra)
andi	sp, sp, 255
lui	zero, 262148
slli	zero, zero, 1
lc.ddc	cs2, (sp)
andi	zero, zero, 255
lui	sp, 262148
slli	sp, sp, 1
sw	zero, 0(ca6)
andi	s2, s2, 255
lui	zero, 262148
slli	zero, zero, 1
lc.ddc	cgp, (s2)
andi	tp, tp, 255
lui	a6, 262148
slli	a6, a6, 1
sc.cap	cs3, (ctp)
andi	sp, sp, 255
lui	s2, 262148
slli	s2, s2, 1
lw	s2, 0(csp)
cspecialw	ddc, cra
li	zero, -3
candperm	ctp, ctp, zero
li	zero, -257
candperm	ctp, ctp, zero
cseal	cs3, cs3, cs3
cseal	ctp, ctp, cs3
cinvoke	cs3, ctp
cmove	ct6, cra
cincoffset	csp, cs2, 62
lui	a7, 262148
slli	a7, a7, 1
csetaddr	csp, csp, a7
ccleartag	ca7, cs3
sc	ca7, 0(csp)
cincoffset	csp, csp, 16
sc	ca7, 0(csp)
cincoffset	csp, csp, 16
sc	ca7, 0(csp)
cincoffset	csp, csp, 16
sc	ca7, 0(csp)
cincoffset	csp, csp, 16
sc	cs3, 0(csp)
cincoffset	csp, csp, 16
cincoffset	csp, csp, -80
cloadtags	tp, (csp)
cgettag	zero, cnull
cspecialrw	cs2, mscratchc, ca6
jalr.cap	cs2, cnull
cspecialrw	cra, mscratchc, cs2
cspecialrw	ctp, mscratchc, csp
cgettag	tp, ctp
andi	tp, tp, 255
lui	a7, 262148
slli	a7, a7, 1
sc.cap	ctp, (ctp)
andi	a6, a6, 255
lui	gp, 262148
slli	gp, gp, 1
sc.cap	cs2, (ca6)
andi	ra, ra, 255
lui	a7, 262148
slli	a7, a7, 1
add	ra, a7, ra
sc.cap	cs3, (cra)
csetbounds	cs4, ca7, 0
li	gp, 1
slli	gp, gp, 12
csetoffset	cs4, cs4, gp
cincoffset	cs4, cs4, -12
lui	zero, 262148
slli	zero, zero, 1
csetaddr	cs3, cs3, zero
ccleartag	cnull, csp
sc	cnull, 0(cs3)
cincoffset	cs3, cs3, 16
sc	cnull, 0(cs3)
cincoffset	cs3, cs3, 16
sc	cnull, 0(cs3)
cincoffset	cs3, cs3, 16
sc	csp, 0(cs3)
cincoffset	cs3, cs3, 16
sc	cnull, 0(cs3)
cincoffset	cs3, cs3, 16
cincoffset	cs3, cs3, -80
cloadtags	a6, (cs3)
andi	s3, s3, 255
lui	s4, 262148
slli	s4, s4, 1
add	s3, s4, s3
lw	zero, 0(cs3)
andi	ra, ra, 255
lui	a6, 262148
slli	a6, a6, 1
add	ra, a6, ra
sc.ddc	cs4, (ra)
cgetsealed	zero, ctp
ccseal	ca7, cs2, ctp
li	a6, -3
candperm	cra, cra, a6
cseal	csp, csp, cgp
cseal	cra, cra, cgp
cinvoke	csp, cra
cmove	ct6, cra
cspecialr	ca6, pcc
li	sp, 0
slli	sp, sp, 11
mv	sp, sp
slli	sp, sp, 11
mv	sp, sp
slli	sp, sp, 11
mv	sp, sp
slli	sp, sp, 11
addi	sp, sp, 2
slli	sp, sp, 8
addi	sp, sp, 234
cincoffset	ca6, ca6, sp
li	sp, 0
slli	sp, sp, 11
mv	sp, sp
slli	sp, sp, 11
mv	sp, sp
slli	sp, sp, 11
mv	sp, sp
slli	sp, sp, 11
addi	sp, sp, 2003
slli	sp, sp, 8
addi	sp, sp, 31
csetbounds	ca6, ca6, sp
jalr.cap	ca6, ca6
cgettag	sp, csp
cgettag	zero, cnull
cspecialrw	cs2, mtcc, ctp
cgettag	s2, cs2
cgettag	s3, cs3
andi	gp, gp, 255
lui	s4, 262148
slli	s4, s4, 1
add	gp, s4, gp
sc.ddc	cnull, (gp)
andi	sp, sp, 255
lui	ra, 262148
slli	ra, ra, 1
sc.cap	cs3, (csp)
andi	s4, s4, 255
lui	ra, 262148
slli	ra, ra, 1
lbu	zero, 0(cs4)
andi	gp, gp, 255
lui	a7, 262148
slli	a7, a7, 1
add	gp, a7, gp
lc.ddc	cs2, (gp)
andi	a7, a7, 255
lui	s4, 262148
slli	s4, s4, 1
add	a7, s4, a7
sc.ddc	csp, (a7)
lui	s2, 262148
slli	s2, s2, 1
csetaddr	cs3, cs3, s2
ccleartag	cs2, cgp
sc	cgp, 0(cs3)
cincoffset	cs3, cs3, 16
sc	cs2, 0(cs3)
cincoffset	cs3, cs3, 16
sc	cgp, 0(cs3)
cincoffset	cs3, cs3, 16
sc	cgp, 0(cs3)
cincoffset	cs3, cs3, 16
sc	cgp, 0(cs3)
cincoffset	cs3, cs3, 16
cincoffset	cs3, cs3, -80
cloadtags	s2, (cs3)
andi	s2, s2, 255
lui	gp, 262148
slli	gp, gp, 1
sc.cap	cs4, (cs2)
cgettag	s3, cs3
csetbounds	cgp, cnull, 15
li	s3, 0
slli	s3, s3, 12
csetoffset	cgp, cgp, s3
cincoffset	cgp, cgp, 28
cspecialrw	cs2, mtcc, cs4
blt	tp, s2, 0x1268c <.text+0x15b8>
li	s3, -3
candperm	ca7, ca7, s3
cseal	cs4, cs4, csp
cseal	ca7, ca7, csp
cinvoke	cs4, ca7
cmove	ct6, cra
csetbounds	csp, cs3, 15
li	s4, 2
slli	s4, s4, 12
csetoffset	csp, csp, s4
cincoffset	csp, csp, 1206
cspecialr	cs6, pcc
li	s7, 0
csetflags	cs6, cs6, s7
cspecialw	mtcc, cs6
jr.cap	cs6
andi	gp, gp, 255
lui	s2, 262148
slli	s2, s2, 1
add	gp, s2, gp
lc.ddc	cgp, (gp)
cspecialrw	cra, pcc, ca7
cgettag	zero, cnull
cgettag	a7, ca7
andi	a7, a7, 255
lui	s4, 262148
slli	s4, s4, 1
add	a7, s4, a7
lbu	gp, 0(ca7)
cspecialrw	ca7, mtcc, ca6
andi	sp, sp, 255
lui	zero, 262148
slli	zero, zero, 1
sc.cap	cs3, (csp)
cspecialr	ct3, pcc
li	t4, 1
csetflags	ct3, ct3, t4
cspecialw	mtcc, ct3
jr.cap	ct3
cspecialr	cs3, pcc
li	a7, 0
slli	a7, a7, 11
mv	a7, a7
slli	a7, a7, 11
mv	a7, a7
slli	a7, a7, 11
mv	a7, a7
slli	a7, a7, 11
addi	a7, a7, 15
slli	a7, a7, 8
addi	a7, a7, 193
cincoffset	cs3, cs3, a7
li	a7, 0
slli	a7, a7, 11
mv	a7, a7
slli	a7, a7, 11
mv	a7, a7
slli	a7, a7, 11
addi	a7, a7, 1
slli	a7, a7, 11
addi	a7, a7, 357
slli	a7, a7, 8
addi	a7, a7, 11
csetbounds	cs3, cs3, a7
jalr.cap	cs3, cs3
csetbounds	ctp, cs2, s3
andi	s2, s2, 255
lui	s3, 262148
slli	s3, s3, 1
sc.cap	ca7, (cs2)
cspecialr	cs2, pcc
li	s2, 0
slli	s2, s2, 11
mv	s2, s2
slli	s2, s2, 11
mv	s2, s2
slli	s2, s2, 11
mv	s2, s2
slli	s2, s2, 11
addi	s2, s2, 15
slli	s2, s2, 8
addi	s2, s2, 213
cincoffset	cs2, cs2, s2
li	s2, 0
slli	s2, s2, 11
mv	s2, s2
slli	s2, s2, 11
mv	s2, s2
slli	s2, s2, 11
mv	s2, s2
slli	s2, s2, 11
addi	s2, s2, 1587
slli	s2, s2, 8
addi	s2, s2, 248
csetbounds	cs2, cs2, s2
jalr.cap	cs2, cs2
