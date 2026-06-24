

lui	s2, 262148
slli	s2, s2, 1
csetaddr	cs4, cs4, s2
ccleartag	cs2, cs2
sc	cs2, 0(cs4)
cincoffset	cs4, cs4, 16
sc	cs2, 0(cs4)
cincoffset	cs4, cs4, 16
sc	cs2, 0(cs4)
cincoffset	cs4, cs4, 16
sc	cs2, 0(cs4)
cincoffset	cs4, cs4, 16
sc	cs2, 0(cs4)
cincoffset	cs4, cs4, 16
cincoffset	cs4, cs4, -80
cloadtags	a7, (cs4)
andi	ra, ra, 255
lui	s3, 262148
slli	s3, s3, 1
add	ra, s3, ra
sc.ddc	cs4, (ra)
cspecialr	cs9, pcc
li	s10, 1
csetflags	cs9, cs9, s10
cspecialw	mtcc, cs9
jr.cap	cs9
andi	gp, gp, 255
lui	gp, 262148
slli	gp, gp, 1
add	gp, gp, gp
sc.cap	cnull, (cgp)
cgetsealed	ra, cnull
andi	s2, s2, 255
lui	a6, 262148
slli	a6, a6, 1
sc.cap	cra, (cs2)
andi	gp, gp, 255
lui	sp, 262148
slli	sp, sp, 1
add	gp, sp, gp
sc.cap	cs3, (cgp)
andi	a6, a6, 255
lui	sp, 262148
slli	sp, sp, 1
sc.ddc	cs4, (a6)
andi	a7, a7, 255
lui	s4, 262148
slli	s4, s4, 1
add	a7, s4, a7
lhu	s3, 0(ca7)
andi	a6, a6, 255
lui	a6, 262148
slli	a6, a6, 1
sc.ddc	csp, (a6)
cspecialr	cgp, pcc
li	a6, 0
slli	a6, a6, 11
mv	a6, a6
slli	a6, a6, 11
mv	a6, a6
slli	a6, a6, 11
mv	a6, a6
slli	a6, a6, 11
addi	a6, a6, 4
slli	a6, a6, 8
addi	a6, a6, 214
cincoffset	cgp, cgp, a6
li	a6, 0
slli	a6, a6, 11
mv	a6, a6
slli	a6, a6, 11
mv	a6, a6
slli	a6, a6, 11
mv	a6, a6
slli	a6, a6, 11
addi	a6, a6, 1339
slli	a6, a6, 8
addi	a6, a6, 78
csetbounds	cgp, cgp, a6
jalr.cap	cgp, cgp
cgettag	sp, csp
cgettag	s3, cs3
andi	a7, a7, 255
lui	s3, 262148
slli	s3, s3, 1
add	a7, s3, a7
sc.cap	ca6, (ca7)
andi	a7, a7, 255
lui	s3, 262148
slli	s3, s3, 1
add	a7, s3, a7
sc.ddc	cs2, (a7)
andi	a6, a6, 255
lui	sp, 262148
slli	sp, sp, 1
sc.ddc	cgp, (a6)
addi	ra, a7, 372
andi	s4, s4, 255
lui	a7, 262148
slli	a7, a7, 1
sw	s4, 0(ctp)
csetbounds	ca6, cra, 29
li	a6, 1
slli	a6, a6, 12
csetoffset	ca6, ca6, a6
cincoffset	ca6, ca6, 1957
andi	a7, a7, 255
lui	tp, 262148
slli	tp, tp, 1
add	a7, tp, a7
sc.cap	cs4, (ca7)
cspecialr	cs8, pcc
li	s9, 0
csetflags	cs8, cs8, s9
cspecialw	mtcc, cs8
jr.cap	cs8
andi	gp, gp, 255
lui	gp, 262148
slli	gp, gp, 1
add	gp, gp, gp
sc.ddc	ctp, (gp)
csetbounds	cs3, cs4, 27
li	a6, 0
slli	a6, a6, 12
csetoffset	cs3, cs3, a6
cincoffset	cs3, cs3, 31
andi	gp, gp, 255
lui	ra, 262148
slli	ra, ra, 1
add	gp, ra, gp
sc.ddc	ctp, (gp)
andi	s3, s3, 255
lui	s4, 262148
slli	s4, s4, 1
add	s3, s4, s3
sc.cap	cra, (cs3)
ebreak
<unknown>
li	tp, -3
candperm	cnull, cnull, tp
cseal	cs2, cs2, ca6
cseal	cnull, cnull, ca6
cinvoke	cs2, cnull
cmove	ct6, cra
xor	s3, s2, zero
cspecialr	cs9, pcc
li	s10, 0
csetflags	cs9, cs9, s10
cspecialw	mtcc, cs9
jr.cap	cs9
li	a6, -3
candperm	ca7, ca7, a6
cseal	ctp, ctp, cs4
cseal	ca7, ca7, cs4
cinvoke	ctp, ca7
cmove	ct6, cra
cgettag	ra, cra
cspecialr	cs6, pcc
li	s7, 0
csetflags	cs6, cs6, s7
cspecialw	mtcc, cs6
jr.cap	cs6
cgettag	ra, cra
csetbounds	csp, cnull, 29
li	zero, 1
slli	zero, zero, 12
csetoffset	csp, csp, zero
cincoffset	csp, csp, 658
cspecialr	cs10, pcc
li	s11, 1
csetflags	cs10, cs10, s11
cspecialw	mtcc, cs10
jr.cap	cs10
lui	a7, 262148
slli	a7, a7, 1
csetaddr	cnull, cnull, a7
ccleartag	ca7, ctp
sc	ca7, 0(cnull)
cincoffset	cnull, cnull, 16
sc	ca7, 0(cnull)
cincoffset	cnull, cnull, 16
sc	ca7, 0(cnull)
cincoffset	cnull, cnull, 16
sc	ctp, 0(cnull)
cincoffset	cnull, cnull, 16
sc	ctp, 0(cnull)
cincoffset	cnull, cnull, 16
cincoffset	cnull, cnull, -80
cloadtags	ra, (cnull)
cgettag	s4, cs4
andi	s4, s4, 255
lui	a6, 262148
slli	a6, a6, 1
sc.ddc	cnull, (s4)
cgettag	gp, cgp
csetbounds	cs4, cs4, 22
li	sp, 0
slli	sp, sp, 12
csetoffset	cs4, cs4, sp
cincoffset	cs4, cs4, -1868
andi	s4, s4, 255
lui	a6, 262148
slli	a6, a6, 1
sc.ddc	cgp, (s4)
li	s2, -3
candperm	cs4, cs4, s2
cseal	cgp, cgp, cnull
cseal	cs4, cs4, cnull
cinvoke	cgp, cs4
cmove	ct6, cra
li	a6, -3
candperm	cs3, cs3, a6
cseal	cs4, cs4, cs2
cseal	cs3, cs3, cs2
cinvoke	cs4, cs3
cmove	ct6, cra
andi	s2, s2, 255
lui	s2, 262148
slli	s2, s2, 1
sc.ddc	csp, (s2)
cgettag	gp, cgp
andi	ra, ra, 255
lui	ra, 262148
slli	ra, ra, 1
add	ra, ra, ra
sc.cap	cs4, (cra)
cspecialr	cs10, pcc
li	s11, 1
csetflags	cs10, cs10, s11
cspecialw	mtcc, cs10
jr.cap	cs10
cspecialr	ctp, pcc
li	s4, 0
slli	s4, s4, 11
mv	s4, s4
slli	s4, s4, 11
mv	s4, s4
slli	s4, s4, 11
mv	s4, s4
slli	s4, s4, 11
mv	s4, s4
slli	s4, s4, 8
addi	s4, s4, 156
cincoffset	ctp, ctp, s4
li	s4, 0
slli	s4, s4, 11
mv	s4, s4
slli	s4, s4, 11
mv	s4, s4
slli	s4, s4, 11
addi	s4, s4, 1
slli	s4, s4, 11
addi	s4, s4, 1312
slli	s4, s4, 8
addi	s4, s4, 26
csetbounds	ctp, ctp, s4
jalr.cap	ctp, ctp
cspecialr	cs7, pcc
li	s8, 1
csetflags	cs7, cs7, s8
cspecialw	mtcc, cs7
jr.cap	cs7
andi	a7, a7, 255
lui	s4, 262148
slli	s4, s4, 1
add	a7, s4, a7
sc.ddc	ca7, (a7)
cgettag	s2, cs2
andi	sp, sp, 255
lui	a6, 262148
slli	a6, a6, 1
sc.ddc	ca6, (sp)
cgettag	gp, cgp
andi	a7, a7, 255
lui	s3, 262148
slli	s3, s3, 1
add	a7, s3, a7
sc.cap	cra, (ca7)
li	s4, -3
candperm	csp, csp, s4
cseal	ctp, ctp, cs3
cseal	csp, csp, cs3
cinvoke	ctp, csp
cmove	ct6, cra
lui	s2, 262148
slli	s2, s2, 1
csetaddr	ca7, ca7, s2
ccleartag	cs2, cs4
sc	cs2, 0(ca7)
cincoffset	ca7, ca7, 16
sc	cs2, 0(ca7)
cincoffset	ca7, ca7, 16
sc	cs4, 0(ca7)
cincoffset	ca7, ca7, 16
sc	cs2, 0(ca7)
cincoffset	ca7, ca7, 16
sc	cs4, 0(ca7)
cincoffset	ca7, ca7, 16
cincoffset	ca7, ca7, -80
cloadtags	tp, (ca7)
andi	gp, gp, 255
lui	ra, 262148
slli	ra, ra, 1
add	gp, ra, gp
sw	gp, 0(cgp)
auipcc	ca7, 437828
cgettag	zero, cnull
cspecialw	mscratchc, ca7
jr.cap	cnull
cspecialrw	cra, mscratchc, ctp
cspecialrw	cgp, mscratchc, cra
li	s3, -257
candperm	cra, cra, s3
cseal	cs2, cs2, cgp
cseal	cra, cra, cgp
cinvoke	cs2, cra
cmove	ct6, cra
cinvoke	ca6, csp
csetbounds	csp, ca7, 32
li	gp, 2
slli	gp, gp, 12
csetoffset	csp, csp, gp
cincoffset	csp, csp, 1992
andi	tp, tp, 255
lui	zero, 262148
slli	zero, zero, 1
sc.cap	cs3, (ctp)
andi	tp, tp, 255
lui	sp, 262148
slli	sp, sp, 1
sc.ddc	cra, (tp)
andi	s3, s3, 255
lui	gp, 262148
slli	gp, gp, 1
add	s3, gp, s3
lh	sp, 0(cs3)
andi	s3, s3, 255
lui	gp, 262148
slli	gp, gp, 1
add	s3, gp, s3
sc.cap	csp, (cs3)
andi	sp, sp, 255
lui	sp, 262148
slli	sp, sp, 1
sc.cap	cs2, (csp)
addi	zero, a7, -3
candperm	ca6, ca6, a7
li	a7, -3
candperm	cnull, cnull, a7
li	a7, -257
candperm	cnull, cnull, a7
cseal	ca6, ca6, csp
cseal	cnull, cnull, csp
cinvoke	ca6, cnull
cmove	ct6, cra
andi	s2, s2, 255
lui	sp, 262148
slli	sp, sp, 1
sc.ddc	ctp, (s2)
li	gp, -3
candperm	cra, cra, gp
cseal	csp, csp, ca6
cinvoke	csp, cra
cmove	ct6, cra
cspecialrw	csp, mscratchc, ca6
cspecialr	cs7, pcc
li	s8, 1
csetflags	cs7, cs7, s8
cspecialw	mtcc, cs7
jr.cap	cs7
beq	s3, sp, 0x115c6 <.text+0x4f2>
andi	s4, s4, 255
lui	sp, 262148
slli	sp, sp, 1
sc.cap	ca7, (cs4)
cgettype	a6, cnull
li	a7, -3
candperm	ca7, ca7, a7
li	a7, -257
candperm	ctp, ctp, a7
cseal	ctp, ctp, cra
cseal	ca7, ca7, cra
cinvoke	ctp, ca7
cmove	ct6, cra
csetbounds	cnull, cs2, 21
li	a7, 3
slli	a7, a7, 12
csetoffset	cnull, cnull, a7
cincoffset	cnull, cnull, -1355
andi	ra, ra, 255
lui	ra, 262148
slli	ra, ra, 1
add	ra, ra, ra
sc.cap	ctp, (cra)
andi	zero, zero, 255
lui	a7, 262148
slli	a7, a7, 1
sc.ddc	cgp, (zero)
cspecialr	csp, pcc
li	a7, -1025
candperm	csp, csp, a7
cspecialw	mtcc, csp
jalr.cap	csp, cnull
cgettag	ra, cra
cgettag	ra, cra
cspecialr	ct3, pcc
li	t4, 0
csetflags	ct3, ct3, t4
cspecialw	mtcc, ct3
jr.cap	ct3
cspecialr	csp, pcc
li	s4, 0
slli	s4, s4, 11
mv	s4, s4
slli	s4, s4, 11
mv	s4, s4
slli	s4, s4, 11
mv	s4, s4
slli	s4, s4, 11
addi	s4, s4, 1
slli	s4, s4, 8
addi	s4, s4, 95
cincoffset	csp, csp, s4
li	s4, 0
slli	s4, s4, 11
mv	s4, s4
slli	s4, s4, 11
mv	s4, s4
slli	s4, s4, 11
mv	s4, s4
slli	s4, s4, 11
addi	s4, s4, 1812
slli	s4, s4, 8
addi	s4, s4, 23
csetbounds	csp, csp, s4
jalr.cap	csp, csp
csetbounds	csp, cnull, s3
csetbounds	cgp, ca6, 5
li	s4, 0
slli	s4, s4, 12
csetoffset	cgp, cgp, s4
cincoffset	cgp, cgp, 5
cgettag	sp, csp
andi	ra, ra, 255
lui	s3, 262148
slli	s3, s3, 1
add	ra, s3, ra
sc.ddc	cs4, (ra)
andi	s3, s3, 255
lui	s4, 262148
slli	s4, s4, 1
add	s3, s4, s3
sc.ddc	cs2, (s3)
lui	s4, 262148
slli	s4, s4, 1
csetaddr	cs3, cs3, s4
ccleartag	cs4, ca6
sc	ca6, 0(cs3)
cincoffset	cs3, cs3, 16
sc	ca6, 0(cs3)
cincoffset	cs3, cs3, 16
sc	ca6, 0(cs3)
cincoffset	cs3, cs3, 16
sc	ca6, 0(cs3)
cincoffset	cs3, cs3, 16
sc	cs4, 0(cs3)
cincoffset	cs3, cs3, 16
cincoffset	cs3, cs3, -80
cloadtags	sp, (cs3)
andi	a6, a6, 255
lui	a6, 262148
slli	a6, a6, 1
sw	a6, 0(cgp)
li	s2, -3
candperm	cra, cra, s2
cseal	csp, csp, cra
cseal	cra, cra, cra
cinvoke	csp, cra
cmove	ct6, cra
cspecialr	ct4, pcc
li	t5, 1
csetflags	ct4, ct4, t5
cspecialw	mtcc, ct4
jr.cap	ct4
andi	ra, ra, 255
lui	gp, 262148
slli	gp, gp, 1
add	ra, gp, ra
sc.ddc	cnull, (ra)
andi	s2, s2, 255
lui	s4, 262148
slli	s4, s4, 1
sc.cap	cnull, (cs2)
lui	ra, 262148
slli	ra, ra, 1
csetaddr	cs2, cs2, ra
ccleartag	cra, cs3
sc	cra, 0(cs2)
cincoffset	cs2, cs2, 16
sc	cs3, 0(cs2)
cincoffset	cs2, cs2, 16
sc	cs3, 0(cs2)
cincoffset	cs2, cs2, 16
sc	cra, 0(cs2)
cincoffset	cs2, cs2, 16
sc	cra, 0(cs2)
cincoffset	cs2, cs2, 16
cincoffset	cs2, cs2, -80
cloadtags	s2, (cs2)
cgettag	gp, cgp
andi	gp, gp, 255
lui	s4, 262148
slli	s4, s4, 1
add	gp, s4, gp
lhu	a7, 0(cgp)
bgeu	a6, s3, 0x123a2 <.text+0x12ce>
andi	gp, gp, 255
lui	ra, 262148
slli	ra, ra, 1
add	gp, ra, gp
sc.ddc	cs3, (gp)
cspecialrw	ctp, mscratchc, cs2
jalr.cap	ctp, cnull
cspecialrw	cgp, mscratchc, cra
cspecialr	ca6, mscratchc
andi	a7, a7, 255
lui	a7, 262148
slli	a7, a7, 1
add	a7, a7, a7
sc.cap	cs4, (ca7)
lui	s4, 262148
slli	s4, s4, 1
csetaddr	cs2, cs2, s4
ccleartag	cs4, cs4
sc	cs4, 0(cs2)
cincoffset	cs2, cs2, 16
sc	cs4, 0(cs2)
cincoffset	cs2, cs2, 16
sc	cs4, 0(cs2)
cincoffset	cs2, cs2, 16
sc	cs4, 0(cs2)
cincoffset	cs2, cs2, 16
sc	cs4, 0(cs2)
cincoffset	cs2, cs2, 16
cincoffset	cs2, cs2, -80
cloadtags	s2, (cs2)
andi	a6, a6, 255
lui	s2, 262148
slli	s2, s2, 1
lw	sp, 0(ca6)
lw	gp, 1647(cnull)
cspecialr	cs3, mscratchc
jalr.cap	cs3, cnull
cspecialrw	csp, mscratchc, ctp
cspecialrw	csp, mscratchc, csp
andi	s2, s2, 255
lui	ra, 262148
slli	ra, ra, 1
sc.ddc	cgp, (s2)
andi	zero, zero, 255
lui	s3, 262148
slli	s3, s3, 1
sc.cap	ca6, (cnull)
cgettag	s3, cs3
cspecialr	ctp, pcc
li	s3, 0
slli	s3, s3, 11
mv	s3, s3
slli	s3, s3, 11
mv	s3, s3
slli	s3, s3, 11
mv	s3, s3
slli	s3, s3, 11
addi	s3, s3, 2
slli	s3, s3, 8
addi	s3, s3, 122
cincoffset	ctp, ctp, s3
li	s3, 0
slli	s3, s3, 11
mv	s3, s3
slli	s3, s3, 11
mv	s3, s3
slli	s3, s3, 11
mv	s3, s3
slli	s3, s3, 11
addi	s3, s3, 1754
slli	s3, s3, 8
addi	s3, s3, 59
csetbounds	ctp, ctp, s3
jalr.cap	ctp, ctp
andi	ra, ra, 255
lui	s3, 262148
slli	s3, s3, 1
add	ra, s3, ra
sc.cap	cs2, (cra)
cspecialr	cs9, pcc
li	s10, 1
csetflags	cs9, cs9, s10
cspecialw	mtcc, cs9
jr.cap	cs9
csetbounds	cs4, cra, 15
li	gp, 1
slli	gp, gp, 12
csetoffset	cs4, cs4, gp
cincoffset	cs4, cs4, 534
cseal	cnull, cnull, ctp
cseal	cnull, cnull, ctp
cinvoke	cnull, cnull
cmove	ct6, cra
andi	s2, s2, 255
lui	tp, 262148
slli	tp, tp, 1
sc.ddc	ca6, (s2)
andi	s4, s4, 255
lui	s3, 262148
slli	s3, s3, 1
sc.ddc	cs2, (s4)
andi	zero, zero, 255
lui	s2, 262148
slli	s2, s2, 1
sc.cap	ca6, (cnull)
cgettype	a7, csp
cspecialrw	cs4, ddc, cra
cspecialr	ca7, pcc
li	zero, -1025
candperm	ca7, ca7, zero
cspecialw	mtcc, ca7
jalr.cap	ca7, cnull
cgetbase	s3, ca7
andi	tp, tp, 255
lui	tp, 262148
slli	tp, tp, 1
sc.cap	ctp, (ctp)
andi	gp, gp, 255
lui	zero, 262148
slli	zero, zero, 1
add	gp, zero, gp
sc.cap	cra, (cgp)
cspecialr	cs6, pcc
li	s7, 1
csetflags	cs6, cs6, s7
cspecialw	mtcc, cs6
jr.cap	cs6
csealentry	ca6, cs4
cspecialrw	cgp, mtdc, cs4
cspecialr	cs6, pcc
li	s7, 0
csetflags	cs6, cs6, s7
cspecialw	mtcc, cs6
jr.cap	cs6
lui	zero, 985042
andi	a6, a6, 255
lui	gp, 262148
slli	gp, gp, 1
sc.cap	csp, (ca6)
cgetbase	s4, ca6
cgettag	s3, cs3
cgettag	tp, ctp
andi	tp, tp, 255
lui	tp, 262148
slli	tp, tp, 1
lc.ddc	cra, (tp)
andi	s3, s3, 255
lui	a7, 262148
slli	a7, a7, 1
add	s3, a7, s3
lc.ddc	cs4, (s3)
andi	ra, ra, 255
lui	s2, 262148
slli	s2, s2, 1
add	ra, s2, ra
sc.cap	cra, (cra)
andi	gp, gp, 255
lui	s4, 262148
slli	s4, s4, 1
add	gp, s4, gp
sc.ddc	cgp, (gp)
csetbounds	cgp, ca6, 5
nop
slli	zero, zero, 12
csetoffset	cgp, cgp, zero
cincoffset	cgp, cgp, 22
andi	a6, a6, 255
lui	zero, 262148
slli	zero, zero, 1
sc.ddc	cnull, (a6)
andi	a6, a6, 255
lui	tp, 262148
slli	tp, tp, 1
sc.ddc	csp, (a6)
li	tp, -3
candperm	cra, cra, tp
cseal	cra, cra, cs3
cinvoke	cgp, cra
cmove	ct6, cra
andi	ra, ra, 255
lui	s4, 262148
slli	s4, s4, 1
add	ra, s4, ra
sc.ddc	ctp, (ra)
andi	s4, s4, 255
lui	s3, 262148
slli	s3, s3, 1
sc.ddc	ctp, (s4)
lui	tp, 262148
slli	tp, tp, 1
csetaddr	csp, csp, tp
ccleartag	ctp, ca6
sc	ctp, 0(csp)
cincoffset	csp, csp, 16
sc	ca6, 0(csp)
cincoffset	csp, csp, 16
sc	ctp, 0(csp)
cincoffset	csp, csp, 16
sc	ca6, 0(csp)
cincoffset	csp, csp, 16
sc	ca6, 0(csp)
cincoffset	csp, csp, 16
cincoffset	csp, csp, -80
cloadtags	s4, (csp)
cgettag	gp, cgp
andi	a7, a7, 255
lui	sp, 262148
slli	sp, sp, 1
add	a7, sp, a7
sc.cap	cgp, (ca7)
auipcc	ca6, 264388
cspecialr	ca6, pcc
li	a7, -1025
candperm	ca6, ca6, a7
cspecialw	mtcc, ca6
jalr.cap	ca6, cnull
andi	a7, a7, 255
lui	a7, 262148
slli	a7, a7, 1
add	a7, a7, a7
sc.cap	cs2, (ca7)
cgettag	tp, ctp
lh	s4, 417(cs2)
csetbounds	csp, ca6, 31
li	s3, 3
slli	s3, s3, 12
csetoffset	csp, csp, s3
cincoffset	csp, csp, -837
andi	tp, tp, 255
lui	a6, 262148
slli	a6, a6, 1
sc.cap	cs2, (ctp)
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
cloadtags	s4, (cnull)
lui	zero, 262148
slli	zero, zero, 1
csetaddr	cs3, cs3, zero
ccleartag	cnull, cra
sc	cra, 0(cs3)
cincoffset	cs3, cs3, 16
sc	cra, 0(cs3)
cincoffset	cs3, cs3, 16
sc	cnull, 0(cs3)
cincoffset	cs3, cs3, 16
sc	cnull, 0(cs3)
cincoffset	cs3, cs3, 16
sc	cnull, 0(cs3)
cincoffset	cs3, cs3, 16
cincoffset	cs3, cs3, -80
cloadtags	tp, (cs3)
cspecialrw	cra, mscratchc, ca7
jalr.cap	cnull
cspecialrw	cra, mscratchc, csp
cspecialrw	cs3, mscratchc, cs4
andi	gp, gp, 255
lui	s2, 262148
slli	s2, s2, 1
add	gp, s2, gp
sc.ddc	ca6, (gp)
li	a7, -3
candperm	cs4, cs4, a7
cseal	ca6, ca6, cnull
cinvoke	ca6, cs4
cmove	ct6, cra
andi	zero, zero, 255
lui	s3, 262148
slli	s3, s3, 1
lbu	gp, 0(cnull)
lui	tp, 262148
slli	tp, tp, 1
csetaddr	cnull, cnull, tp
ccleartag	ctp, cnull
sc	ctp, 0(cnull)
cincoffset	cnull, cnull, 16
sc	cnull, 0(cnull)
cincoffset	cnull, cnull, 16
sc	ctp, 0(cnull)
cincoffset	cnull, cnull, 16
sc	ctp, 0(cnull)
cincoffset	cnull, cnull, 16
sc	ctp, 0(cnull)
cincoffset	cnull, cnull, 16
cincoffset	cnull, cnull, -80
cloadtags	zero, (cnull)
cgettag	s3, cs3
cgettag	a7, ca7
andi	zero, zero, 255
lui	ra, 262148
slli	ra, ra, 1
sc.ddc	csp, (zero)
csetbounds	csp, ctp, 9
li	sp, 0
slli	sp, sp, 12
csetoffset	csp, csp, sp
cincoffset	csp, csp, 19
cgettag	gp, cgp
cspecialr	cs11, pcc
li	t3, 1
csetflags	cs11, cs11, t3
cspecialw	mtcc, cs11
jr.cap	cs11
cspecialrw	csp, mepcc, ca7
andi	zero, zero, 255
lui	ra, 262148
slli	ra, ra, 1
sc.ddc	ca6, (zero)
cgettag	s3, cs3
andi	s3, s3, 255
lui	sp, 262148
slli	sp, sp, 1
add	s3, sp, s3
sc.ddc	csp, (s3)
andi	zero, zero, 255
lui	zero, 262148
slli	zero, zero, 1
lbu	ra, 0(cnull)
cgettag	s2, cs2
cspecialr	cs2, pcc
li	gp, -1025
candperm	cs2, cs2, gp
cspecialw	mtcc, cs2
jalr.cap	cs2, cnull
cspecialrw	cs4, mtdc, cgp
andi	ra, ra, 255
lui	zero, 262148
slli	zero, zero, 1
add	ra, zero, ra
lc.ddc	ca7, (ra)
cspecialr	cgp, pcc
li	a7, -1025
candperm	cgp, cgp, a7
cspecialw	mtcc, cgp
jalr.cap	cgp, cnull
andi	s3, s3, 255
lui	s2, 262148
slli	s2, s2, 1
add	s3, s2, s3
sc.cap	ca7, (cs3)
cspecialr	cs9, pcc
li	s10, 0
csetflags	cs9, cs9, s10
cspecialw	mtcc, cs9
jr.cap	cs9
cgettag	s2, cs2
cgettag	s4, cs4
andi	gp, gp, 255
lui	s2, 262148
slli	s2, s2, 1
add	gp, s2, gp
sc.ddc	cs4, (gp)
andi	a7, a7, 255
lui	s4, 262148
slli	s4, s4, 1
add	a7, s4, a7
sc.cap	cs3, (ca7)
cgettag	a7, ca7
andi	a6, a6, 255
lui	s2, 262148
slli	s2, s2, 1
sc.cap	cs4, (ca6)
cgettag	s4, cs4
andi	a6, a6, 255
lui	tp, 262148
slli	tp, tp, 1
sc.ddc	cgp, (a6)
ori	a7, s4, -14
andi	s2, s2, 255
lui	sp, 262148
slli	sp, sp, 1
sc.ddc	cs3, (s2)
andi	tp, tp, 255
lui	ra, 262148
slli	ra, ra, 1
sc.cap	cs2, (ctp)
andi	s2, s2, 255
lui	s2, 262148
slli	s2, s2, 1
sc.cap	cgp, (cs2)
cspecialr	cs9, pcc
li	s10, 0
csetflags	cs9, cs9, s10
cspecialw	mtcc, cs9
jr.cap	cs9
andi	a6, a6, 255
lui	sp, 262148
slli	sp, sp, 1
sc.cap	ca7, (ca6)
cspecialr	csp, pcc
li	zero, -1025
candperm	csp, csp, zero
cspecialw	mtcc, csp
jalr.cap	csp, cnull
cspecialr	cs8, pcc
li	s9, 1
csetflags	cs8, cs8, s9
cspecialw	mtcc, cs8
jr.cap	cs8
cspecialr	ca6, pcc
li	zero, -1025
candperm	ca6, ca6, zero
cspecialw	mtcc, ca6
jalr.cap	ca6, cnull
andi	sp, sp, 255
lui	tp, 262148
slli	tp, tp, 1
sc.cap	csp, (csp)
andi	s4, s4, 255
lui	s3, 262148
slli	s3, s3, 1
sc.cap	ctp, (cs4)
cspecialrw	ca6, mtdc, cs4
cspecialrw	cs2, mscratchc, cs4
jalr.cap	cs2, cnull
cspecialrw	cs4, mscratchc, csp
cspecialrw	cra, mscratchc, cgp
andi	sp, sp, 255
lui	zero, 262148
slli	zero, zero, 1
sc.ddc	cs4, (sp)
andi	a7, a7, 255
lui	sp, 262148
slli	sp, sp, 1
add	a7, sp, a7
sc.ddc	cra, (a7)
andi	s4, s4, 255
lui	s4, 262148
slli	s4, s4, 1
sc.cap	cra, (cs4)
andi	sp, sp, 255
lui	zero, 262148
slli	zero, zero, 1
sc.cap	cs4, (csp)
cgettag	tp, ctp
cgettag	s4, cs4
cspecialr	cs10, pcc
li	s11, 1
csetflags	cs10, cs10, s11
cspecialw	mtcc, cs10
jr.cap	cs10
li	tp, -3
candperm	ca6, ca6, tp
cseal	ca6, ca6, cs3
cseal	ca6, ca6, cs3
cinvoke	ca6, ca6
cmove	ct6, cra
andi	s3, s3, 255
lui	s3, 262148
slli	s3, s3, 1
add	s3, s3, s3
lw	s4, 0(cs3)
andi	s4, s4, 255
lui	sp, 262148
slli	sp, sp, 1
sc.cap	cs2, (cs4)
cspecialr	cs7, pcc
li	s8, 0
csetflags	cs7, cs7, s8
cspecialw	mtcc, cs7
jr.cap	cs7
andi	ra, ra, 255
lui	a7, 262148
slli	a7, a7, 1
add	ra, a7, ra
sc.cap	cs2, (cra)
bge	sp, s4, 0x1112c <.text+0x58>
lui	s2, 262148
slli	s2, s2, 1
csetaddr	cs2, cs2, s2
ccleartag	cs2, cnull
sc	cnull, 0(cs2)
cincoffset	cs2, cs2, 16
sc	cnull, 0(cs2)
cincoffset	cs2, cs2, 16
sc	cs2, 0(cs2)
cincoffset	cs2, cs2, 16
sc	cs2, 0(cs2)
cincoffset	cs2, cs2, 16
sc	cnull, 0(cs2)
cincoffset	cs2, cs2, 16
cincoffset	cs2, cs2, -80
cloadtags	ra, (cs2)
cspecialr	ct3, pcc
li	t4, 1
csetflags	ct3, ct3, t4
cspecialw	mtcc, ct3
jr.cap	ct3
cspecialrw	csp, mtdc, cs2
cspecialr	cs6, pcc
li	s7, 0
csetflags	cs6, cs6, s7
cspecialw	mtcc, cs6
jr.cap	cs6
cspecialrw	cra, mtcc, cs4
lui	s2, 262148
slli	s2, s2, 1
csetaddr	ca7, ca7, s2
ccleartag	cs2, cgp
sc	cgp, 0(ca7)
cincoffset	ca7, ca7, 16
sc	cs2, 0(ca7)
cincoffset	ca7, ca7, 16
sc	cgp, 0(ca7)
cincoffset	ca7, ca7, 16
sc	cgp, 0(ca7)
cincoffset	ca7, ca7, 16
sc	cs2, 0(ca7)
cincoffset	ca7, ca7, 16
cincoffset	ca7, ca7, -80
cloadtags	s2, (ca7)
cspecialrw	cgp, mtdc, ctp
andi	a6, a6, 255
lui	tp, 262148
slli	tp, tp, 1
sc.ddc	csp, (a6)
cspecialrw	cs2, 2, cgp
cspecialr	ca7, pcc
li	tp, -1025
candperm	ca7, ca7, tp
cspecialw	mtcc, ca7
jalr.cap	ca7, cnull
csetbounds	csp, cs3, 16
li	zero, 2
slli	zero, zero, 12
csetoffset	csp, csp, zero
cincoffset	csp, csp, -266
andi	tp, tp, 255
lui	zero, 262148
slli	zero, zero, 1
sc.cap	ctp, (ctp)
csetbounds	ca7, ca7, a6
andi	a6, a6, 255
lui	a7, 262148
slli	a7, a7, 1
sc.cap	cs3, (ca6)
andi	a7, a7, 255
lui	tp, 262148
slli	tp, tp, 1
add	a7, tp, a7
sc.ddc	cra, (a7)
cgettag	s3, cs3
csetoffset	cnull, cgp, s2
li	s4, -3
candperm	cs2, cs2, s4
cseal	cs3, cs3, cs2
cseal	cs2, cs2, cs2
cinvoke	cs3, cs2
cmove	ct6, cra
csetbounds	ca6, cs4, 13
nop
slli	zero, zero, 12
csetoffset	ca6, ca6, zero
cincoffset	ca6, ca6, 12
csetbounds	cnull, csp, 599
cspecialr	cs9, pcc
li	s10, 1
csetflags	cs9, cs9, s10
cspecialw	mtcc, cs9
jr.cap	cs9
sll	s3, ra, s2
andi	s4, s4, 255
lui	s3, 262148
slli	s3, s3, 1
sc.cap	cs4, (cs4)
cspecialrw	ca6, mepcc, ctp
cgettag	s4, cs4
andi	tp, tp, 255
lui	s4, 262148
slli	s4, s4, 1
sc.cap	ca7, (ctp)
cgettag	s2, cs2
andi	tp, tp, 255
lui	a6, 262148
slli	a6, a6, 1
sb	tp, 0(cs3)
andi	ra, ra, 255
lui	sp, 262148
slli	sp, sp, 1
add	ra, sp, ra
sb	ra, 0(cs4)
andi	ra, ra, 255
lui	s2, 262148
slli	s2, s2, 1
add	ra, s2, ra
sc.cap	ca6, (cra)
cspecialr	cs10, pcc
li	s11, 0
csetflags	cs10, cs10, s11
cspecialw	mtcc, cs10
jr.cap	cs10
<unknown>
ebreak
cspecialr	cra, pcc
li	s2, -1025
candperm	cra, cra, s2
cspecialw	mtcc, cra
jalr.cap	cnull
cgettag	zero, cnull
cspecialr	cs8, pcc
li	s9, 1
csetflags	cs8, cs8, s9
cspecialw	mtcc, cs8
jr.cap	cs8
ctestsubset	s2, csp, cgp
csetbounds	ca6, cnull, 2
li	a6, 1
slli	a6, a6, 12
csetoffset	ca6, ca6, a6
cincoffset	ca6, ca6, -163
