andi	a7, a7, 255
lui	a6, 262148
slli	a6, a6, 1
add	a7, a6, a7
sc.ddc	cs3, (a7)
csetbounds	csp, cra, 14
li	s3, 0
slli	s3, s3, 12
csetoffset	csp, csp, s3
cincoffset	csp, csp, 30
cspecialr	cs2, pcc
li	a6, 0
slli	a6, a6, 11
mv	a6, a6
slli	a6, a6, 11
mv	a6, a6
slli	a6, a6, 11
mv	a6, a6
slli	a6, a6, 11
addi	a6, a6, 8
slli	a6, a6, 8
addi	a6, a6, 243
cincoffset	cs2, cs2, a6
li	a6, 0
slli	a6, a6, 11
mv	a6, a6
slli	a6, a6, 11
mv	a6, a6
slli	a6, a6, 11
addi	a6, a6, 1
slli	a6, a6, 11
addi	a6, a6, 428
slli	a6, a6, 8
addi	a6, a6, 188
csetbounds	cs2, cs2, a6
jalr.cap	cs2, cs2
andi	gp, gp, 255
lui	ra, 262148
slli	ra, ra, 1
add	gp, ra, gp
sc.cap	ca7, (cgp)
andi	s4, s4, 255
lui	s2, 262148
slli	s2, s2, 1
sw	s4, 0(ctp)
andi	s3, s3, 255
lui	ra, 262148
slli	ra, ra, 1
add	s3, ra, s3
sc.cap	ctp, (cs3)
andi	s3, s3, 255
lui	ra, 262148
slli	ra, ra, 1
add	s3, ra, s3
sc.cap	cs4, (cs3)
andi	zero, zero, 255
lui	s4, 262148
slli	s4, s4, 1
lb	gp, 0(cnull)
cspecialr	ct4, pcc
li	t5, 1
csetflags	ct4, ct4, t5
cspecialw	mtcc, ct4
jr.cap	ct4
andi	s3, s3, 255
lui	gp, 262148
slli	gp, gp, 1
add	s3, gp, s3
lc.ddc	cnull, (s3)
cspecialrw	ca7, mscratchc, ctp
jalr.cap	ca7, cnull
cspecialrw	ca6, mscratchc, cgp
cspecialrw	csp, mscratchc, ca7
csetbounds	cs2, csp, 26
li	tp, 1
slli	tp, tp, 12
csetoffset	cs2, cs2, tp
cincoffset	cs2, cs2, 1886
cspecialrw	cs4, 2, csp
lui	zero, 262148
slli	zero, zero, 1
csetaddr	cgp, cgp, zero
ccleartag	cnull, cra
sc	cnull, 0(cgp)
cincoffset	cgp, cgp, 16
sc	cnull, 0(cgp)
cincoffset	cgp, cgp, 16
sc	cnull, 0(cgp)
cincoffset	cgp, cgp, 16
sc	cnull, 0(cgp)
cincoffset	cgp, cgp, 16
sc	cra, 0(cgp)
cincoffset	cgp, cgp, 16
cincoffset	cgp, cgp, -80
cloadtags	sp, (cgp)
andi	zero, zero, 255
lui	a6, 262148
slli	a6, a6, 1
sc.cap	cs2, (cnull)
li	a7, -3
candperm	cgp, cgp, a7
cseal	cgp, cgp, ca7
cinvoke	csp, cgp
cmove	ct6, cra
andi	ra, ra, 255
lui	s2, 262148
slli	s2, s2, 1
add	ra, s2, ra
lb	a6, 0(cra)
csetbounds	ca6, csp, 31
li	s4, 0
slli	s4, s4, 12
csetoffset	ca6, ca6, s4
cincoffset	ca6, ca6, 15
csetbounds	cs2, cnull, 29
li	s4, 1
slli	s4, s4, 12
csetoffset	cs2, cs2, s4
cincoffset	cs2, cs2, 1124
csetbounds	ctp, ca6, 7
li	ra, 1
slli	ra, ra, 12
csetoffset	ctp, ctp, ra
cincoffset	ctp, ctp, -867
andi	gp, gp, 255
lui	gp, 262148
slli	gp, gp, 1
add	gp, gp, gp
sc.cap	cs4, (cgp)
lui	ra, 262148
slli	ra, ra, 1
csetaddr	cs2, cs2, ra
ccleartag	cra, csp
sc	csp, 0(cs2)
cincoffset	cs2, cs2, 16
sc	cra, 0(cs2)
cincoffset	cs2, cs2, 16
sc	csp, 0(cs2)
cincoffset	cs2, cs2, 16
sc	cra, 0(cs2)
cincoffset	cs2, cs2, 16
sc	csp, 0(cs2)
cincoffset	cs2, cs2, 16
cincoffset	cs2, cs2, -80
cloadtags	s3, (cs2)
andi	tp, tp, 255
lui	a6, 262148
slli	a6, a6, 1
sc.ddc	cs3, (tp)
bgeu	s4, s2, 0x11e2e <.text+0xd5a>
cgettag	s3, cs3
lui	a7, 262148
slli	a7, a7, 1
csetaddr	ca7, ca7, a7
ccleartag	ca7, cgp
sc	cgp, 0(ca7)
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
cloadtags	tp, (ca7)
cspecialr	cgp, pcc
li	sp, -1025
candperm	cgp, cgp, sp
cspecialw	mtcc, cgp
jalr.cap	cgp, cnull
cgettag	sp, csp
cspecialr	cnull, pcc
li	s2, -1025
candperm	cnull, cnull, s2
cspecialr	cnull, mtcc
jr.cap	cnull
cspecialrw	cs3, mtdc, cs4
cspecialr	ca6, 2
cgettag	sp, csp
andi	s2, s2, 255
lui	s4, 262148
slli	s4, s4, 1
sc.cap	cs3, (cs2)
cspecialr	csp, pcc
li	s2, -1025
candperm	csp, csp, s2
cspecialw	mtcc, csp
jalr.cap	csp, cnull
li	s4, -3
candperm	cgp, cgp, s4
cseal	cgp, cgp, cgp
cseal	cgp, cgp, cgp
cinvoke	cgp, cgp
cmove	ct6, cra
cgettag	zero, cnull
andi	zero, zero, 255
lui	a6, 262148
slli	a6, a6, 1
sc.ddc	ca6, (zero)
cloadtags	zero, (ca6)
cspecialr	csp, mscratchc
jalr.cap	csp, cnull
cspecialrw	ca6, mscratchc, cgp
cspecialw	mscratchc, cs3
andi	s4, s4, 255
lui	s4, 262148
slli	s4, s4, 1
lc.ddc	cnull, (s4)
lui	s2, 262148
slli	s2, s2, 1
csetaddr	cnull, cnull, s2
ccleartag	cs2, ctp
sc	ctp, 0(cnull)
cincoffset	cnull, cnull, 16
sc	cs2, 0(cnull)
cincoffset	cnull, cnull, 16
sc	cs2, 0(cnull)
cincoffset	cnull, cnull, 16
sc	cs2, 0(cnull)
cincoffset	cnull, cnull, 16
sc	ctp, 0(cnull)
cincoffset	cnull, cnull, 16
cincoffset	cnull, cnull, -80
cloadtags	s3, (cnull)
andi	ra, ra, 255
lui	a6, 262148
slli	a6, a6, 1
add	ra, a6, ra
lc.ddc	cs2, (ra)
li	a7, -3
candperm	ca7, ca7, a7
cseal	csp, csp, ctp
cseal	ca7, ca7, ctp
cinvoke	csp, ca7
cmove	ct6, cra
andi	s2, s2, 255
lui	sp, 262148
slli	sp, sp, 1
lb	s3, 0(cs2)
cgettag	gp, cgp
cgettag	s4, cs4
cspecialrw	cs3, mscratchc, csp
jalr.cap	cs3, cnull
cspecialrw	cra, mscratchc, cgp
cspecialrw	ca7, mscratchc, cra
cgetbase	a6, cs3
andi	a7, a7, 255
lui	gp, 262148
slli	gp, gp, 1
add	a7, gp, a7
lw	a6, 0(ca7)
cbuildcap	cs2, cs3, cs4
andi	s2, s2, 255
lui	s2, 262148
slli	s2, s2, 1
sc.ddc	ca7, (s2)
andi	zero, zero, 255
lui	s3, 262148
slli	s3, s3, 1
lh	ra, 0(cnull)
lui	tp, 262148
slli	tp, tp, 1
csetaddr	cs4, cs4, tp
ccleartag	ctp, ca7
sc	ctp, 0(cs4)
cincoffset	cs4, cs4, 16
sc	ctp, 0(cs4)
cincoffset	cs4, cs4, 16
sc	ctp, 0(cs4)
cincoffset	cs4, cs4, 16
sc	ca7, 0(cs4)
cincoffset	cs4, cs4, 16
sc	ctp, 0(cs4)
cincoffset	cs4, cs4, 16
cincoffset	cs4, cs4, -80
cloadtags	s3, (cs4)
ctestsubset	ra, cgp, cs4
cspecialr	ct3, pcc
li	t4, 0
csetflags	ct3, ct3, t4
cspecialw	mtcc, ct3
jr.cap	ct3
andi	s3, s3, 255
lui	sp, 262148
slli	sp, sp, 1
add	s3, sp, s3
sw	s3, 0(cs4)
cspecialr	cs6, pcc
li	s7, 0
csetflags	cs6, cs6, s7
cspecialw	mtcc, cs6
jr.cap	cs6
cspecialr	csp, pcc
li	sp, -1025
candperm	csp, csp, sp
cspecialw	mtcc, csp
jalr.cap	csp, cnull
andi	sp, sp, 255
lui	s4, 262148
slli	s4, s4, 1
sc.ddc	cs2, (sp)
csetbounds	cnull, cs4, 20
li	ra, 0
slli	ra, ra, 12
csetoffset	cnull, cnull, ra
cincoffset	cnull, cnull, 821
andi	tp, tp, 255
lui	ra, 262148
slli	ra, ra, 1
sc.ddc	cgp, (tp)
csetbounds	cra, ca7, 15
li	tp, 1
slli	tp, tp, 12
csetoffset	cra, cra, tp
cincoffset	cra, cra, -1097
lui	s3, 262148
slli	s3, s3, 1
csetaddr	csp, csp, s3
ccleartag	cs3, ca7
sc	ca7, 0(csp)
cincoffset	csp, csp, 16
sc	cs3, 0(csp)
cincoffset	csp, csp, 16
sc	cs3, 0(csp)
cincoffset	csp, csp, 16
sc	ca7, 0(csp)
cincoffset	csp, csp, 16
sc	cs3, 0(csp)
cincoffset	csp, csp, 16
cincoffset	csp, csp, -80
cloadtags	sp, (csp)
cspecialr	cs4, pcc
li	a6, -1025
candperm	cs4, cs4, a6
cspecialw	mtcc, cs4
jalr.cap	cs4, cnull
cspecialrw	cs4, ddc, cs4
cgettag	sp, csp
andi	s3, s3, 255
lui	s3, 262148
slli	s3, s3, 1
add	s3, s3, s3
sc.ddc	ctp, (s3)
cgettag	tp, ctp
bgtz	s3, 0x11d16 <.text+0xc42>
li	ra, -3
candperm	csp, csp, ra
cseal	cs4, cs4, ca7
cseal	csp, csp, ca7
cinvoke	cs4, csp
cmove	ct6, cra
cspecialrw	ca6, mscratchc, ca6
jalr.cap	ca6, cnull
cspecialw	mscratchc, cra
cspecialrw	csp, mscratchc, ctp
cgettag	ra, cra
cspecialrw	cs3, mscratchc, ca6
jalr.cap	cs3, cnull
cspecialw	mscratchc, cs2
cspecialrw	ctp, mscratchc, cs2
andi	a6, a6, 255
lui	gp, 262148
slli	gp, gp, 1
sc.cap	ca7, (ca6)
cgettag	a6, ca6
cspecialrw	cs3, mscratchc, ca6
jalr.cap	cs3, cnull
cspecialrw	ca6, mscratchc, cra
cspecialrw	cs2, mscratchc, csp
andi	zero, zero, 255
lui	tp, 262148
slli	tp, tp, 1
lh	zero, 0(cnull)
cspecialw	mepcc, cs3
andi	a6, a6, 255
lui	s3, 262148
slli	s3, s3, 1
sc.cap	csp, (ca6)
cspecialrw	ca6, mtdc, cs2
andi	sp, sp, 255
lui	s3, 262148
slli	s3, s3, 1
sc.ddc	cnull, (sp)
andi	s2, s2, 255
lui	a6, 262148
slli	a6, a6, 1
sc.cap	ca6, (cs2)
li	a7, -3
candperm	ctp, ctp, a7
cseal	ctp, ctp, ctp
cinvoke	cs3, ctp
cmove	ct6, cra
cgetbase	sp, csp
lui	zero, 262148
slli	zero, zero, 1
csetaddr	csp, csp, zero
ccleartag	cnull, cs2
sc	cs2, 0(csp)
cincoffset	csp, csp, 16
sc	cs2, 0(csp)
cincoffset	csp, csp, 16
sc	cnull, 0(csp)
cincoffset	csp, csp, 16
sc	cs2, 0(csp)
cincoffset	csp, csp, 16
sc	cs2, 0(csp)
cincoffset	csp, csp, 16
cincoffset	csp, csp, -80
cloadtags	ra, (csp)
andi	zero, zero, 255
lui	s3, 262148
slli	s3, s3, 1
sc.cap	cs4, (cnull)
andi	gp, gp, 255
lui	zero, 262148
slli	zero, zero, 1
add	gp, zero, gp
sc.ddc	cs3, (gp)
csetbounds	cnull, cnull, 24
li	tp, 3
slli	tp, tp, 12
csetoffset	cnull, cnull, tp
cincoffset	cnull, cnull, -1701
andi	sp, sp, 255
lui	ra, 262148
slli	ra, ra, 1
sc.ddc	cs3, (sp)
csetbounds	ca7, cs2, 30
li	a7, 0
slli	a7, a7, 12
csetoffset	ca7, ca7, a7
cincoffset	ca7, ca7, 9
cspecialr	cs6, pcc
li	s7, 0
csetflags	cs6, cs6, s7
cspecialw	mtcc, cs6
jr.cap	cs6
cgettag	tp, ctp
cspecialr	cs7, pcc
li	s8, 0
csetflags	cs7, cs7, s8
cspecialw	mtcc, cs7
jr.cap	cs7
andi	s2, s2, 255
lui	tp, 262148
slli	tp, tp, 1
lbu	gp, 0(cs2)
cgettag	s3, cs3
ccseal	cs4, ca7, ctp
cspecialrw	ca6, mscratchc, cs2
jalr.cap	ca6, cnull
cspecialw	mscratchc, cgp
cspecialrw	csp, mscratchc, cra
cspecialr	cra, pcc
li	sp, -1025
candperm	cra, cra, sp
cspecialw	mtcc, cra
jalr.cap	cnull
cgettag	a7, ca7
cspecialr	ct3, pcc
li	t4, 1
csetflags	ct3, ct3, t4
cspecialw	mtcc, ct3
jr.cap	ct3
andi	tp, tp, 255
lui	s4, 262148
slli	s4, s4, 1
lc.ddc	cs4, (tp)
andi	tp, tp, 255
lui	s3, 262148
slli	s3, s3, 1
sc.ddc	cgp, (tp)
andi	tp, tp, 255
lui	gp, 262148
slli	gp, gp, 1
sc.ddc	ca6, (tp)
csetbounds	csp, cgp, 15
li	a7, 0
slli	a7, a7, 12
csetoffset	csp, csp, a7
cincoffset	csp, csp, 26
lui	tp, 262148
slli	tp, tp, 1
csetaddr	cra, cra, tp
ccleartag	ctp, cgp
sc	ctp, 0(cra)
cincoffset	cra, cra, 16
sc	cgp, 0(cra)
cincoffset	cra, cra, 16
sc	ctp, 0(cra)
cincoffset	cra, cra, 16
sc	ctp, 0(cra)
cincoffset	cra, cra, 16
sc	ctp, 0(cra)
cincoffset	cra, cra, 16
cincoffset	cra, cra, -80
cloadtags	s3, (cra)
lui	zero, 262148
slli	zero, zero, 1
csetaddr	csp, csp, zero
ccleartag	cnull, ca7
sc	cnull, 0(csp)
cincoffset	csp, csp, 16
sc	ca7, 0(csp)
cincoffset	csp, csp, 16
sc	ca7, 0(csp)
cincoffset	csp, csp, 16
sc	cnull, 0(csp)
cincoffset	csp, csp, 16
sc	ca7, 0(csp)
cincoffset	csp, csp, 16
cincoffset	csp, csp, -80
cloadtags	zero, (csp)
andi	zero, zero, 255
lui	a7, 262148
slli	a7, a7, 1
sc.ddc	cs4, (zero)
andi	s3, s3, 255
lui	s2, 262148
slli	s2, s2, 1
add	s3, s2, s3
sc.ddc	cra, (s3)
cspecialrw	cs2, mscratchc, cgp
jalr.cap	cs2, cnull
cspecialrw	cgp, mscratchc, cs3
cspecialw	mscratchc, cra
csetbounds	csp, csp, 8
li	tp, 2
slli	tp, tp, 12
csetoffset	csp, csp, tp
cincoffset	csp, csp, 1836
andi	gp, gp, 255
lui	ra, 262148
slli	ra, ra, 1
add	gp, ra, gp
sc.cap	cs4, (cgp)
andi	ra, ra, 255
lui	ra, 262148
slli	ra, ra, 1
add	ra, ra, ra
lc.ddc	ctp, (ra)
csetbounds	cgp, ca7, 22
li	tp, 0
slli	tp, tp, 12
csetoffset	cgp, cgp, tp
cincoffset	cgp, cgp, 30
cspecialrw	cs2, mtcc, cgp
andi	a6, a6, 255
lui	ra, 262148
slli	ra, ra, 1
sw	a6, 0(cs2)
cseal	cs3, cs3, cs4
cseal	cra, cra, cs4
cinvoke	cs3, cra
cmove	ct6, cra
andi	sp, sp, 255
lui	sp, 262148
slli	sp, sp, 1
lw	tp, 0(csp)
andi	tp, tp, 255
lui	a6, 262148
slli	a6, a6, 1
sb	tp, 0(ctp)
cspecialr	cs10, pcc
li	s11, 0
csetflags	cs10, cs10, s11
cspecialw	mtcc, cs10
jr.cap	cs10
andi	ra, ra, 255
lui	gp, 262148
slli	gp, gp, 1
add	ra, gp, ra
sw	ra, 0(cs4)
cspecialrw	cgp, 2, ca6
csetbounds	ca7, cra, 0
li	s4, 0
slli	s4, s4, 12
csetoffset	ca7, ca7, s4
cincoffset	ca7, ca7, 16
lui	ra, 262148
slli	ra, ra, 1
csetaddr	cs4, cs4, ra
ccleartag	cra, cgp
sc	cgp, 0(cs4)
cincoffset	cs4, cs4, 16
sc	cra, 0(cs4)
cincoffset	cs4, cs4, 16
sc	cgp, 0(cs4)
cincoffset	cs4, cs4, 16
sc	cgp, 0(cs4)
cincoffset	cs4, cs4, 16
sc	cgp, 0(cs4)
cincoffset	cs4, cs4, 16
cincoffset	cs4, cs4, -80
cloadtags	s3, (cs4)
cspecialrw	cs2, mtdc, cs2
cgettag	tp, ctp
andi	tp, tp, 255
lui	sp, 262148
slli	sp, sp, 1
lb	tp, 0(ctp)
cgettag	ra, cra
andi	a6, a6, 255
lui	ra, 262148
slli	ra, ra, 1
lb	a6, 0(ca6)
cgettag	s3, cs3
andi	s4, s4, 255
lui	ra, 262148
slli	ra, ra, 1
sc.cap	ctp, (cs4)
andi	a6, a6, 255
lui	a7, 262148
slli	a7, a7, 1
sh	a6, 0(cnull)
andi	gp, gp, 255
lui	a7, 262148
slli	a7, a7, 1
add	gp, a7, gp
sc.ddc	cs2, (gp)
li	sp, -3
candperm	cnull, cnull, sp
cseal	cs4, cs4, cra
cinvoke	cs4, cnull
cmove	ct6, cra
ctestsubset	gp, ctp, cs3
cspecialw	2, csp
andi	gp, gp, 255
lui	s2, 262148
slli	s2, s2, 1
add	gp, s2, gp
sc.cap	cs2, (cgp)
li	a7, -3
candperm	cs2, cs2, a7
li	a7, -257
candperm	cs2, cs2, a7
cseal	cra, cra, cnull
cseal	cs2, cs2, cnull
cinvoke	cra, cs2
cmove	ct6, cra
li	tp, -3
candperm	cs3, cs3, tp
cseal	cs3, cs3, cra
cinvoke	cs3, cs3
cmove	ct6, cra
cspecialr	cs7, pcc
li	s8, 0
csetflags	cs7, cs7, s8
cspecialw	mtcc, cs7
jr.cap	cs7
andi	gp, gp, 255
lui	gp, 262148
slli	gp, gp, 1
add	gp, gp, gp
sc.ddc	cra, (gp)
cspecialr	cgp, pcc
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
addi	s2, s2, 20
cincoffset	cgp, cgp, s2
li	s2, 0
slli	s2, s2, 11
mv	s2, s2
slli	s2, s2, 11
mv	s2, s2
slli	s2, s2, 11
mv	s2, s2
slli	s2, s2, 11
addi	s2, s2, 1486
slli	s2, s2, 8
addi	s2, s2, 161
csetbounds	cgp, cgp, s2
jalr.cap	cgp, cgp
cspecialr	cs9, pcc
li	s10, 1
csetflags	cs9, cs9, s10
cspecialw	mtcc, cs9
jr.cap	cs9
lui	zero, 262148
slli	zero, zero, 1
csetaddr	cgp, cgp, zero
ccleartag	cnull, cs3
sc	cnull, 0(cgp)
cincoffset	cgp, cgp, 16
sc	cs3, 0(cgp)
cincoffset	cgp, cgp, 16
sc	cnull, 0(cgp)
cincoffset	cgp, cgp, 16
sc	cs3, 0(cgp)
cincoffset	cgp, cgp, 16
sc	cs3, 0(cgp)
cincoffset	cgp, cgp, 16
cincoffset	cgp, cgp, -80
cloadtags	a7, (cgp)
cspecialr	cs7, pcc
li	s8, 0
csetflags	cs7, cs7, s8
cspecialw	mtcc, cs7
jr.cap	cs7
cspecialrw	csp, pcc, csp
cspecialw	mscratchc, ctp
jr.cap	cnull
cspecialrw	csp, mscratchc, csp
cspecialr	cgp, mscratchc
andi	sp, sp, 255
lui	ra, 262148
slli	ra, ra, 1
lc.ddc	csp, (sp)
andi	sp, sp, 255
lui	ra, 262148
slli	ra, ra, 1
sc.cap	cs2, (csp)
andi	s4, s4, 255
lui	s3, 262148
slli	s3, s3, 1
sw	s4, 0(ctp)
cgettag	s4, cs4
andi	s2, s2, 255
lui	zero, 262148
slli	zero, zero, 1
sc.cap	cnull, (cs2)
cspecialr	cnull, pcc
li	s2, -1025
candperm	cnull, cnull, s2
cspecialr	cnull, mtcc
jr.cap	cnull
sw	s3, 723(cra)
cspecialr	ca7, pcc
li	a6, -1025
candperm	ca7, ca7, a6
cspecialw	mtcc, ca7
jalr.cap	ca7, cnull
li	a7, -3
candperm	cs2, cs2, a7
cseal	ctp, ctp, cnull
cseal	cs2, cs2, cnull
cinvoke	ctp, cs2
cmove	ct6, cra
andi	ra, ra, 255
lui	gp, 262148
slli	gp, gp, 1
add	ra, gp, ra
sc.ddc	ctp, (ra)
lui	tp, 262148
slli	tp, tp, 1
csetaddr	cs3, cs3, tp
ccleartag	ctp, cs3
sc	ctp, 0(cs3)
cincoffset	cs3, cs3, 16
sc	ctp, 0(cs3)
cincoffset	cs3, cs3, 16
sc	ctp, 0(cs3)
cincoffset	cs3, cs3, 16
sc	cs3, 0(cs3)
cincoffset	cs3, cs3, 16
sc	ctp, 0(cs3)
cincoffset	cs3, cs3, 16
cincoffset	cs3, cs3, -80
cloadtags	a6, (cs3)
cspecialr	cs9, pcc
li	s10, 0
csetflags	cs9, cs9, s10
cspecialw	mtcc, cs9
jr.cap	cs9
cspecialr	cs10, pcc
li	s11, 0
csetflags	cs10, cs10, s11
cspecialw	mtcc, cs10
jr.cap	cs10
cram	s2, ra
cspecialr	ct4, pcc
li	t5, 0
csetflags	ct4, ct4, t5
cspecialw	mtcc, ct4
jr.cap	ct4
cspecialr	ctp, pcc
li	sp, 0
slli	sp, sp, 11
mv	sp, sp
slli	sp, sp, 11
mv	sp, sp
slli	sp, sp, 11
mv	sp, sp
slli	sp, sp, 11
addi	sp, sp, 8
slli	sp, sp, 8
addi	sp, sp, 192
cincoffset	ctp, ctp, sp
li	sp, 0
slli	sp, sp, 11
mv	sp, sp
slli	sp, sp, 11
mv	sp, sp
slli	sp, sp, 11
addi	sp, sp, 1
slli	sp, sp, 11
addi	sp, sp, 1837
slli	sp, sp, 8
addi	sp, sp, 190
csetbounds	ctp, ctp, sp
jalr.cap	ctp, ctp
csetbounds	cs4, cs3, 25
nop
slli	zero, zero, 12
csetoffset	cs4, cs4, zero
cincoffset	cs4, cs4, 31
cspecialr	cgp, pcc
li	tp, 0
slli	tp, tp, 11
mv	tp, tp
slli	tp, tp, 11
mv	tp, tp
slli	tp, tp, 11
mv	tp, tp
slli	tp, tp, 11
addi	tp, tp, 7
slli	tp, tp, 8
addi	tp, tp, 80
cincoffset	cgp, cgp, tp
li	tp, 0
slli	tp, tp, 11
mv	tp, tp
slli	tp, tp, 11
mv	tp, tp
slli	tp, tp, 11
addi	tp, tp, 1
slli	tp, tp, 11
addi	tp, tp, 1365
slli	tp, tp, 8
addi	tp, tp, 147
csetbounds	cgp, cgp, tp
jalr.cap	cgp, cgp
li	s4, -3
candperm	cgp, cgp, s4
cseal	ca6, ca6, cs2
cseal	cgp, cgp, cs2
cinvoke	ca6, cgp
cmove	ct6, cra
andi	a7, a7, 255
lui	a7, 262148
slli	a7, a7, 1
add	a7, a7, a7
sc.ddc	cs2, (a7)
cgettag	zero, cnull
csetbounds	cgp, cs3, 20
nop
slli	zero, zero, 12
csetoffset	cgp, cgp, zero
cincoffset	cgp, cgp, 1773
cspecialr	cs11, pcc
li	t3, 1
csetflags	cs11, cs11, t3
cspecialw	mtcc, cs11
jr.cap	cs11
addi	zero, s2, -3
candperm	csp, csp, s2
li	s2, -3
candperm	cra, cra, s2
cseal	csp, csp, ca6
cseal	cra, cra, ca6
cinvoke	csp, cra
cmove	ct6, cra
li	gp, -3
candperm	ctp, ctp, gp
cseal	csp, csp, ctp
cseal	ctp, ctp, ctp
cinvoke	csp, ctp
cmove	ct6, cra
cspecialrw	cs3, pcc, cs4
andi	sp, sp, 255
lui	ra, 262148
slli	ra, ra, 1
sc.cap	cs4, (csp)
cspecialr	ct4, pcc
li	t5, 1
csetflags	ct4, ct4, t5
cspecialw	mtcc, ct4
jr.cap	ct4
cspecialr	cs10, pcc
li	s11, 0
csetflags	cs10, cs10, s11
cspecialw	mtcc, cs10
jr.cap	cs10
li	sp, -3
candperm	ca7, ca7, sp
li	sp, -257
candperm	cra, cra, sp
li	sp, -257
candperm	ca7, ca7, sp
cseal	cra, cra, ca7
cseal	ca7, ca7, ca7
cinvoke	cra, ca7
cmove	ct6, cra
andi	a6, a6, 255
lui	ra, 262148
slli	ra, ra, 1
lb	ra, 0(ca6)
cspecialrw	cs2, mepcc, cra
cgettag	ra, cra
cspecialr	csp, pcc
li	s4, -1025
candperm	csp, csp, s4
cspecialw	mtcc, csp
jalr.cap	csp, cnull
cspecialr	ca6, pcc
li	a6, 0
slli	a6, a6, 11
mv	a6, a6
slli	a6, a6, 11
mv	a6, a6
slli	a6, a6, 11
mv	a6, a6
slli	a6, a6, 11
addi	a6, a6, 15
slli	a6, a6, 8
addi	a6, a6, 15
cincoffset	ca6, ca6, a6
li	a6, 0
slli	a6, a6, 11
mv	a6, a6
slli	a6, a6, 11
mv	a6, a6
slli	a6, a6, 11
mv	a6, a6
slli	a6, a6, 11
addi	a6, a6, 1587
slli	a6, a6, 8
addi	a6, a6, 116
csetbounds	ca6, ca6, a6
jalr.cap	ca6, ca6
cspecialrw	csp, mscratchc, ctp
jalr.cap	csp, cnull
cspecialrw	ctp, mscratchc, cra
cspecialrw	ctp, mscratchc, cs2
cgettag	s3, cs3
andi	a6, a6, 255
lui	gp, 262148
slli	gp, gp, 1
sc.cap	cs3, (ca6)
csetbounds	csp, cs4, 20
li	s3, 0
slli	s3, s3, 12
csetoffset	csp, csp, s3
cincoffset	csp, csp, 2
cspecialrw	cra, mscratchc, cra
jalr.cap	cnull
cspecialrw	cra, mscratchc, cs3
cspecialw	mscratchc, ca7
andi	sp, sp, 255
lui	gp, 262148
slli	gp, gp, 1
sc.cap	cnull, (csp)
cspecialr	cs6, pcc
li	s7, 1
csetflags	cs6, cs6, s7
cspecialw	mtcc, cs6
jr.cap	cs6
andi	tp, tp, 255
lui	s3, 262148
slli	s3, s3, 1
lbu	s3, 0(ctp)
lhu	gp, -204(cra)
cgettag	sp, csp
andi	gp, gp, 255
lui	sp, 262148
slli	sp, sp, 1
add	gp, sp, gp
sc.cap	cs2, (cgp)
cspecialr	csp, pcc
li	ra, 0
slli	ra, ra, 11
mv	ra, ra
slli	ra, ra, 11
mv	ra, ra
slli	ra, ra, 11
mv	ra, ra
slli	ra, ra, 11
addi	ra, ra, 3
slli	ra, ra, 8
addi	ra, ra, 19
cincoffset	csp, csp, ra
li	ra, 0
slli	ra, ra, 11
mv	ra, ra
slli	ra, ra, 11
mv	ra, ra
slli	ra, ra, 11
addi	ra, ra, 1
slli	ra, ra, 11
addi	ra, ra, 2017
slli	ra, ra, 8
addi	ra, ra, 103
csetbounds	csp, csp, ra
jalr.cap	csp, csp
cincoffset	ctp, cs2, gp
andi	s3, s3, 255
lui	sp, 262148
slli	sp, sp, 1
add	s3, sp, s3
sc.cap	cnull, (cs3)
cspecialrw	cra, ddc, cs3
andi	gp, gp, 255
lui	ra, 262148
slli	ra, ra, 1
add	gp, ra, gp
sc.cap	cra, (cgp)
csetbounds	ca7, ca7, 28
li	s4, 0
slli	s4, s4, 12
csetoffset	ca7, ca7, s4
cincoffset	ca7, ca7, 21
andi	s3, s3, 255
lui	tp, 262148
slli	tp, tp, 1
add	s3, tp, s3
sc.cap	cs4, (cs3)
cspecialrw	cs3, mscratchc, cs2
jalr.cap	cs3, cnull
cspecialrw	ctp, mscratchc, cgp
cspecialw	mscratchc, ctp
csetbounds	cs3, cgp, 2
li	s2, 0
slli	s2, s2, 12
csetoffset	cs3, cs3, s2
cincoffset	cs3, cs3, 17
cgettag	gp, cgp
cspecialr	ct3, pcc
li	t4, 1
csetflags	ct3, ct3, t4
cspecialw	mtcc, ct3
jr.cap	ct3
cincoffset	cs2, cra, 193
cgethigh	ra, csp
cspecialr	cs9, pcc
li	s10, 1
csetflags	cs9, cs9, s10
cspecialw	mtcc, cs9
jr.cap	cs9
andi	a7, a7, 255
lui	sp, 262148
slli	sp, sp, 1
add	a7, sp, a7
lc.ddc	csp, (a7)
csetbounds	cs3, cgp, 7
li	s2, 0
slli	s2, s2, 12
csetoffset	cs3, cs3, s2
cincoffset	cs3, cs3, 24
andi	a7, a7, 255
lui	gp, 262148
slli	gp, gp, 1
add	a7, gp, a7
lc.ddc	ctp, (a7)
andi	zero, zero, 255
lui	a6, 262148
slli	a6, a6, 1
sc.cap	cgp, (cnull)
cspecialr	cra, pcc
li	s2, 0
slli	s2, s2, 11
mv	s2, s2
slli	s2, s2, 11
mv	s2, s2
slli	s2, s2, 11
mv	s2, s2
slli	s2, s2, 11
addi	s2, s2, 6
slli	s2, s2, 8
addi	s2, s2, 166
cincoffset	cra, cra, s2
li	s2, 0
slli	s2, s2, 11
mv	s2, s2
slli	s2, s2, 11
mv	s2, s2
slli	s2, s2, 11
mv	s2, s2
slli	s2, s2, 11
addi	s2, s2, 892
slli	s2, s2, 8
addi	s2, s2, 167
csetbounds	cra, cra, s2
jalr.cap	cra
andi	a7, a7, 255
lui	s3, 262148
slli	s3, s3, 1
add	a7, s3, a7
sc.cap	ca6, (ca7)
csethigh	cnull, ctp, s3
cgettag	ra, cra
<unknown>
andi	ra, ra, 255
lui	sp, 262148
slli	sp, sp, 1
add	ra, sp, ra
sc.ddc	ctp, (ra)
cspecialrw	cs2, mscratchc, cs4
jalr.cap	cs2, cnull
cspecialrw	ctp, mscratchc, cra
cspecialrw	cs3, mscratchc, ca6
andi	a7, a7, 255
lui	sp, 262148
slli	sp, sp, 1
add	a7, sp, a7
sc.cap	ctp, (ca7)
li	a7, -3
candperm	cs3, cs3, a7
cseal	cs3, cs3, ca6
cinvoke	cs4, cs3
cmove	ct6, cra
cloadtags	s4, (csp)
andi	a7, a7, 255
lui	sp, 262148
slli	sp, sp, 1
add	a7, sp, a7
sc.ddc	ca7, (a7)
csetbounds	cs3, ca6, 29
li	gp, 0
slli	gp, gp, 12
csetoffset	cs3, cs3, gp
cincoffset	cs3, cs3, 17
ori	a6, s2, 129
andi	tp, tp, 255
lui	zero, 262148
slli	zero, zero, 1
lh	ra, 0(ctp)
andi	a6, a6, 255
lui	s4, 262148
slli	s4, s4, 1
sc.ddc	cs3, (a6)
cspecialw	mscratchc, cgp
jr.cap	cnull
cspecialrw	cgp, mscratchc, cra
cspecialrw	ca6, mscratchc, csp
andi	ra, ra, 255
lui	zero, 262148
slli	zero, zero, 1
add	ra, zero, ra
lw	sp, 0(cra)
andi	s4, s4, 255
lui	gp, 262148
slli	gp, gp, 1
sc.ddc	ctp, (s4)
andi	s2, s2, 255
lui	a7, 262148
slli	a7, a7, 1
sc.cap	cgp, (cs2)
andi	zero, zero, 255
lui	a7, 262148
slli	a7, a7, 1
sc.cap	cnull, (cnull)
andi	s4, s4, 255
lui	gp, 262148
slli	gp, gp, 1
sc.cap	cs3, (cs4)
cspecialr	cra, pcc
li	sp, -1025
candperm	cra, cra, sp
cspecialw	mtcc, cra
jalr.cap	cnull
cgettag	zero, cnull
cspecialr	cs6, pcc
li	s7, 1
csetflags	cs6, cs6, s7
cspecialw	mtcc, cs6
jr.cap	cs6
