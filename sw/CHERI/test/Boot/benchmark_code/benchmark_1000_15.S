andi	a7, a7, 255
lui	a7, 262148
slli	a7, a7, 1
add	a7, a7, a7
sc.cap	ca7, (ca7)
andi	a7, a7, 255
lui	sp, 262148
slli	sp, sp, 1
add	a7, sp, a7
sc.ddc	ca6, (a7)
csetbounds	ctp, ca6, 31
li	s4, 0
slli	s4, s4, 12
csetoffset	ctp, ctp, s4
cincoffset	ctp, ctp, 11
andi	sp, sp, 255
lui	s3, 262148
slli	s3, s3, 1
sc.cap	cnull, (csp)
<unknown>
cgettag	gp, cgp
andi	s3, s3, 255
lui	s2, 262148
slli	s2, s2, 1
add	s3, s2, s3
sc.cap	ca7, (cs3)
cgettag	ra, cra
lui	a6, 262148
slli	a6, a6, 1
csetaddr	ctp, ctp, a6
ccleartag	ca6, ca7
sc	ca6, 0(ctp)
cincoffset	ctp, ctp, 16
sc	ca7, 0(ctp)
cincoffset	ctp, ctp, 16
sc	ca6, 0(ctp)
cincoffset	ctp, ctp, 16
sc	ca6, 0(ctp)
cincoffset	ctp, ctp, 16
sc	ca7, 0(ctp)
cincoffset	ctp, ctp, 16
cincoffset	ctp, ctp, -80
cloadtags	a6, (ctp)
andi	sp, sp, 255
lui	s4, 262148
slli	s4, s4, 1
sc.cap	cra, (csp)
cspecialr	ca7, pcc
li	a7, 0
slli	a7, a7, 11
mv	a7, a7
slli	a7, a7, 11
mv	a7, a7
slli	a7, a7, 11
mv	a7, a7
slli	a7, a7, 11
addi	a7, a7, 8
slli	a7, a7, 8
addi	a7, a7, 93
cincoffset	ca7, ca7, a7
li	a7, 0
slli	a7, a7, 11
mv	a7, a7
slli	a7, a7, 11
mv	a7, a7
slli	a7, a7, 11
addi	a7, a7, 1
slli	a7, a7, 11
addi	a7, a7, 303
slli	a7, a7, 8
addi	a7, a7, 32
csetbounds	ca7, ca7, a7
jalr.cap	ca7, ca7
andi	tp, tp, 255
lui	a6, 262148
slli	a6, a6, 1
sc.ddc	ctp, (tp)
csetbounds	cra, cra, 3
li	a7, 0
slli	a7, a7, 12
csetoffset	cra, cra, a7
cincoffset	cra, cra, 25
lui	ra, 262148
slli	ra, ra, 1
csetaddr	cnull, cnull, ra
ccleartag	cra, cra
sc	cra, 0(cnull)
cincoffset	cnull, cnull, 16
sc	cra, 0(cnull)
cincoffset	cnull, cnull, 16
sc	cra, 0(cnull)
cincoffset	cnull, cnull, 16
sc	cra, 0(cnull)
cincoffset	cnull, cnull, 16
sc	cra, 0(cnull)
cincoffset	cnull, cnull, 16
cincoffset	cnull, cnull, -80
cloadtags	s2, (cnull)
cspecialr	cs3, pcc
li	sp, -1025
candperm	cs3, cs3, sp
cspecialw	mtcc, cs3
jalr.cap	cs3, cnull
andi	ra, ra, 255
lui	sp, 262148
slli	sp, sp, 1
add	ra, sp, ra
sc.ddc	cs3, (ra)
andi	a6, a6, 255
lui	zero, 262148
slli	zero, zero, 1
sc.ddc	cnull, (a6)
andi	tp, tp, 255
lui	ra, 262148
slli	ra, ra, 1
sc.ddc	cgp, (tp)
andi	a6, a6, 255
lui	gp, 262148
slli	gp, gp, 1
sc.ddc	cgp, (a6)
andi	s3, s3, 255
lui	ra, 262148
slli	ra, ra, 1
add	s3, ra, s3
sc.cap	cgp, (cs3)
cgetflags	a6, cra
andi	sp, sp, 255
lui	a6, 262148
slli	a6, a6, 1
sc.ddc	cs3, (sp)
sw	tp, -1670(cra)
andi	a7, a7, 255
lui	a6, 262148
slli	a6, a6, 1
add	a7, a6, a7
sc.ddc	cs4, (a7)
cspecialr	ct3, pcc
li	t4, 0
csetflags	ct3, ct3, t4
cspecialw	mtcc, ct3
jr.cap	ct3
andi	s3, s3, 255
lui	a6, 262148
slli	a6, a6, 1
add	s3, a6, s3
sc.ddc	cs3, (s3)
cspecialrw	ctp, pcc, cra
andi	zero, zero, 255
lui	zero, 262148
slli	zero, zero, 1
sc.ddc	cs3, (zero)
andi	s3, s3, 255
lui	tp, 262148
slli	tp, tp, 1
add	s3, tp, s3
sc.ddc	ca7, (s3)
csetbounds	csp, cra, 22
li	tp, 0
slli	tp, tp, 12
csetoffset	csp, csp, tp
cincoffset	csp, csp, 29
andi	zero, zero, 255
lui	ra, 262148
slli	ra, ra, 1
sc.cap	cra, (cnull)
andi	ra, ra, 255
lui	s3, 262148
slli	s3, s3, 1
add	ra, s3, ra
sc.cap	ca7, (cra)
cspecialr	cs2, pcc
li	ra, -1025
candperm	cs2, cs2, ra
cspecialw	mtcc, cs2
jalr.cap	cs2, cnull
andi	gp, gp, 255
lui	ra, 262148
slli	ra, ra, 1
add	gp, ra, gp
sc.ddc	ca6, (gp)
andi	s4, s4, 255
lui	s2, 262148
slli	s2, s2, 1
sb	s4, 0(ctp)
andi	gp, gp, 255
lui	gp, 262148
slli	gp, gp, 1
add	gp, gp, gp
sc.ddc	cs2, (gp)
andi	sp, sp, 255
lui	s2, 262148
slli	s2, s2, 1
sc.ddc	ctp, (sp)
andi	gp, gp, 255
lui	a6, 262148
slli	a6, a6, 1
add	gp, a6, gp
sc.ddc	ca6, (gp)
cspecialrw	cra, mscratchc, csp
jalr.cap	cnull
cspecialrw	ca6, mscratchc, csp
cspecialrw	ctp, mscratchc, ctp
andi	s2, s2, 255
lui	tp, 262148
slli	tp, tp, 1
lc.ddc	csp, (s2)
csetbounds	cs2, cgp, 17
li	ra, 0
slli	ra, ra, 12
csetoffset	cs2, cs2, ra
cincoffset	cs2, cs2, 1675
andi	zero, zero, 255
lui	gp, 262148
slli	gp, gp, 1
sc.ddc	ca7, (zero)
cgettag	a7, ca7
lui	zero, 262148
slli	zero, zero, 1
csetaddr	cnull, cnull, zero
ccleartag	cnull, cra
sc	cra, 0(cnull)
cincoffset	cnull, cnull, 16
sc	cra, 0(cnull)
cincoffset	cnull, cnull, 16
sc	cra, 0(cnull)
cincoffset	cnull, cnull, 16
sc	cnull, 0(cnull)
cincoffset	cnull, cnull, 16
sc	cnull, 0(cnull)
cincoffset	cnull, cnull, 16
cincoffset	cnull, cnull, -80
cloadtags	ra, (cnull)
andi	s2, s2, 255
lui	s4, 262148
slli	s4, s4, 1
sc.cap	ca7, (cs2)
cspecialr	ca6, pcc
li	a7, -1025
candperm	ca6, ca6, a7
cspecialw	mtcc, ca6
jalr.cap	ca6, cnull
andi	tp, tp, 255
lui	a7, 262148
slli	a7, a7, 1
lbu	sp, 0(ctp)
cgettag	ra, cra
andi	sp, sp, 255
lui	ra, 262148
slli	ra, ra, 1
sc.ddc	cra, (sp)
andi	s4, s4, 255
lui	a7, 262148
slli	a7, a7, 1
sc.cap	cnull, (cs4)
andi	a7, a7, 255
lui	sp, 262148
slli	sp, sp, 1
add	a7, sp, a7
sc.ddc	ca7, (a7)
cgettag	a6, ca6
andi	zero, zero, 255
lui	a6, 262148
slli	a6, a6, 1
sc.ddc	cs3, (zero)
andi	a7, a7, 255
lui	s4, 262148
slli	s4, s4, 1
add	a7, s4, a7
sc.ddc	cnull, (a7)
andi	a6, a6, 255
lui	a7, 262148
slli	a7, a7, 1
sc.cap	cra, (ca6)
andi	s4, s4, 255
lui	a7, 262148
slli	a7, a7, 1
sc.ddc	cs2, (s4)
cspecialr	cra, pcc
li	sp, -1025
candperm	cra, cra, sp
cspecialw	mtcc, cra
jalr.cap	cnull
csetbounds	cgp, ctp, 22
li	a7, 0
slli	a7, a7, 12
csetoffset	cgp, cgp, a7
cincoffset	cgp, cgp, 492
cspecialrw	cra, pcc, cgp
lui	gp, 262148
slli	gp, gp, 1
csetaddr	ctp, ctp, gp
ccleartag	cgp, ca6
sc	cgp, 0(ctp)
cincoffset	ctp, ctp, 16
sc	ca6, 0(ctp)
cincoffset	ctp, ctp, 16
sc	ca6, 0(ctp)
cincoffset	ctp, ctp, 16
sc	ca6, 0(ctp)
cincoffset	ctp, ctp, 16
sc	ca6, 0(ctp)
cincoffset	ctp, ctp, 16
cincoffset	ctp, ctp, -80
cloadtags	s3, (ctp)
andi	s3, s3, 255
lui	zero, 262148
slli	zero, zero, 1
add	s3, zero, s3
sc.ddc	cs4, (s3)
andi	zero, zero, 255
lui	sp, 262148
slli	sp, sp, 1
sc.cap	cnull, (cnull)
andi	tp, tp, 255
lui	s3, 262148
slli	s3, s3, 1
sc.ddc	cs4, (tp)
andi	s4, s4, 255
lui	gp, 262148
slli	gp, gp, 1
sc.ddc	cs3, (s4)
andi	sp, sp, 255
lui	s3, 262148
slli	s3, s3, 1
sc.ddc	ca7, (sp)
cgettag	a6, ca6
andi	gp, gp, 255
lui	gp, 262148
slli	gp, gp, 1
add	gp, gp, gp
sc.ddc	ctp, (gp)
lui	a7, 262148
slli	a7, a7, 1
csetaddr	cnull, cnull, a7
ccleartag	ca7, cs2
sc	cs2, 0(cnull)
cincoffset	cnull, cnull, 16
sc	cs2, 0(cnull)
cincoffset	cnull, cnull, 16
sc	cs2, 0(cnull)
cincoffset	cnull, cnull, 16
sc	ca7, 0(cnull)
cincoffset	cnull, cnull, 16
sc	cs2, 0(cnull)
cincoffset	cnull, cnull, 16
cincoffset	cnull, cnull, -80
cloadtags	tp, (cnull)
andi	s3, s3, 255
lui	tp, 262148
slli	tp, tp, 1
add	s3, tp, s3
sc.cap	csp, (cs3)
andi	gp, gp, 255
lui	s2, 262148
slli	s2, s2, 1
add	gp, s2, gp
sc.cap	cgp, (cgp)
andi	a7, a7, 255
lui	s3, 262148
slli	s3, s3, 1
add	a7, s3, a7
sc.cap	cnull, (ca7)
andi	zero, zero, 255
lui	sp, 262148
slli	sp, sp, 1
sc.ddc	csp, (zero)
andi	a6, a6, 255
lui	ra, 262148
slli	ra, ra, 1
sc.ddc	ctp, (a6)
andi	zero, zero, 255
lui	gp, 262148
slli	gp, gp, 1
sc.cap	cnull, (cnull)
cspecialw	2, cra
andi	gp, gp, 255
lui	tp, 262148
slli	tp, tp, 1
add	gp, tp, gp
sc.ddc	ca6, (gp)
cspecialrw	ca7, mscratchc, ca7
jalr.cap	ca7, cnull
cspecialr	cs4, mscratchc
cspecialrw	cgp, mscratchc, cs3
andi	s4, s4, 255
lui	s4, 262148
slli	s4, s4, 1
sc.ddc	cs3, (s4)
andi	s4, s4, 255
lui	a7, 262148
slli	a7, a7, 1
sc.ddc	cs3, (s4)
andi	zero, zero, 255
lui	zero, 262148
slli	zero, zero, 1
sc.ddc	cnull, (zero)
andi	s3, s3, 255
lui	s3, 262148
slli	s3, s3, 1
add	s3, s3, s3
sc.ddc	ca7, (s3)
blez	sp, 0x1140a <.text+0x336>
andi	a6, a6, 255
lui	sp, 262148
slli	sp, sp, 1
sc.ddc	ctp, (a6)
andi	a7, a7, 255
lui	ra, 262148
slli	ra, ra, 1
add	a7, ra, a7
sc.ddc	cgp, (a7)
andi	s4, s4, 255
lui	s4, 262148
slli	s4, s4, 1
sc.ddc	cs3, (s4)
andi	s4, s4, 255
lui	gp, 262148
slli	gp, gp, 1
sc.cap	csp, (cs4)
csetbounds	ca6, cra, 18
li	s2, 0
slli	s2, s2, 12
csetoffset	ca6, ca6, s2
cincoffset	ca6, ca6, 31
cspecialr	cs9, pcc
li	s10, 0
csetflags	cs9, cs9, s10
cspecialw	mtcc, cs9
jr.cap	cs9
andi	s2, s2, 255
lui	gp, 262148
slli	gp, gp, 1
sc.ddc	cs4, (s2)
andi	ra, ra, 255
lui	s2, 262148
slli	s2, s2, 1
add	ra, s2, ra
sc.ddc	cra, (ra)
li	a7, -3
candperm	cs3, cs3, a7
cseal	csp, csp, cs4
cseal	cs3, cs3, cs4
cinvoke	csp, cs3
cmove	ct6, cra
andi	ra, ra, 255
lui	zero, 262148
slli	zero, zero, 1
add	ra, zero, ra
sc.cap	cs4, (cra)
andi	a7, a7, 255
lui	s4, 262148
slli	s4, s4, 1
add	a7, s4, a7
sc.ddc	cs4, (a7)
andi	s2, s2, 255
lui	s3, 262148
slli	s3, s3, 1
sc.ddc	cs3, (s2)
andi	s4, s4, 255
lui	s2, 262148
slli	s2, s2, 1
sc.ddc	ca6, (s4)
cspecialr	cnull, pcc
li	s2, 0
slli	s2, s2, 11
mv	s2, s2
slli	s2, s2, 11
mv	s2, s2
slli	s2, s2, 11
mv	s2, s2
slli	s2, s2, 11
addi	s2, s2, 13
slli	s2, s2, 8
addi	s2, s2, 38
cincoffset	cnull, cnull, s2
li	s2, 0
slli	s2, s2, 11
mv	s2, s2
slli	s2, s2, 11
mv	s2, s2
slli	s2, s2, 11
addi	s2, s2, 1
slli	s2, s2, 11
addi	s2, s2, 1891
slli	s2, s2, 8
addi	s2, s2, 38
csetbounds	cnull, cnull, s2
jr.cap	cnull
andi	a7, a7, 255
lui	tp, 262148
slli	tp, tp, 1
add	a7, tp, a7
lb	ra, 0(ca7)
srai	a6, sp, 6
andi	tp, tp, 255
lui	s4, 262148
slli	s4, s4, 1
sc.cap	cs4, (ctp)
andi	a7, a7, 255
lui	a6, 262148
slli	a6, a6, 1
add	a7, a6, a7
sc.ddc	csp, (a7)
andi	zero, zero, 255
lui	tp, 262148
slli	tp, tp, 1
sc.ddc	cgp, (zero)
andi	s2, s2, 255
lui	gp, 262148
slli	gp, gp, 1
sc.ddc	cs2, (s2)
andi	gp, gp, 255
lui	s2, 262148
slli	s2, s2, 1
add	gp, s2, gp
sc.ddc	cra, (gp)
andi	sp, sp, 255
lui	s4, 262148
slli	s4, s4, 1
sc.ddc	csp, (sp)
andi	zero, zero, 255
lui	ra, 262148
slli	ra, ra, 1
sh	zero, 0(csp)
cspecialr	cra, pcc
li	gp, -1025
candperm	cra, cra, gp
cspecialw	mtcc, cra
jalr.cap	cnull
andi	zero, zero, 255
lui	s3, 262148
slli	s3, s3, 1
sc.cap	cs2, (cnull)
cspecialrw	cs2, mscratchc, cs2
jalr.cap	cs2, cnull
cspecialrw	cs3, mscratchc, csp
cspecialw	mscratchc, cs4
cgettag	a7, ca7
andi	a6, a6, 255
lui	tp, 262148
slli	tp, tp, 1
lw	s3, 0(ca6)
cspecialrw	cs3, mtcc, ctp
andi	gp, gp, 255
lui	gp, 262148
slli	gp, gp, 1
add	gp, gp, gp
sc.ddc	ctp, (gp)
andi	a7, a7, 255
lui	a7, 262148
slli	a7, a7, 1
add	a7, a7, a7
sc.ddc	cgp, (a7)
andi	s4, s4, 255
lui	ra, 262148
slli	ra, ra, 1
sc.cap	ctp, (cs4)
andi	s3, s3, 255
lui	sp, 262148
slli	sp, sp, 1
add	s3, sp, s3
sc.ddc	cra, (s3)
andi	sp, sp, 255
lui	a6, 262148
slli	a6, a6, 1
sc.cap	cs2, (csp)
andi	a6, a6, 255
lui	ra, 262148
slli	ra, ra, 1
sc.ddc	ca7, (a6)
andi	sp, sp, 255
lui	zero, 262148
slli	zero, zero, 1
lc.ddc	cs4, (sp)
andi	sp, sp, 255
lui	tp, 262148
slli	tp, tp, 1
sc.ddc	ca6, (sp)
andi	tp, tp, 255
lui	a7, 262148
slli	a7, a7, 1
sc.cap	cra, (ctp)
li	s4, -3
candperm	cs4, cs4, s4
cseal	csp, csp, cs2
cseal	cs4, cs4, cs2
cinvoke	csp, cs4
cmove	ct6, cra
andi	sp, sp, 255
lui	gp, 262148
slli	gp, gp, 1
sc.ddc	cra, (sp)
andi	zero, zero, 255
lui	ra, 262148
slli	ra, ra, 1
sc.ddc	cnull, (zero)
andi	s3, s3, 255
lui	sp, 262148
slli	sp, sp, 1
add	s3, sp, s3
sc.cap	cra, (cs3)
andi	a6, a6, 255
lui	ra, 262148
slli	ra, ra, 1
sc.ddc	cra, (a6)
cgettag	gp, cgp
andi	s3, s3, 255
lui	tp, 262148
slli	tp, tp, 1
add	s3, tp, s3
sc.cap	cgp, (cs3)
andi	gp, gp, 255
lui	gp, 262148
slli	gp, gp, 1
add	gp, gp, gp
sc.ddc	ctp, (gp)
andi	ra, ra, 255
lui	sp, 262148
slli	sp, sp, 1
add	ra, sp, ra
sc.cap	ca6, (cra)
andi	gp, gp, 255
lui	a7, 262148
slli	a7, a7, 1
add	gp, a7, gp
sc.cap	cra, (cgp)
andi	zero, zero, 255
lui	gp, 262148
slli	gp, gp, 1
sc.cap	ca7, (cnull)
andi	sp, sp, 255
lui	zero, 262148
slli	zero, zero, 1
sc.cap	csp, (csp)
li	ra, -3
candperm	cnull, cnull, ra
cseal	cs2, cs2, cs2
cinvoke	cs2, cnull
cmove	ct6, cra
cgettag	s3, cs3
andi	a6, a6, 255
lui	ra, 262148
slli	ra, ra, 1
sc.ddc	cra, (a6)
andi	a7, a7, 255
lui	sp, 262148
slli	sp, sp, 1
add	a7, sp, a7
sc.ddc	csp, (a7)
andi	s2, s2, 255
lui	sp, 262148
slli	sp, sp, 1
sc.ddc	ca7, (s2)
andi	gp, gp, 255
lui	gp, 262148
slli	gp, gp, 1
add	gp, gp, gp
sc.ddc	cs4, (gp)
andi	ra, ra, 255
lui	sp, 262148
slli	sp, sp, 1
add	ra, sp, ra
sc.cap	cs3, (cra)
andi	s3, s3, 255
lui	a7, 262148
slli	a7, a7, 1
add	s3, a7, s3
sc.ddc	ctp, (s3)
csetboundsexact	ctp, ctp, tp
andi	zero, zero, 255
lui	a7, 262148
slli	a7, a7, 1
sc.cap	cgp, (cnull)
andi	s4, s4, 255
lui	tp, 262148
slli	tp, tp, 1
sc.ddc	cs2, (s4)
andi	tp, tp, 255
lui	a7, 262148
slli	a7, a7, 1
sc.cap	cgp, (ctp)
lui	a7, 262148
slli	a7, a7, 1
csetaddr	cs3, cs3, a7
ccleartag	ca7, cnull
sc	cnull, 0(cs3)
cincoffset	cs3, cs3, 16
sc	cnull, 0(cs3)
cincoffset	cs3, cs3, 16
sc	ca7, 0(cs3)
cincoffset	cs3, cs3, 16
sc	cnull, 0(cs3)
cincoffset	cs3, cs3, 16
sc	cnull, 0(cs3)
cincoffset	cs3, cs3, 16
cincoffset	cs3, cs3, -80
cloadtags	a7, (cs3)
andi	tp, tp, 255
lui	ra, 262148
slli	ra, ra, 1
sc.ddc	cs4, (tp)
andi	a7, a7, 255
lui	ra, 262148
slli	ra, ra, 1
add	a7, ra, a7
sc.ddc	csp, (a7)
andi	tp, tp, 255
lui	ra, 262148
slli	ra, ra, 1
sc.cap	cgp, (ctp)
andi	s3, s3, 255
lui	zero, 262148
slli	zero, zero, 1
add	s3, zero, s3
sc.ddc	ca7, (s3)
andi	tp, tp, 255
lui	s3, 262148
slli	s3, s3, 1
sc.cap	csp, (ctp)
cspecialrw	ctp, mscratchc, cra
andi	s4, s4, 255
lui	a7, 262148
slli	a7, a7, 1
sc.ddc	ca7, (s4)
andi	s2, s2, 255
lui	sp, 262148
slli	sp, sp, 1
sc.ddc	ca6, (s2)
andi	sp, sp, 255
lui	ra, 262148
slli	ra, ra, 1
sc.ddc	csp, (sp)
andi	s4, s4, 255
lui	ra, 262148
slli	ra, ra, 1
sc.ddc	ca6, (s4)
li	gp, -3
candperm	ca7, ca7, gp
cseal	ca6, ca6, cs2
cseal	ca7, ca7, cs2
cinvoke	ca6, ca7
cmove	ct6, cra
andi	a6, a6, 255
lui	sp, 262148
slli	sp, sp, 1
sc.ddc	ca7, (a6)
andi	a6, a6, 255
lui	a6, 262148
slli	a6, a6, 1
sc.ddc	cs3, (a6)
andi	tp, tp, 255
lui	s3, 262148
slli	s3, s3, 1
sc.ddc	ctp, (tp)
cspecialrw	csp, mscratchc, cs3
jalr.cap	csp, cnull
cspecialrw	cgp, mscratchc, ca6
cspecialrw	cs4, mscratchc, ca6
andi	sp, sp, 255
lui	s3, 262148
slli	s3, s3, 1
sc.cap	csp, (csp)
andi	ra, ra, 255
lui	tp, 262148
slli	tp, tp, 1
add	ra, tp, ra
sc.cap	cra, (cra)
csetbounds	csp, cs4, 20
li	zero, 1
slli	zero, zero, 12
csetoffset	csp, csp, zero
cincoffset	csp, csp, -184
andi	a6, a6, 255
lui	tp, 262148
slli	tp, tp, 1
sc.ddc	cra, (a6)
cgettag	zero, cnull
andi	s4, s4, 255
lui	ra, 262148
slli	ra, ra, 1
sc.ddc	cgp, (s4)
andi	zero, zero, 255
lui	a6, 262148
slli	a6, a6, 1
sc.cap	cs4, (cnull)
andi	gp, gp, 255
lui	s4, 262148
slli	s4, s4, 1
add	gp, s4, gp
sc.cap	cra, (cgp)
lui	tp, 262148
slli	tp, tp, 1
csetaddr	cs4, cs4, tp
ccleartag	ctp, ca6
sc	ctp, 0(cs4)
cincoffset	cs4, cs4, 16
sc	ctp, 0(cs4)
cincoffset	cs4, cs4, 16
sc	ca6, 0(cs4)
cincoffset	cs4, cs4, 16
sc	ctp, 0(cs4)
cincoffset	cs4, cs4, 16
sc	ca6, 0(cs4)
cincoffset	cs4, cs4, 16
cincoffset	cs4, cs4, -80
cloadtags	a7, (cs4)
cspecialr	ca6, mscratchc
jalr.cap	ca6, cnull
cspecialrw	ca6, mscratchc, cs3
cspecialrw	ca6, mscratchc, ca6
andi	s3, s3, 255
lui	zero, 262148
slli	zero, zero, 1
add	s3, zero, s3
sw	s3, 0(cnull)
andi	a6, a6, 255
lui	s3, 262148
slli	s3, s3, 1
sc.ddc	cnull, (a6)
andi	sp, sp, 255
lui	s2, 262148
slli	s2, s2, 1
sc.cap	csp, (csp)
lui	tp, 262148
slli	tp, tp, 1
csetaddr	cnull, cnull, tp
ccleartag	ctp, cnull
sc	ctp, 0(cnull)
cincoffset	cnull, cnull, 16
sc	ctp, 0(cnull)
cincoffset	cnull, cnull, 16
sc	cnull, 0(cnull)
cincoffset	cnull, cnull, 16
sc	ctp, 0(cnull)
cincoffset	cnull, cnull, 16
sc	cnull, 0(cnull)
cincoffset	cnull, cnull, 16
cincoffset	cnull, cnull, -80
cloadtags	a6, (cnull)
cspecialr	cs10, pcc
li	s11, 1
csetflags	cs10, cs10, s11
cspecialw	mtcc, cs10
jr.cap	cs10
andi	a6, a6, 255
lui	s4, 262148
slli	s4, s4, 1
sc.cap	cs4, (ca6)
andi	s3, s3, 255
lui	sp, 262148
slli	sp, sp, 1
add	s3, sp, s3
sc.ddc	cgp, (s3)
andi	zero, zero, 255
lui	tp, 262148
slli	tp, tp, 1
sc.ddc	cnull, (zero)
andi	s2, s2, 255
lui	a6, 262148
slli	a6, a6, 1
sc.cap	cnull, (cs2)
andi	zero, zero, 255
lui	zero, 262148
slli	zero, zero, 1
sc.ddc	csp, (zero)
andi	zero, zero, 255
lui	tp, 262148
slli	tp, tp, 1
sc.cap	ca7, (cnull)
andi	ra, ra, 255
lui	zero, 262148
slli	zero, zero, 1
add	ra, zero, ra
sc.ddc	cs4, (ra)
cgettag	s4, cs4
andi	a6, a6, 255
lui	gp, 262148
slli	gp, gp, 1
sc.cap	cs3, (ca6)
andi	a6, a6, 255
lui	a7, 262148
slli	a7, a7, 1
sc.cap	cs4, (ca6)
andi	tp, tp, 255
lui	gp, 262148
slli	gp, gp, 1
sc.cap	ca6, (ctp)
andi	ra, ra, 255
lui	a7, 262148
slli	a7, a7, 1
add	ra, a7, ra
sc.cap	ca7, (cra)
lui	s3, 262148
slli	s3, s3, 1
csetaddr	cra, cra, s3
ccleartag	cs3, cnull
sc	cs3, 0(cra)
cincoffset	cra, cra, 16
sc	cs3, 0(cra)
cincoffset	cra, cra, 16
sc	cnull, 0(cra)
cincoffset	cra, cra, 16
sc	cs3, 0(cra)
cincoffset	cra, cra, 16
sc	cnull, 0(cra)
cincoffset	cra, cra, 16
cincoffset	cra, cra, -80
cloadtags	sp, (cra)
andi	a7, a7, 255
lui	s4, 262148
slli	s4, s4, 1
add	a7, s4, a7
sc.cap	cs2, (ca7)
cspecialr	ctp, 2
andi	tp, tp, 255
lui	zero, 262148
slli	zero, zero, 1
sc.cap	cnull, (ctp)
slli	s3, a6, 2
andi	tp, tp, 255
lui	s2, 262148
slli	s2, s2, 1
sc.ddc	ctp, (tp)
cspecialrw	csp, mtcc, cgp
andi	a6, a6, 255
lui	tp, 262148
slli	tp, tp, 1
sc.ddc	cs3, (a6)
cspecialr	ct4, pcc
li	t5, 1
csetflags	ct4, ct4, t5
cspecialw	mtcc, ct4
jr.cap	ct4
andi	gp, gp, 255
lui	a6, 262148
slli	a6, a6, 1
add	gp, a6, gp
sc.cap	ca6, (cgp)
andi	s4, s4, 255
lui	a6, 262148
slli	a6, a6, 1
sc.ddc	cs3, (s4)
andi	ra, ra, 255
lui	ra, 262148
slli	ra, ra, 1
add	ra, ra, ra
sc.ddc	ctp, (ra)
andi	a6, a6, 255
lui	zero, 262148
slli	zero, zero, 1
sc.ddc	cs2, (a6)
sw	s2, -998(ctp)
csetbounds	ctp, csp, 4
li	gp, 0
slli	gp, gp, 12
csetoffset	ctp, ctp, gp
cincoffset	ctp, ctp, -562
andi	tp, tp, 255
lui	s4, 262148
slli	s4, s4, 1
sc.ddc	cs2, (tp)
cgettag	s2, cs2
andi	s4, s4, 255
lui	tp, 262148
slli	tp, tp, 1
sc.cap	cs3, (cs4)
andi	a6, a6, 255
lui	tp, 262148
slli	tp, tp, 1
sc.ddc	cnull, (a6)
cgettag	a6, ca6
andi	a7, a7, 255
lui	zero, 262148
slli	zero, zero, 1
add	a7, zero, a7
sc.ddc	cgp, (a7)
csetbounds	cra, csp, 8
li	tp, 2
slli	tp, tp, 12
csetoffset	cra, cra, tp
cincoffset	cra, cra, 1987
andi	gp, gp, 255
lui	s3, 262148
slli	s3, s3, 1
add	gp, s3, gp
sc.ddc	ctp, (gp)
andi	s4, s4, 255
lui	ra, 262148
slli	ra, ra, 1
sc.ddc	cra, (s4)
cgettag	gp, cgp
andi	sp, sp, 255
lui	s2, 262148
slli	s2, s2, 1
sc.ddc	ca7, (sp)
andi	s2, s2, 255
lui	zero, 262148
slli	zero, zero, 1
sc.ddc	cs4, (s2)
