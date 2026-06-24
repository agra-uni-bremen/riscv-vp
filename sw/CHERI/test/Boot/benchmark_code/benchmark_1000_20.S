andi	s4, s4, 255
lui	zero, 262148
slli	zero, zero, 1
sc.ddc	ca7, (s4)
andi	s3, s3, 255
lui	zero, 262148
slli	zero, zero, 1
add	s3, zero, s3
sc.ddc	csp, (s3)
andi	sp, sp, 255
lui	zero, 262148
slli	zero, zero, 1
sc.ddc	csp, (sp)
andi	tp, tp, 255
lui	a7, 262148
slli	a7, a7, 1
sc.ddc	cnull, (tp)
andi	a6, a6, 255
lui	ra, 262148
slli	ra, ra, 1
sc.ddc	cra, (a6)
andi	a6, a6, 255
lui	zero, 262148
slli	zero, zero, 1
sc.cap	cgp, (ca6)
andi	a7, a7, 255
lui	ra, 262148
slli	ra, ra, 1
add	a7, ra, a7
sc.cap	cs3, (ca7)
cspecialrw	csp, mscratchc, cgp
jalr.cap	csp, cnull
cspecialrw	cs4, mscratchc, cs3
cspecialrw	ctp, mscratchc, cs2
andi	zero, zero, 255
lui	a7, 262148
slli	a7, a7, 1
sc.cap	ca6, (cnull)
lui	s3, 262148
slli	s3, s3, 1
csetaddr	cs3, cs3, s3
ccleartag	cs3, cgp
sc	cgp, 0(cs3)
cincoffset	cs3, cs3, 16
sc	cgp, 0(cs3)
cincoffset	cs3, cs3, 16
sc	cs3, 0(cs3)
cincoffset	cs3, cs3, 16
sc	cs3, 0(cs3)
cincoffset	cs3, cs3, 16
sc	cgp, 0(cs3)
cincoffset	cs3, cs3, 16
cincoffset	cs3, cs3, -80
cloadtags	s3, (cs3)
cspecialrw	ctp, pcc, cs4
andi	a7, a7, 255
lui	s4, 262148
slli	s4, s4, 1
add	a7, s4, a7
sc.ddc	csp, (a7)
andi	tp, tp, 255
lui	a6, 262148
slli	a6, a6, 1
sc.ddc	csp, (tp)
andi	s2, s2, 255
lui	ra, 262148
slli	ra, ra, 1
sc.cap	cs4, (cs2)
cspecialr	cs9, pcc
li	s10, 1
csetflags	cs9, cs9, s10
cspecialw	mtcc, cs9
jr.cap	cs9
andi	s2, s2, 255
lui	s3, 262148
slli	s3, s3, 1
sc.ddc	ca7, (s2)
andi	ra, ra, 255
lui	s2, 262148
slli	s2, s2, 1
add	ra, s2, ra
sc.cap	cs2, (cra)
andi	sp, sp, 255
lui	s4, 262148
slli	s4, s4, 1
sc.ddc	cs3, (sp)
andi	gp, gp, 255
lui	zero, 262148
slli	zero, zero, 1
add	gp, zero, gp
sc.cap	cs3, (cgp)
andi	s2, s2, 255
lui	s4, 262148
slli	s4, s4, 1
sc.cap	cra, (cs2)
andi	gp, gp, 255
lui	gp, 262148
slli	gp, gp, 1
add	gp, gp, gp
sc.cap	cnull, (cgp)
cspecialr	cs3, pcc
li	s2, 0
slli	s2, s2, 11
mv	s2, s2
slli	s2, s2, 11
mv	s2, s2
slli	s2, s2, 11
mv	s2, s2
slli	s2, s2, 11
addi	s2, s2, 7
slli	s2, s2, 8
addi	s2, s2, 37
cincoffset	cs3, cs3, s2
li	s2, 0
slli	s2, s2, 11
mv	s2, s2
slli	s2, s2, 11
mv	s2, s2
slli	s2, s2, 11
addi	s2, s2, 1
slli	s2, s2, 11
addi	s2, s2, 162
slli	s2, s2, 8
addi	s2, s2, 24
csetbounds	cs3, cs3, s2
jalr.cap	cs3, cs3
andi	s3, s3, 255
lui	sp, 262148
slli	sp, sp, 1
add	s3, sp, s3
sc.ddc	cra, (s3)
andi	s4, s4, 255
lui	zero, 262148
slli	zero, zero, 1
sc.cap	cnull, (cs4)
andi	a6, a6, 255
lui	tp, 262148
slli	tp, tp, 1
sc.ddc	cgp, (a6)
andi	gp, gp, 255
lui	s4, 262148
slli	s4, s4, 1
add	gp, s4, gp
sc.ddc	cs3, (gp)
andi	a6, a6, 255
lui	a6, 262148
slli	a6, a6, 1
sc.cap	ca6, (ca6)
andi	ra, ra, 255
lui	zero, 262148
slli	zero, zero, 1
add	ra, zero, ra
sc.cap	ca6, (cra)
cspecialrw	cra, pcc, cs4
crrl	s2, s4
li	s2, -3
candperm	cgp, cgp, s2
cseal	ctp, ctp, ctp
cinvoke	ctp, cgp
cmove	ct6, cra
andi	a6, a6, 255
lui	a7, 262148
slli	a7, a7, 1
sc.cap	cgp, (ca6)
andi	s3, s3, 255
lui	s3, 262148
slli	s3, s3, 1
add	s3, s3, s3
sc.cap	ca7, (cs3)
andi	s3, s3, 255
lui	s3, 262148
slli	s3, s3, 1
add	s3, s3, s3
sc.ddc	ca7, (s3)
andi	s3, s3, 255
lui	zero, 262148
slli	zero, zero, 1
add	s3, zero, s3
sc.cap	cs4, (cs3)
andi	a7, a7, 255
lui	ra, 262148
slli	ra, ra, 1
add	a7, ra, a7
sc.cap	cra, (ca7)
cinvoke	cs3, ca7
andi	s4, s4, 255
lui	s2, 262148
slli	s2, s2, 1
sc.ddc	cs4, (s4)
andi	zero, zero, 255
lui	gp, 262148
slli	gp, gp, 1
sc.ddc	cs2, (zero)
andi	a6, a6, 255
lui	s3, 262148
slli	s3, s3, 1
sc.cap	cs2, (ca6)
andi	a7, a7, 255
lui	ra, 262148
slli	ra, ra, 1
add	a7, ra, a7
sc.ddc	cs3, (a7)
andi	a7, a7, 255
lui	s3, 262148
slli	s3, s3, 1
add	a7, s3, a7
sc.ddc	csp, (a7)
andi	a7, a7, 255
lui	s4, 262148
slli	s4, s4, 1
add	a7, s4, a7
sc.ddc	ca6, (a7)
andi	zero, zero, 255
lui	a7, 262148
slli	a7, a7, 1
sc.ddc	cra, (zero)
andi	s3, s3, 255
lui	sp, 262148
slli	sp, sp, 1
add	s3, sp, s3
sc.cap	cs3, (cs3)
andi	ra, ra, 255
lui	a6, 262148
slli	a6, a6, 1
add	ra, a6, ra
sc.cap	cgp, (cra)
andi	s3, s3, 255
lui	zero, 262148
slli	zero, zero, 1
add	s3, zero, s3
sc.cap	cs3, (cs3)
andi	gp, gp, 255
lui	zero, 262148
slli	zero, zero, 1
add	gp, zero, gp
sc.cap	ca6, (cgp)
andi	zero, zero, 255
lui	gp, 262148
slli	gp, gp, 1
sc.ddc	ca7, (zero)
andi	s4, s4, 255
lui	tp, 262148
slli	tp, tp, 1
sc.cap	ctp, (cs4)
andi	sp, sp, 255
lui	gp, 262148
slli	gp, gp, 1
sc.cap	ca7, (csp)
andi	a6, a6, 255
lui	zero, 262148
slli	zero, zero, 1
sc.cap	csp, (ca6)
andi	gp, gp, 255
lui	ra, 262148
slli	ra, ra, 1
add	gp, ra, gp
sc.ddc	cgp, (gp)
andi	a6, a6, 255
lui	zero, 262148
slli	zero, zero, 1
sc.ddc	cs4, (a6)
andi	tp, tp, 255
lui	ra, 262148
slli	ra, ra, 1
sc.cap	csp, (ctp)
andi	s4, s4, 255
lui	sp, 262148
slli	sp, sp, 1
sc.cap	cs3, (cs4)
andi	zero, zero, 255
lui	a7, 262148
slli	a7, a7, 1
sc.ddc	ca7, (zero)
andi	a6, a6, 255
lui	s2, 262148
slli	s2, s2, 1
sc.ddc	cnull, (a6)
andi	s2, s2, 255
lui	s4, 262148
slli	s4, s4, 1
sc.ddc	cs4, (s2)
andi	s3, s3, 255
lui	zero, 262148
slli	zero, zero, 1
add	s3, zero, s3
sc.cap	ca7, (cs3)
andi	s2, s2, 255
lui	sp, 262148
slli	sp, sp, 1
sc.cap	cs4, (cs2)
andi	tp, tp, 255
lui	s3, 262148
slli	s3, s3, 1
sc.cap	cnull, (ctp)
cspecialrw	cgp, mscratchc, cgp
jalr.cap	cgp, cnull
cspecialrw	ctp, mscratchc, csp
cspecialrw	cra, mscratchc, cgp
andi	sp, sp, 255
lui	s2, 262148
slli	s2, s2, 1
sc.ddc	cnull, (sp)
andi	s4, s4, 255
lui	tp, 262148
slli	tp, tp, 1
sc.cap	cs2, (cs4)
andi	s3, s3, 255
lui	s3, 262148
slli	s3, s3, 1
add	s3, s3, s3
sc.ddc	csp, (s3)
andi	tp, tp, 255
lui	a6, 262148
slli	a6, a6, 1
sc.cap	cs3, (ctp)
andi	s4, s4, 255
lui	s3, 262148
slli	s3, s3, 1
sh	s4, 0(cnull)
andi	a6, a6, 255
lui	gp, 262148
slli	gp, gp, 1
sc.ddc	cs2, (a6)
andi	a6, a6, 255
lui	a6, 262148
slli	a6, a6, 1
sc.cap	csp, (ca6)
andi	sp, sp, 255
lui	gp, 262148
slli	gp, gp, 1
sc.cap	cgp, (csp)
andi	a6, a6, 255
lui	ra, 262148
slli	ra, ra, 1
lc.ddc	ca6, (a6)
andi	s2, s2, 255
lui	a6, 262148
slli	a6, a6, 1
sc.ddc	cnull, (s2)
andi	sp, sp, 255
lui	s2, 262148
slli	s2, s2, 1
sc.cap	cs2, (csp)
andi	s4, s4, 255
lui	s3, 262148
slli	s3, s3, 1
sc.cap	ca7, (cs4)
andi	tp, tp, 255
lui	a7, 262148
slli	a7, a7, 1
sc.cap	csp, (ctp)
andi	a7, a7, 255
lui	a7, 262148
slli	a7, a7, 1
add	a7, a7, a7
sc.cap	cs3, (ca7)
andi	s4, s4, 255
lui	tp, 262148
slli	tp, tp, 1
sc.cap	ctp, (cs4)
andi	s4, s4, 255
lui	s4, 262148
slli	s4, s4, 1
sc.ddc	cgp, (s4)
cspecialr	cs11, pcc
li	t3, 0
csetflags	cs11, cs11, t3
cspecialw	mtcc, cs11
jr.cap	cs11
andi	s4, s4, 255
lui	ra, 262148
slli	ra, ra, 1
sc.ddc	cs4, (s4)
andi	a6, a6, 255
lui	ra, 262148
slli	ra, ra, 1
sc.cap	ca7, (ca6)
andi	s4, s4, 255
lui	zero, 262148
slli	zero, zero, 1
sc.ddc	ca7, (s4)
andi	ra, ra, 255
lui	a7, 262148
slli	a7, a7, 1
add	ra, a7, ra
sc.cap	cra, (cra)
andi	ra, ra, 255
lui	a6, 262148
slli	a6, a6, 1
add	ra, a6, ra
sc.ddc	ca7, (ra)
cgettag	tp, ctp
andi	ra, ra, 255
lui	a7, 262148
slli	a7, a7, 1
add	ra, a7, ra
sc.cap	cs3, (cra)
andi	s2, s2, 255
lui	a7, 262148
slli	a7, a7, 1
sc.cap	cs3, (cs2)
andi	s2, s2, 255
lui	sp, 262148
slli	sp, sp, 1
sc.ddc	cs2, (s2)
andi	zero, zero, 255
lui	a6, 262148
slli	a6, a6, 1
sc.ddc	csp, (zero)
andi	ra, ra, 255
lui	s4, 262148
slli	s4, s4, 1
add	ra, s4, ra
sc.cap	cnull, (cra)
andi	s3, s3, 255
lui	sp, 262148
slli	sp, sp, 1
add	s3, sp, s3
sc.cap	csp, (cs3)
andi	gp, gp, 255
lui	a6, 262148
slli	a6, a6, 1
add	gp, a6, gp
sc.cap	cs3, (cgp)
andi	s2, s2, 255
lui	zero, 262148
slli	zero, zero, 1
sc.ddc	cs3, (s2)
andi	sp, sp, 255
lui	zero, 262148
slli	zero, zero, 1
sc.ddc	ca6, (sp)
andi	zero, zero, 255
lui	gp, 262148
slli	gp, gp, 1
sc.cap	cnull, (cnull)
andi	ra, ra, 255
lui	s3, 262148
slli	s3, s3, 1
add	ra, s3, ra
sc.ddc	cgp, (ra)
cspecialrw	csp, mscratchc, csp
jalr.cap	csp, cnull
cspecialrw	ca6, mscratchc, cs4
cspecialrw	cs2, mscratchc, csp
andi	a7, a7, 255
lui	a6, 262148
slli	a6, a6, 1
add	a7, a6, a7
sc.ddc	cnull, (a7)
andi	tp, tp, 255
lui	a7, 262148
slli	a7, a7, 1
sc.ddc	ctp, (tp)
andi	sp, sp, 255
lui	s4, 262148
slli	s4, s4, 1
sc.ddc	cs2, (sp)
andi	s4, s4, 255
lui	a6, 262148
slli	a6, a6, 1
sc.cap	cra, (cs4)
cgettag	zero, cnull
andi	a7, a7, 255
lui	gp, 262148
slli	gp, gp, 1
add	a7, gp, a7
sc.ddc	cs3, (a7)
andi	a6, a6, 255
lui	gp, 262148
slli	gp, gp, 1
sc.cap	csp, (ca6)
andi	s2, s2, 255
lui	s4, 262148
slli	s4, s4, 1
sc.cap	ca7, (cs2)
andi	ra, ra, 255
lui	a6, 262148
slli	a6, a6, 1
add	ra, a6, ra
sc.ddc	cra, (ra)
andi	a6, a6, 255
lui	gp, 262148
slli	gp, gp, 1
sc.ddc	cra, (a6)
andi	s4, s4, 255
lui	s4, 262148
slli	s4, s4, 1
sc.cap	ctp, (cs4)
andi	gp, gp, 255
lui	zero, 262148
slli	zero, zero, 1
add	gp, zero, gp
sc.cap	ca6, (cgp)
andi	s3, s3, 255
lui	a7, 262148
slli	a7, a7, 1
add	s3, a7, s3
sc.cap	cs4, (cs3)
andi	tp, tp, 255
lui	s3, 262148
slli	s3, s3, 1
sc.ddc	ca6, (tp)
andi	s4, s4, 255
lui	gp, 262148
slli	gp, gp, 1
sc.cap	cra, (cs4)
andi	s3, s3, 255
lui	gp, 262148
slli	gp, gp, 1
add	s3, gp, s3
sb	s3, 0(ca7)
andi	sp, sp, 255
lui	s3, 262148
slli	s3, s3, 1
sc.cap	ca6, (csp)
andi	sp, sp, 255
lui	zero, 262148
slli	zero, zero, 1
sc.cap	csp, (csp)
andi	s3, s3, 255
lui	tp, 262148
slli	tp, tp, 1
add	s3, tp, s3
sc.cap	ca6, (cs3)
andi	s2, s2, 255
lui	a6, 262148
slli	a6, a6, 1
sc.ddc	ca7, (s2)
andi	s4, s4, 255
lui	gp, 262148
slli	gp, gp, 1
sc.cap	cra, (cs4)
andi	ra, ra, 255
lui	a7, 262148
slli	a7, a7, 1
add	ra, a7, ra
sc.cap	cs4, (cra)
cgethigh	zero, ca6
andi	gp, gp, 255
lui	a6, 262148
slli	a6, a6, 1
add	gp, a6, gp
sc.ddc	ca7, (gp)
andi	s2, s2, 255
lui	a7, 262148
slli	a7, a7, 1
sc.cap	cgp, (cs2)
lui	a6, 262148
slli	a6, a6, 1
csetaddr	ca7, ca7, a6
ccleartag	ca6, csp
sc	ca6, 0(ca7)
cincoffset	ca7, ca7, 16
sc	csp, 0(ca7)
cincoffset	ca7, ca7, 16
sc	ca6, 0(ca7)
cincoffset	ca7, ca7, 16
sc	csp, 0(ca7)
cincoffset	ca7, ca7, 16
sc	csp, 0(ca7)
cincoffset	ca7, ca7, 16
cincoffset	ca7, ca7, -80
cloadtags	a6, (ca7)
andi	gp, gp, 255
lui	gp, 262148
slli	gp, gp, 1
add	gp, gp, gp
sc.cap	ca6, (cgp)
andi	s3, s3, 255
lui	sp, 262148
slli	sp, sp, 1
add	s3, sp, s3
sc.cap	cra, (cs3)
andi	tp, tp, 255
lui	a6, 262148
slli	a6, a6, 1
sc.cap	ca6, (ctp)
andi	tp, tp, 255
lui	a6, 262148
slli	a6, a6, 1
sc.cap	cs2, (ctp)
andi	gp, gp, 255
lui	gp, 262148
slli	gp, gp, 1
add	gp, gp, gp
sc.cap	cra, (cgp)
andi	s2, s2, 255
lui	gp, 262148
slli	gp, gp, 1
sc.cap	ca7, (cs2)
andi	s4, s4, 255
lui	a6, 262148
slli	a6, a6, 1
sc.cap	ca6, (cs4)
andi	s4, s4, 255
lui	s3, 262148
slli	s3, s3, 1
sc.ddc	cgp, (s4)
andi	ra, ra, 255
lui	tp, 262148
slli	tp, tp, 1
add	ra, tp, ra
sc.ddc	cs4, (ra)
andi	gp, gp, 255
lui	zero, 262148
slli	zero, zero, 1
add	gp, zero, gp
sc.ddc	cra, (gp)
andi	ra, ra, 255
lui	s3, 262148
slli	s3, s3, 1
add	ra, s3, ra
sc.ddc	cs3, (ra)
andi	s4, s4, 255
lui	sp, 262148
slli	sp, sp, 1
sc.ddc	ca7, (s4)
andi	s4, s4, 255
lui	s3, 262148
slli	s3, s3, 1
sc.ddc	cs4, (s4)
cgetoffset	a6, csp
andi	ra, ra, 255
lui	gp, 262148
slli	gp, gp, 1
add	ra, gp, ra
sc.cap	csp, (cra)
andi	ra, ra, 255
lui	s4, 262148
slli	s4, s4, 1
add	ra, s4, ra
sc.ddc	cs3, (ra)
andi	s3, s3, 255
lui	ra, 262148
slli	ra, ra, 1
add	s3, ra, s3
sc.ddc	ctp, (s3)
andi	ra, ra, 255
lui	a7, 262148
slli	a7, a7, 1
add	ra, a7, ra
sc.ddc	cra, (ra)
andi	a6, a6, 255
lui	s4, 262148
slli	s4, s4, 1
sc.ddc	ca7, (a6)
andi	gp, gp, 255
lui	sp, 262148
slli	sp, sp, 1
add	gp, sp, gp
sc.ddc	ca7, (gp)
cspecialr	ct4, pcc
li	t5, 0
csetflags	ct4, ct4, t5
cspecialw	mtcc, ct4
jr.cap	ct4
andi	gp, gp, 255
lui	zero, 262148
slli	zero, zero, 1
add	gp, zero, gp
sc.ddc	cnull, (gp)
andi	gp, gp, 255
lui	a7, 262148
slli	a7, a7, 1
add	gp, a7, gp
sc.cap	cnull, (cgp)
andi	gp, gp, 255
lui	tp, 262148
slli	tp, tp, 1
add	gp, tp, gp
lb	zero, 0(cgp)
andi	gp, gp, 255
lui	zero, 262148
slli	zero, zero, 1
add	gp, zero, gp
sc.cap	cra, (cgp)
andi	gp, gp, 255
lui	sp, 262148
slli	sp, sp, 1
add	gp, sp, gp
sc.cap	cra, (cgp)
andi	a6, a6, 255
lui	ra, 262148
slli	ra, ra, 1
sc.cap	csp, (ca6)
andi	a6, a6, 255
lui	gp, 262148
slli	gp, gp, 1
sc.ddc	cra, (a6)
andi	s3, s3, 255
lui	a6, 262148
slli	a6, a6, 1
add	s3, a6, s3
sc.ddc	cra, (s3)
andi	a7, a7, 255
lui	gp, 262148
slli	gp, gp, 1
add	a7, gp, a7
sc.cap	cs4, (ca7)
andi	s2, s2, 255
lui	ra, 262148
slli	ra, ra, 1
sc.cap	cs2, (cs2)
andi	s3, s3, 255
lui	a6, 262148
slli	a6, a6, 1
add	s3, a6, s3
sc.ddc	cs2, (s3)
andi	zero, zero, 255
lui	sp, 262148
slli	sp, sp, 1
sc.ddc	cs3, (zero)
andi	a7, a7, 255
lui	ra, 262148
slli	ra, ra, 1
add	a7, ra, a7
sc.ddc	ctp, (a7)
andi	a7, a7, 255
lui	tp, 262148
slli	tp, tp, 1
add	a7, tp, a7
sc.cap	csp, (ca7)
andi	tp, tp, 255
lui	zero, 262148
slli	zero, zero, 1
sc.ddc	ca6, (tp)
andi	gp, gp, 255
lui	s3, 262148
slli	s3, s3, 1
add	gp, s3, gp
sc.ddc	ca6, (gp)
lui	zero, 262148
slli	zero, zero, 1
csetaddr	cs4, cs4, zero
ccleartag	cnull, cs2
sc	cnull, 0(cs4)
cincoffset	cs4, cs4, 16
sc	cs2, 0(cs4)
cincoffset	cs4, cs4, 16
sc	cs2, 0(cs4)
cincoffset	cs4, cs4, 16
sc	cnull, 0(cs4)
cincoffset	cs4, cs4, 16
sc	cnull, 0(cs4)
cincoffset	cs4, cs4, 16
cincoffset	cs4, cs4, -80
cloadtags	s4, (cs4)
lui	sp, 262148
slli	sp, sp, 1
csetaddr	cnull, cnull, sp
ccleartag	csp, cgp
sc	csp, 0(cnull)
cincoffset	cnull, cnull, 16
sc	csp, 0(cnull)
cincoffset	cnull, cnull, 16
sc	csp, 0(cnull)
cincoffset	cnull, cnull, 16
sc	cgp, 0(cnull)
cincoffset	cnull, cnull, 16
sc	csp, 0(cnull)
cincoffset	cnull, cnull, 16
cincoffset	cnull, cnull, -80
cloadtags	zero, (cnull)
andi	ra, ra, 255
lui	zero, 262148
slli	zero, zero, 1
add	ra, zero, ra
sc.ddc	cs2, (ra)
andi	s4, s4, 255
lui	s2, 262148
slli	s2, s2, 1
sc.ddc	cs3, (s4)
andi	sp, sp, 255
lui	a7, 262148
slli	a7, a7, 1
sc.cap	csp, (csp)
cgettag	s2, cs2
andi	zero, zero, 255
lui	s4, 262148
slli	s4, s4, 1
sc.cap	cra, (cnull)
andi	ra, ra, 255
lui	s4, 262148
slli	s4, s4, 1
add	ra, s4, ra
sc.cap	cra, (cra)
andi	tp, tp, 255
lui	s2, 262148
slli	s2, s2, 1
sc.cap	cs4, (ctp)
andi	a6, a6, 255
lui	a7, 262148
slli	a7, a7, 1
sc.cap	ctp, (ca6)
andi	s3, s3, 255
lui	a6, 262148
slli	a6, a6, 1
add	s3, a6, s3
sc.cap	cs4, (cs3)
andi	sp, sp, 255
lui	gp, 262148
slli	gp, gp, 1
sc.ddc	cs4, (sp)
andi	a7, a7, 255
lui	sp, 262148
slli	sp, sp, 1
add	a7, sp, a7
sc.cap	cgp, (ca7)
andi	a7, a7, 255
lui	s2, 262148
slli	s2, s2, 1
add	a7, s2, a7
sc.cap	cra, (ca7)
andi	ra, ra, 255
lui	s4, 262148
slli	s4, s4, 1
add	ra, s4, ra
sc.cap	ca7, (cra)
andi	s3, s3, 255
lui	tp, 262148
slli	tp, tp, 1
add	s3, tp, s3
sc.cap	cra, (cs3)
andi	s2, s2, 255
lui	a7, 262148
slli	a7, a7, 1
sc.cap	ca7, (cs2)
andi	gp, gp, 255
lui	zero, 262148
slli	zero, zero, 1
add	gp, zero, gp
sc.ddc	cnull, (gp)
andi	s2, s2, 255
lui	gp, 262148
slli	gp, gp, 1
sc.ddc	cgp, (s2)
andi	s4, s4, 255
lui	s2, 262148
slli	s2, s2, 1
sc.cap	cgp, (cs4)
andi	gp, gp, 255
lui	gp, 262148
slli	gp, gp, 1
add	gp, gp, gp
sc.ddc	ca6, (gp)
andi	gp, gp, 255
lui	a7, 262148
slli	a7, a7, 1
add	gp, a7, gp
sc.cap	ctp, (cgp)
andi	a7, a7, 255
lui	tp, 262148
slli	tp, tp, 1
add	a7, tp, a7
sc.cap	cnull, (ca7)
lui	zero, 262148
slli	zero, zero, 1
csetaddr	ctp, ctp, zero
ccleartag	cnull, cs3
sc	cnull, 0(ctp)
cincoffset	ctp, ctp, 16
sc	cnull, 0(ctp)
cincoffset	ctp, ctp, 16
sc	cnull, 0(ctp)
cincoffset	ctp, ctp, 16
sc	cnull, 0(ctp)
cincoffset	ctp, ctp, 16
sc	cs3, 0(ctp)
cincoffset	ctp, ctp, 16
cincoffset	ctp, ctp, -80
cloadtags	ra, (ctp)
andi	zero, zero, 255
lui	a6, 262148
slli	a6, a6, 1
sc.cap	csp, (cnull)
andi	s4, s4, 255
lui	s4, 262148
slli	s4, s4, 1
sc.ddc	cnull, (s4)
cgettag	tp, ctp
andi	s4, s4, 255
lui	sp, 262148
slli	sp, sp, 1
sc.ddc	cnull, (s4)
andi	a7, a7, 255
lui	ra, 262148
slli	ra, ra, 1
add	a7, ra, a7
sc.cap	cs4, (ca7)
andi	s2, s2, 255
lui	tp, 262148
slli	tp, tp, 1
sc.ddc	ca6, (s2)
andi	s2, s2, 255
lui	gp, 262148
slli	gp, gp, 1
sc.ddc	cgp, (s2)
andi	gp, gp, 255
lui	s3, 262148
slli	s3, s3, 1
add	gp, s3, gp
sc.cap	ctp, (cgp)
andi	a6, a6, 255
lui	s4, 262148
slli	s4, s4, 1
sc.cap	cs3, (ca6)
lui	s2, 262148
slli	s2, s2, 1
csetaddr	cra, cra, s2
ccleartag	cs2, cra
sc	cra, 0(cra)
cincoffset	cra, cra, 16
sc	cra, 0(cra)
cincoffset	cra, cra, 16
sc	cs2, 0(cra)
cincoffset	cra, cra, 16
sc	cra, 0(cra)
cincoffset	cra, cra, 16
sc	cs2, 0(cra)
cincoffset	cra, cra, 16
cincoffset	cra, cra, -80
cloadtags	zero, (cra)
andi	gp, gp, 255
lui	a7, 262148
slli	a7, a7, 1
add	gp, a7, gp
sc.cap	ca7, (cgp)
andi	s4, s4, 255
lui	a7, 262148
slli	a7, a7, 1
sc.ddc	cra, (s4)
andi	s2, s2, 255
lui	a7, 262148
slli	a7, a7, 1
sc.ddc	cra, (s2)
andi	s3, s3, 255
lui	s2, 262148
slli	s2, s2, 1
add	s3, s2, s3
sc.cap	ctp, (cs3)
andi	s4, s4, 255
lui	ra, 262148
slli	ra, ra, 1
sc.cap	cgp, (cs4)
andi	zero, zero, 255
lui	sp, 262148
slli	sp, sp, 1
sc.cap	ca7, (cnull)
andi	a6, a6, 255
lui	tp, 262148
slli	tp, tp, 1
sc.cap	cs3, (ca6)
andi	s3, s3, 255
lui	zero, 262148
slli	zero, zero, 1
add	s3, zero, s3
sc.ddc	ca7, (s3)
andi	ra, ra, 255
lui	ra, 262148
slli	ra, ra, 1
add	ra, ra, ra
sc.ddc	ctp, (ra)
andi	ra, ra, 255
lui	ra, 262148
slli	ra, ra, 1
add	ra, ra, ra
sc.cap	cnull, (cra)
andi	s4, s4, 255
lui	gp, 262148
slli	gp, gp, 1
sc.cap	cra, (cs4)
andi	s2, s2, 255
lui	s4, 262148
slli	s4, s4, 1
sc.cap	cs2, (cs2)
andi	sp, sp, 255
lui	zero, 262148
slli	zero, zero, 1
sc.ddc	csp, (sp)
andi	a6, a6, 255
lui	a7, 262148
slli	a7, a7, 1
sc.ddc	cgp, (a6)
andi	tp, tp, 255
lui	s2, 262148
slli	s2, s2, 1
lh	sp, 0(ctp)
cspecialr	ctp, pcc
li	a7, 0
slli	a7, a7, 11
mv	a7, a7
slli	a7, a7, 11
mv	a7, a7
slli	a7, a7, 11
mv	a7, a7
slli	a7, a7, 11
mv	a7, a7
slli	a7, a7, 8
addi	a7, a7, 211
cincoffset	ctp, ctp, a7
li	a7, 0
slli	a7, a7, 11
mv	a7, a7
slli	a7, a7, 11
mv	a7, a7
slli	a7, a7, 11
addi	a7, a7, 1
slli	a7, a7, 11
addi	a7, a7, 265
slli	a7, a7, 8
addi	a7, a7, 91
csetbounds	ctp, ctp, a7
jalr.cap	ctp, ctp
