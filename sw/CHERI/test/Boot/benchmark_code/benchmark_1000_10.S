
   csetbounds	cnull, ca6, 9
li	s2, 3
slli	s2, s2, 12
csetoffset	cnull, cnull, s2
cincoffset	cnull, cnull, 471
andi	ra, ra, 255
lui	a6, 262148
slli	a6, a6, 1
add	ra, a6, ra
sc.ddc	cs4, (ra)
andi	s2, s2, 255
lui	s2, 262148
slli	s2, s2, 1
sc.ddc	cnull, (s2)
cgettag	s2, cs2
andi	a6, a6, 255
lui	ra, 262148
slli	ra, ra, 1
sc.cap	csp, (ca6)
li	s3, -3
candperm	cs2, cs2, s3
cseal	ctp, ctp, cgp
cseal	cs2, cs2, cgp
cinvoke	ctp, cs2
cmove	ct6, cra
cspecialr	cs3, pcc
li	s3, -1025
candperm	cs3, cs3, s3
cspecialw	mtcc, cs3
andi	zero, zero, 255
lui	zero, 262148
slli	zero, zero, 1
sc.cap	cs4, (cnull)
andi	s4, s4, 255
lui	gp, 262148
slli	gp, gp, 1
sc.ddc	ca7, (s4)
andi	a6, a6, 255
lui	s4, 262148
slli	s4, s4, 1
sc.ddc	cnull, (a6)
cincoffset	cs4, cgp, zero
cgettag	tp, ctp
andi	s4, s4, 255
lui	s2, 262148
slli	s2, s2, 1
sc.cap	ctp, (cs4)
andi	s4, s4, 255
lui	gp, 262148
slli	gp, gp, 1
sc.cap	ca6, (cs4)
andi	a7, a7, 255
lui	s4, 262148
slli	s4, s4, 1
add	a7, s4, a7
sc.ddc	csp, (a7)
andi	sp, sp, 255
lui	s4, 262148
slli	s4, s4, 1
sc.cap	cra, (csp)
andi	a6, a6, 255
lui	s4, 262148
slli	s4, s4, 1
lc.ddc	cnull, (a6)
csealentry	cs2, cs4
andi	sp, sp, 255
lui	s2, 262148
slli	s2, s2, 1
sc.cap	cnull, (csp)
andi	sp, sp, 255
lui	s3, 262148
slli	s3, s3, 1
sc.cap	cgp, (csp)
andi	s3, s3, 255
lui	ra, 262148
slli	ra, ra, 1
add	s3, ra, s3
sc.ddc	cs2, (s3)
cspecialrw	ctp, mscratchc, cs4
cspecialrw	cra, mscratchc, cra
cspecialrw	cs4, mscratchc, cs2
andi	sp, sp, 255
lui	s2, 262148
slli	s2, s2, 1
sc.ddc	cs2, (sp)
andi	s2, s2, 255
lui	ra, 262148
slli	ra, ra, 1
sc.cap	ctp, (cs2)
cgettag	a6, ca6
andi	zero, zero, 255
lui	a7, 262148
slli	a7, a7, 1
sc.ddc	cs2, (zero)
cspecialrw	cs3, ddc, cra
li	zero, -3
candperm	cs3, cs3, zero
li	zero, -257
candperm	cs3, cs3, zero
cseal	ca6, ca6, cra
cseal	cs3, cs3, cra
cinvoke	ca6, cs3
cmove	ct6, cra
lui	s2, 262148
slli	s2, s2, 1
csetaddr	cgp, cgp, s2
ccleartag	cs2, csp
sc	cs2, 0(cgp)
cincoffset	cgp, cgp, 16
sc	csp, 0(cgp)
cincoffset	cgp, cgp, 16
sc	csp, 0(cgp)
cincoffset	cgp, cgp, 16
sc	csp, 0(cgp)
cincoffset	cgp, cgp, 16
sc	cs2, 0(cgp)
cincoffset	cgp, cgp, 16
cincoffset	cgp, cgp, -80
cloadtags	sp, (cgp)
andi	zero, zero, 255
lui	tp, 262148
slli	tp, tp, 1
sc.cap	ca7, (cnull)
andi	sp, sp, 255
lui	a7, 262148
slli	a7, a7, 1
lc.ddc	ca7, (sp)
andi	s4, s4, 255
lui	tp, 262148
slli	tp, tp, 1
sh	s4, 0(ctp)
andi	ra, ra, 255
lui	s4, 262148
slli	s4, s4, 1
add	ra, s4, ra
sc.cap	cgp, (cra)
andi	tp, tp, 255
lui	s2, 262148
slli	s2, s2, 1
sc.ddc	ca7, (tp)
andi	s2, s2, 255
lui	tp, 262148
slli	tp, tp, 1
sc.ddc	cra, (s2)
li	a6, -3
candperm	cra, cra, a6
cseal	cs2, cs2, csp
cseal	cra, cra, csp
cinvoke	cs2, cra
cmove	ct6, cra
andi	s2, s2, 255
lui	a7, 262148
slli	a7, a7, 1
sc.cap	ca7, (cs2)
ccopytype	cs4, csp, cs3
cspecialr	cs4, pcc
li	a7, -1025
candperm	cs4, cs4, a7
cspecialw	mtcc, cs4
andi	sp, sp, 255
lui	zero, 262148
slli	zero, zero, 1
sc.cap	cs4, (csp)
andi	tp, tp, 255
lui	tp, 262148
slli	tp, tp, 1
sc.cap	cgp, (ctp)
andi	s3, s3, 255
lui	a6, 262148
slli	a6, a6, 1
add	s3, a6, s3
sc.cap	ctp, (cs3)
andi	s3, s3, 255
lui	s3, 262148
slli	s3, s3, 1
add	s3, s3, s3
sw	s3, 0(ctp)
andi	a6, a6, 255
lui	a6, 262148
slli	a6, a6, 1
sc.ddc	ca6, (a6)
ccleartag	csp, csp
cgettag	s4, cs4
andi	sp, sp, 255
lui	s4, 262148
slli	s4, s4, 1
lw	s3, 0(csp)
cgettag	a6, ca6
andi	sp, sp, 255
lui	s4, 262148
slli	s4, s4, 1
sc.ddc	ca7, (sp)
andi	ra, ra, 255
lui	s4, 262148
slli	s4, s4, 1
add	ra, s4, ra
sc.ddc	cra, (ra)
andi	gp, gp, 255
lui	ra, 262148
slli	ra, ra, 1
add	gp, ra, gp
sh	gp, 0(ca6)
cgettag	zero, cnull
cseal	cs4, cs4, cs2
cseal	cgp, cgp, cs2
cinvoke	cs4, cgp
cmove	ct6, cra
cspecialr	cs8, pcc
li	s9, 1
csetflags	cs8, cs8, s9
cspecialw	mtcc, cs8
cspecialr	ctp, pcc
nop
slli	zero, zero, 11
nop
slli	zero, zero, 11
nop
slli	zero, zero, 11
nop
slli	zero, zero, 11
li	zero, 5
slli	zero, zero, 8
li	zero, 197
cincoffset	ctp, ctp, zero
nop
slli	zero, zero, 11
nop
slli	zero, zero, 11
nop
slli	zero, zero, 11
li	zero, 1
slli	zero, zero, 11
li	zero, 2045
slli	zero, zero, 8
li	zero, 240
csetbounds	ctp, ctp, zero
lh	tp, 930(cgp)
sw	s3, 1440(ctp)
cgettag	tp, ctp
li	s2, -3
candperm	cnull, cnull, s2
cseal	ca6, ca6, cs2
cseal	cnull, cnull, cs2
cinvoke	ca6, cnull
cmove	ct6, cra
lui	zero, 262148
slli	zero, zero, 1
csetaddr	ca7, ca7, zero
ccleartag	cnull, cs4
sc	cnull, 0(ca7)
cincoffset	ca7, ca7, 16
sc	cs4, 0(ca7)
cincoffset	ca7, ca7, 16
sc	cnull, 0(ca7)
cincoffset	ca7, ca7, 16
sc	cnull, 0(ca7)
cincoffset	ca7, ca7, 16
sc	cnull, 0(ca7)
cincoffset	ca7, ca7, 16
cincoffset	ca7, ca7, -80
cloadtags	s4, (ca7)
lui	ra, 262148
slli	ra, ra, 1
csetaddr	cra, cra, ra
ccleartag	cra, ca6
sc	cra, 0(cra)
cincoffset	cra, cra, 16
sc	cra, 0(cra)
cincoffset	cra, cra, 16
sc	ca6, 0(cra)
cincoffset	cra, cra, 16
sc	cra, 0(cra)
cincoffset	cra, cra, 16
sc	ca6, 0(cra)
cincoffset	cra, cra, 16
cincoffset	cra, cra, -80
cloadtags	tp, (cra)
cspecialrw	ca6, mscratchc, ctp
cspecialrw	cs2, mscratchc, cs4
cspecialrw	csp, mscratchc, cs4
sub	gp, ra, ra
lui	sp, 262148
slli	sp, sp, 1
csetaddr	cs3, cs3, sp
ccleartag	csp, csp
sc	csp, 0(cs3)
cincoffset	cs3, cs3, 16
sc	csp, 0(cs3)
cincoffset	cs3, cs3, 16
sc	csp, 0(cs3)
cincoffset	cs3, cs3, 16
sc	csp, 0(cs3)
cincoffset	cs3, cs3, 16
sc	csp, 0(cs3)
cincoffset	cs3, cs3, 16
cincoffset	cs3, cs3, -80
cloadtags	zero, (cs3)
andi	zero, zero, 255
lui	s4, 262148
slli	s4, s4, 1
sc.cap	ctp, (cnull)
andi	gp, gp, 255
lui	sp, 262148
slli	sp, sp, 1
add	gp, sp, gp
sw	gp, 0(cs3)
cspecialrw	cra, mscratchc, ctp
cspecialrw	csp, mscratchc, cs3
cspecialrw	csp, mscratchc, cs4
andi	a6, a6, 255
lui	tp, 262148
slli	tp, tp, 1
sc.ddc	ca7, (a6)
cspecialr	cs3, pcc
li	s2, -1025
candperm	cs3, cs3, s2
cspecialw	mtcc, cs3
li	zero, -3
candperm	ca6, ca6, zero
li	zero, -3
candperm	cgp, cgp, zero
cseal	ca6, ca6, ctp
cinvoke	ca6, cgp
cmove	ct6, cra
andi	a6, a6, 255
lui	s4, 262148
slli	s4, s4, 1
sc.ddc	ca6, (a6)
cspecialrw	ctp, mepcc, ca6
andi	a7, a7, 255
lui	ra, 262148
slli	ra, ra, 1
add	a7, ra, a7
sc.ddc	csp, (a7)
andi	a6, a6, 255
lui	sp, 262148
slli	sp, sp, 1
sc.ddc	ca7, (a6)
csetbounds	cs4, cnull, 9
li	gp, 0
slli	gp, gp, 12
csetoffset	cs4, cs4, gp
cincoffset	cs4, cs4, 8
andi	ra, ra, 255
lui	ra, 262148
slli	ra, ra, 1
add	ra, ra, ra
sc.ddc	cra, (ra)
cspecialrw	cgp, ddc, csp
andi	ra, ra, 255
lui	zero, 262148
slli	zero, zero, 1
add	ra, zero, ra
lhu	sp, 0(cra)
li	a7, -3
candperm	cra, cra, a7
cseal	cra, cra, cs4
cseal	cra, cra, cs4
cinvoke	cra, cra
cmove	ct6, cra
andi	s2, s2, 255
lui	sp, 262148
slli	sp, sp, 1
sc.cap	csp, (cs2)
andi	s2, s2, 255
lui	ra, 262148
slli	ra, ra, 1
sc.ddc	ca6, (s2)
csetbounds	cnull, cra, 20
li	a7, 0
slli	a7, a7, 12
csetoffset	cnull, cnull, a7
cincoffset	cnull, cnull, 8
andi	gp, gp, 255
lui	a6, 262148
slli	a6, a6, 1
add	gp, a6, gp
sc.ddc	ca6, (gp)
cspecialrw	cs4, mscratchc, csp
cgettag	a6, ca6
cspecialrw	cs3, mscratchc, cgp
cspecialrw	cs2, mscratchc, cs2
cspecialrw	ca7, mscratchc, cs4
cgettag	s4, cs4
andi	a7, a7, 255
lui	zero, 262148
slli	zero, zero, 1
add	a7, zero, a7
sc.ddc	ctp, (a7)
andi	a6, a6, 255
lui	a7, 262148
slli	a7, a7, 1
sc.cap	ctp, (ca6)
andi	a7, a7, 255
lui	sp, 262148
slli	sp, sp, 1
add	a7, sp, a7
sc.cap	ca7, (ca7)
lbu.ddc	ra, (a6)
andi	s2, s2, 255
lui	s3, 262148
slli	s3, s3, 1
sc.cap	cs3, (cs2)
cspecialrw	cs4, ddc, cs2
and	ra, tp, s2
csetbounds	cs2, ca6, 10
li	s3, 2
slli	s3, s3, 12
csetoffset	cs2, cs2, s3
cincoffset	cs2, cs2, -2038
andi	ra, ra, 255
lui	sp, 262148
slli	sp, sp, 1
add	ra, sp, ra
sc.ddc	ca7, (ra)
andi	s4, s4, 255
lui	sp, 262148
slli	sp, sp, 1
sc.ddc	cs4, (s4)
cgettag	zero, cnull
cspecialw	mscratchc, ca6
cspecialrw	ca6, mscratchc, ca6
cspecialrw	cs2, mscratchc, cs4
andi	s2, s2, 255
lui	a6, 262148
slli	a6, a6, 1
sc.ddc	cgp, (s2)
andi	a6, a6, 255
lui	s4, 262148
slli	s4, s4, 1
sc.cap	ca7, (ca6)
cgettag	gp, cgp
lui	s4, 262148
slli	s4, s4, 1
csetaddr	cs4, cs4, s4
ccleartag	cs4, cgp
sc	cgp, 0(cs4)
cincoffset	cs4, cs4, 16
sc	cgp, 0(cs4)
cincoffset	cs4, cs4, 16
sc	cgp, 0(cs4)
cincoffset	cs4, cs4, 16
sc	cs4, 0(cs4)
cincoffset	cs4, cs4, 16
sc	cgp, 0(cs4)
cincoffset	cs4, cs4, 16
cincoffset	cs4, cs4, -80
cloadtags	s3, (cs4)
andi	a7, a7, 255
lui	sp, 262148
slli	sp, sp, 1
add	a7, sp, a7
sc.cap	cs4, (ca7)
andi	a7, a7, 255
lui	s2, 262148
slli	s2, s2, 1
add	a7, s2, a7
sc.ddc	ca7, (a7)
andi	gp, gp, 255
lui	s4, 262148
slli	s4, s4, 1
add	gp, s4, gp
sc.ddc	cnull, (gp)
cgettag	s2, cs2
andi	sp, sp, 255
lui	s2, 262148
slli	s2, s2, 1
sc.cap	ca6, (csp)
cspecialrw	ca7, ddc, cs3
cspecialr	ct3, pcc
li	t4, 0
csetflags	ct3, ct3, t4
cspecialw	mtcc, ct3
andi	a7, a7, 255
lui	a7, 262148
slli	a7, a7, 1
add	a7, a7, a7
sc.cap	cs3, (ca7)
lui	s2, 262148
slli	s2, s2, 1
csetaddr	cs3, cs3, s2
ccleartag	cs2, cs2
sc	cs2, 0(cs3)
cincoffset	cs3, cs3, 16
sc	cs2, 0(cs3)
cincoffset	cs3, cs3, 16
sc	cs2, 0(cs3)
cincoffset	cs3, cs3, 16
sc	cs2, 0(cs3)
cincoffset	cs3, cs3, 16
sc	cs2, 0(cs3)
cincoffset	cs3, cs3, 16
cincoffset	cs3, cs3, -80
cloadtags	s2, (cs3)
andi	tp, tp, 255
lui	a7, 262148
slli	a7, a7, 1
sc.cap	cnull, (ctp)
cloadtags	gp, (csp)
andi	zero, zero, 255
lui	s4, 262148
slli	s4, s4, 1
sc.cap	cs2, (cnull)
andi	a7, a7, 255
lui	s2, 262148
slli	s2, s2, 1
add	a7, s2, a7
sc.cap	cgp, (ca7)
andi	a7, a7, 255
lui	zero, 262148
slli	zero, zero, 1
add	a7, zero, a7
sc.ddc	cs2, (a7)
cgettag	ra, cra
andi	zero, zero, 255
lui	zero, 262148
slli	zero, zero, 1
sc.ddc	cnull, (zero)
andi	a6, a6, 255
lui	ra, 262148
slli	ra, ra, 1
sc.cap	cnull, (ca6)
csetbounds	cra, cs3, 26
li	sp, 0
slli	sp, sp, 12
csetoffset	cra, cra, sp
cincoffset	cra, cra, 8
andi	zero, zero, 255
lui	ra, 262148
slli	ra, ra, 1
sc.cap	csp, (cnull)
csetbounds	ca6, cra, 11
li	a6, 0
slli	a6, a6, 12
csetoffset	ca6, ca6, a6
cincoffset	ca6, ca6, 28
andi	tp, tp, 255
lui	a6, 262148
slli	a6, a6, 1
sc.cap	cra, (ctp)
andi	ra, ra, 255
lui	a6, 262148
slli	a6, a6, 1
add	ra, a6, ra
sc.cap	cnull, (cra)
srai	zero, tp, 6
andi	gp, gp, 255
lui	s3, 262148
slli	s3, s3, 1
add	gp, s3, gp
sc.cap	ctp, (cgp)
andi	s2, s2, 255
lui	ra, 262148
slli	ra, ra, 1
lc.ddc	ca6, (s2)
andi	gp, gp, 255
lui	tp, 262148
slli	tp, tp, 1
add	gp, tp, gp
sc.ddc	csp, (gp)
andi	tp, tp, 255
lui	tp, 262148
slli	tp, tp, 1
sc.cap	ctp, (ctp)
cgettag	gp, cgp
cspecialr	cs10, pcc
li	s11, 1
csetflags	cs10, cs10, s11
cspecialw	mtcc, cs10
cspecialrw	cra, mscratchc, ca7
cspecialrw	csp, mscratchc, ctp
cspecialrw	cra, mscratchc, cra
cspecialrw	cs2, mscratchc, cs3
cspecialrw	cra, mscratchc, ca7
cspecialrw	csp, mscratchc, cs4
cgettag	gp, cgp
cgettag	s2, cs2
andi	ra, ra, 255
lui	s4, 262148
slli	s4, s4, 1
add	ra, s4, ra
sc.cap	cs2, (cra)
andi	gp, gp, 255
lui	a7, 262148
slli	a7, a7, 1
add	gp, a7, gp
sc.cap	cs4, (cgp)
andi	zero, zero, 255
lui	s3, 262148
slli	s3, s3, 1
sc.cap	cs4, (cnull)
andi	zero, zero, 255
lui	a6, 262148
slli	a6, a6, 1
sc.ddc	ca7, (zero)
andi	sp, sp, 255
lui	tp, 262148
slli	tp, tp, 1
sc.cap	cra, (csp)
andi	s3, s3, 255
lui	s3, 262148
slli	s3, s3, 1
add	s3, s3, s3
sw	s3, 0(cnull)
csetbounds	ca7, cra, 22
li	s3, 0
slli	s3, s3, 12
csetoffset	ca7, ca7, s3
cincoffset	ca7, ca7, 1
cspecialr	ca6, pcc
li	s3, 0
slli	s3, s3, 11
mv	s3, s3
slli	s3, s3, 11
mv	s3, s3
slli	s3, s3, 11
mv	s3, s3
slli	s3, s3, 11
addi	s3, s3, 5
slli	s3, s3, 8
addi	s3, s3, 249
cincoffset	ca6, ca6, s3
li	s3, 0
slli	s3, s3, 11
mv	s3, s3
slli	s3, s3, 11
mv	s3, s3
slli	s3, s3, 11
mv	s3, s3
slli	s3, s3, 11
addi	s3, s3, 1761
slli	s3, s3, 8
addi	s3, s3, 65
csetbounds	ca6, ca6, s3
cspecialr	cs2, mscratchc
cspecialrw	cgp, mscratchc, ctp
cspecialrw	cs3, mscratchc, ca6
lui	s3, 262148
slli	s3, s3, 1
csetaddr	ca7, ca7, s3
ccleartag	cs3, csp
sc	cs3, 0(ca7)
cincoffset	ca7, ca7, 16
sc	csp, 0(ca7)
cincoffset	ca7, ca7, 16
sc	csp, 0(ca7)
cincoffset	ca7, ca7, 16
sc	cs3, 0(ca7)
cincoffset	ca7, ca7, 16
sc	csp, 0(ca7)
cincoffset	ca7, ca7, 16
cincoffset	ca7, ca7, -80
cloadtags	s3, (ca7)
cgettag	a7, ca7
cgettag	ra, cra
andi	a7, a7, 255
lui	a7, 262148
slli	a7, a7, 1
add	a7, a7, a7
sc.cap	ca6, (ca7)
andi	tp, tp, 255
lui	s2, 262148
slli	s2, s2, 1
sc.ddc	ctp, (tp)
cgettag	ra, cra
andi	s2, s2, 255
lui	a7, 262148
slli	a7, a7, 1
lb	a6, 0(cs2)
cspecialr	cs4, pcc
li	s4, 0
slli	s4, s4, 11
mv	s4, s4
slli	s4, s4, 11
mv	s4, s4
slli	s4, s4, 11
mv	s4, s4
slli	s4, s4, 11
addi	s4, s4, 11
slli	s4, s4, 8
addi	s4, s4, 17
cincoffset	cs4, cs4, s4
li	s4, 0
slli	s4, s4, 11
mv	s4, s4
slli	s4, s4, 11
mv	s4, s4
slli	s4, s4, 11
addi	s4, s4, 1
slli	s4, s4, 11
addi	s4, s4, 1410
slli	s4, s4, 8
addi	s4, s4, 37
csetbounds	cs4, cs4, s4
andi	zero, zero, 255
lui	gp, 262148
slli	gp, gp, 1
sc.ddc	cgp, (zero)
andi	s3, s3, 255
lui	tp, 262148
slli	tp, tp, 1
add	s3, tp, s3
sc.cap	cnull, (cs3)
csetbounds	ca7, ctp, 28
li	s2, 0
slli	s2, s2, 12
csetoffset	ca7, ca7, s2
cincoffset	ca7, ca7, 3
cspecialw	pcc, csp
andi	tp, tp, 255
lui	a7, 262148
slli	a7, a7, 1
sc.cap	ctp, (ctp)
andi	zero, zero, 255
lui	tp, 262148
slli	tp, tp, 1
sc.cap	cgp, (cnull)
andi	tp, tp, 255
lui	a7, 262148
slli	a7, a7, 1
sc.cap	cra, (ctp)
andi	zero, zero, 255
lui	s4, 262148
slli	s4, s4, 1
sc.cap	cnull, (cnull)
cgettag	tp, ctp
andi	ra, ra, 255
lui	a6, 262148
slli	a6, a6, 1
add	ra, a6, ra
sc.cap	csp, (cra)
cspecialrw	ca7, pcc, cs4
andi	tp, tp, 255
lui	gp, 262148
slli	gp, gp, 1
sc.cap	ctp, (ctp)
cseal	ca6, ca6, ca7
cseal	cs2, cs2, ca7
cinvoke	ca6, cs2
cmove	ct6, cra
cspecialrw	ca6, mtdc, ca6
cspecialr	cs11, pcc
li	t3, 0
csetflags	cs11, cs11, t3
cspecialw	mtcc, cs11
andi	gp, gp, 255
lui	s2, 262148
slli	s2, s2, 1
add	gp, s2, gp
sc.cap	cra, (cgp)
cspecialr	ca6, pcc
li	ra, -1025
candperm	ca6, ca6, ra
cspecialw	mtcc, ca6
cspecialr	cra, pcc
li	gp, -1025
candperm	cra, cra, gp
cspecialw	mtcc, cra
cgettag	gp, cgp
cgettag	gp, cgp
andi	s4, s4, 255
lui	tp, 262148
slli	tp, tp, 1
sc.cap	ctp, (cs4)
# ebreak
li	zero, -3
candperm	ca6, ca6, zero
cseal	csp, csp, cnull
cseal	ca6, ca6, cnull
cinvoke	csp, ca6
cmove	ct6, cra
addi	zero, tp, -3
candperm	cra, cra, tp
li	tp, -3
candperm	csp, csp, tp
cseal	cra, cra, ctp
cseal	csp, csp, ctp
cinvoke	cra, csp
cmove	ct6, cra
andi	s4, s4, 255
lui	gp, 262148
slli	gp, gp, 1
sc.ddc	cs3, (s4)
andi	gp, gp, 255
lui	ra, 262148
slli	ra, ra, 1
add	gp, ra, gp
sc.ddc	ctp, (gp)
cgettag	s3, cs3
cgettag	a7, ca7
andi	zero, zero, 255
lui	a6, 262148
slli	a6, a6, 1
sc.ddc	cs2, (zero)
andi	ra, ra, 255
lui	sp, 262148
slli	sp, sp, 1
add	ra, sp, ra
sc.ddc	cs4, (ra)
lui	s2, 262148
slli	s2, s2, 1
csetaddr	cra, cra, s2
ccleartag	cs2, cra
sc	cs2, 0(cra)
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
cloadtags	sp, (cra)
csetbounds	cgp, ctp, ra
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
addi	sp, sp, 81
cincoffset	ca6, ca6, sp
li	sp, 0
slli	sp, sp, 11
mv	sp, sp
slli	sp, sp, 11
mv	sp, sp
slli	sp, sp, 11
addi	sp, sp, 1
slli	sp, sp, 11
addi	sp, sp, 142
slli	sp, sp, 8
addi	sp, sp, 180
csetbounds	ca6, ca6, sp
andi	s4, s4, 255
lui	a6, 262148
slli	a6, a6, 1
sc.cap	cra, (cs4)
li	sp, -3
candperm	ca6, ca6, sp
cseal	cgp, cgp, ctp
cseal	ca6, ca6, ctp
cinvoke	cgp, ca6
cmove	ct6, cra
cgettag	ra, cra
andi	s2, s2, 255
lui	gp, 262148
slli	gp, gp, 1
sc.cap	cgp, (cs2)
cgettag	ra, cra
andi	s3, s3, 255
lui	zero, 262148
slli	zero, zero, 1
add	s3, zero, s3
sc.ddc	cs2, (s3)
cspecialr	cs9, pcc
li	s10, 0
csetflags	cs9, cs9, s10
cspecialw	mtcc, cs9
cspecialrw	cs3, mepcc, cs3
lui	a6, 262148
slli	a6, a6, 1
csetaddr	cgp, cgp, a6
ccleartag	ca6, cs3
sc	cs3, 0(cgp)
cincoffset	cgp, cgp, 16
sc	ca6, 0(cgp)
cincoffset	cgp, cgp, 16
sc	cs3, 0(cgp)
cincoffset	cgp, cgp, 16
sc	ca6, 0(cgp)
cincoffset	cgp, cgp, 16
sc	ca6, 0(cgp)
cincoffset	cgp, cgp, 16
cincoffset	cgp, cgp, -80
cloadtags	zero, (cgp)
andi	s2, s2, 255
lui	ra, 262148
slli	ra, ra, 1
sc.cap	cs2, (cs2)
andi	s4, s4, 255
lui	ra, 262148
slli	ra, ra, 1
sc.cap	ctp, (cs4)
cspecialr	ca6, pcc
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
addi	tp, tp, 133
cincoffset	ca6, ca6, tp
li	tp, 0
slli	tp, tp, 11
mv	tp, tp
slli	tp, tp, 11
mv	tp, tp
slli	tp, tp, 11
addi	tp, tp, 1
slli	tp, tp, 11
addi	tp, tp, 1336
slli	tp, tp, 8
addi	tp, tp, 84
csetbounds	ca6, ca6, tp
andi	sp, sp, 255
lui	a7, 262148
slli	a7, a7, 1
sc.cap	cs2, (csp)
andi	ra, ra, 255
lui	sp, 262148
slli	sp, sp, 1
add	ra, sp, ra
sc.ddc	cra, (ra)
andi	tp, tp, 255
lui	zero, 262148
slli	zero, zero, 1
sc.ddc	cs2, (tp)
cspecialrw	cra, mscratchc, ca7
cspecialrw	ca7, mscratchc, cs2
cspecialr	csp, mscratchc
andi	sp, sp, 255
lui	ra, 262148
slli	ra, ra, 1
sc.cap	cnull, (csp)
andi	sp, sp, 255
lui	sp, 262148
slli	sp, sp, 1
sc.cap	ca7, (csp)
andi	a7, a7, 255
lui	s2, 262148
slli	s2, s2, 1
add	a7, s2, a7
sc.cap	ca7, (ca7)
andi	gp, gp, 255
lui	ra, 262148
slli	ra, ra, 1
add	gp, ra, gp
sc.ddc	ctp, (gp)
andi	tp, tp, 255
lui	sp, 262148
slli	sp, sp, 1
sc.cap	cra, (ctp)
andi	sp, sp, 255
lui	zero, 262148
slli	zero, zero, 1
sc.cap	cs4, (csp)
andi	s3, s3, 255
lui	s4, 262148
slli	s4, s4, 1
add	s3, s4, s3
sc.cap	cnull, (cs3)
lui	ra, 262148
slli	ra, ra, 1
csetaddr	cgp, cgp, ra
ccleartag	cra, ca6
sc	ca6, 0(cgp)
cincoffset	cgp, cgp, 16
sc	cra, 0(cgp)
cincoffset	cgp, cgp, 16
sc	ca6, 0(cgp)
cincoffset	cgp, cgp, 16
sc	cra, 0(cgp)
cincoffset	cgp, cgp, 16
sc	cra, 0(cgp)
cincoffset	cgp, cgp, 16
cincoffset	cgp, cgp, -80
cloadtags	a6, (cgp)
andi	s3, s3, 255
lui	s4, 262148
slli	s4, s4, 1
add	s3, s4, s3
sc.ddc	cnull, (s3)
li	ra, -3
candperm	cs2, cs2, ra
cseal	cs3, cs3, cgp
cinvoke	cs3, cs2
cmove	ct6, cra
cspecialrw	cs3, mscratchc, ca7
cspecialrw	ctp, mscratchc, cs4
cspecialw	mscratchc, cs4
sh	zero, 847(ctp)
csetbounds	cs2, cs4, 1
li	s2, 3
slli	s2, s2, 12
csetoffset	cs2, cs2, s2
cincoffset	cs2, cs2, 1308
andi	s3, s3, 255
lui	s3, 262148
slli	s3, s3, 1
add	s3, s3, s3
sc.ddc	cnull, (s3)
li	ra, -3
candperm	cs2, cs2, ra
li	ra, -257
candperm	cs2, cs2, ra
cseal	cs2, cs2, cs2
cseal	cs2, cs2, cs2
cinvoke	cs2, cs2
cmove	ct6, cra


