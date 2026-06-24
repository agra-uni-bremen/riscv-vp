
    andi	s2, s2, 255
    lui	tp, 262148
    slli	tp, tp, 1
    sc.cap	ctp, (cs2)
    cspecialr	ct4, pcc
    li	t5, 1
    csetflags	ct4, ct4, t5
    cspecialw	mtcc, ct4
    jr.cap	ct4
    cspecialrw	cs3, mtcc, ca7
    lui	zero, 262148
    slli	zero, zero, 1
    csetaddr	ctp, ctp, zero
    ccleartag	cnull, ca6
    sc	ca6, 0(ctp)
    cincoffset	ctp, ctp, 16
    sc	cnull, 0(ctp)
    cincoffset	ctp, ctp, 16
    sc	ca6, 0(ctp)
    cincoffset	ctp, ctp, 16
    sc	cnull, 0(ctp)
    cincoffset	ctp, ctp, 16
    sc	cnull, 0(ctp)
    cincoffset	ctp, ctp, 16
    cincoffset	ctp, ctp, -80
    cloadtags	a7, (ctp)
    li	zero, -3
    candperm	cs2, cs2, zero
    cseal	ca7, ca7, cs2
    cseal	cs2, cs2, cs2
    cinvoke	ca7, cs2
    cmove	ct6, cra
    cspecialrw	cgp, mscratchc, cgp
    jalr.cap	cgp, cnull
    cspecialrw	cs4, mscratchc, cgp
    cspecialrw	ca6, mscratchc, cs2
    li	gp, -3
    candperm	ctp, ctp, gp
    cseal	cra, cra, cs3
    cseal	ctp, ctp, cs3
    cinvoke	cra, ctp
    cmove	ct6, cra
    cspecialrw	cs2, pcc, cs3
    cgettag	s3, cs3
    cspecialr	cs2, pcc
    li	s3, 0
    slli	s3, s3, 11
    mv	s3, s3
    slli	s3, s3, 11
    mv	s3, s3
    slli	s3, s3, 11
    mv	s3, s3
    slli	s3, s3, 11
    addi	s3, s3, 7
    slli	s3, s3, 8
    addi	s3, s3, 211
    cincoffset	cs2, cs2, s3
    li	s3, 0
    slli	s3, s3, 11
    mv	s3, s3
    slli	s3, s3, 11
    mv	s3, s3
    slli	s3, s3, 11
    mv	s3, s3
    slli	s3, s3, 11
    addi	s3, s3, 1603
    slli	s3, s3, 8
    addi	s3, s3, 74
    csetbounds	cs2, cs2, s3
    jalr.cap	cs2, cs2
    lui	a6, 262148
    slli	a6, a6, 1
    csetaddr	ca6, ca6, a6
    ccleartag	ca6, cgp
    sc	cgp, 0(ca6)
    cincoffset	ca6, ca6, 16
    sc	cgp, 0(ca6)
    cincoffset	ca6, ca6, 16
    sc	ca6, 0(ca6)
    cincoffset	ca6, ca6, 16
    sc	ca6, 0(ca6)
    cincoffset	ca6, ca6, 16
    sc	ca6, 0(ca6)
    cincoffset	ca6, ca6, 16
    cincoffset	ca6, ca6, -80
    cloadtags	ra, (ca6)
    cspecialrw	ctp, 2, cra
    cspecialrw	ca7, mscratchc, csp
    jalr.cap	ca7, cnull
    cspecialr	cnull, mscratchc
    cspecialw	mscratchc, ca7
    cloadtags	sp, (cra)
    cspecialr	cs2, 2
    candperm	csp, csp, s3
    cspecialr	ct4, pcc
    li	t5, 1
    csetflags	ct4, ct4, t5
    cspecialw	mtcc, ct4
    jr.cap	ct4
    andi	tp, tp, 255
    lui	s3, 262148
    slli	s3, s3, 1
    sw	tp, 0(ca7)
    li	ra, -3
    candperm	csp, csp, ra
    cseal	cs3, cs3, cs3
    cseal	csp, csp, cs3
    cinvoke	cs3, csp
    cmove	ct6, cra
    li	gp, -3
    candperm	cs3, cs3, gp
    cseal	csp, csp, ca7
    cseal	cs3, cs3, ca7
    cinvoke	csp, cs3
    cmove	ct6, cra
    cseal	cnull, cnull, ca7
    cseal	cnull, cnull, ca7
    cinvoke	cnull, cnull
    cmove	ct6, cra
    cspecialr	cs9, pcc
    li	s10, 1
    csetflags	cs9, cs9, s10
    cspecialw	mtcc, cs9
    jr.cap	cs9
    cgettag	tp, ctp
    andi	gp, gp, 255
    lui	ra, 262148
    slli	ra, ra, 1
    add	gp, ra, gp
    sb	gp, 0(cs3)
    cgettag	s2, cs2
    cspecialw	mscratchc, cs4
    jr.cap	cnull
    cspecialrw	csp, mscratchc, cra
    cspecialrw	csp, mscratchc, ctp
    cgettag	a7, ca7
    cincoffset	cs4, csp, s3
    andi	a6, a6, 255
    lui	s3, 262148
    slli	s3, s3, 1
    lw	s3, 0(ca6)
    li	s2, -3
    candperm	cnull, cnull, s2
    cseal	cnull, cnull, cnull
    cinvoke	cs3, cnull
    cmove	ct6, cra
    li	gp, -3
    candperm	ca6, ca6, gp
    cseal	cs4, cs4, csp
    cseal	ca6, ca6, csp
    cinvoke	cs4, ca6
    cmove	ct6, cra
    lui	tp, 262148
    slli	tp, tp, 1
    csetaddr	csp, csp, tp
    ccleartag	ctp, cgp
    sc	ctp, 0(csp)
    cincoffset	csp, csp, 16
    sc	cgp, 0(csp)
    cincoffset	csp, csp, 16
    sc	ctp, 0(csp)
    cincoffset	csp, csp, 16
    sc	ctp, 0(csp)
    cincoffset	csp, csp, 16
    sc	ctp, 0(csp)
    cincoffset	csp, csp, 16
    cincoffset	csp, csp, -80
    cloadtags	s2, (csp)
    cgettag	zero, cnull
    csetbounds	ctp, ctp, 3
    li	s3, 0
    slli	s3, s3, 12
    csetoffset	ctp, ctp, s3
    cincoffset	ctp, ctp, 14
    cspecialrw	cra, mscratchc, ctp
    jalr.cap	cnull
    cspecialrw	cra, mscratchc, cs3
    cspecialrw	csp, mscratchc, csp
    andi	ra, ra, 255
    lui	s4, 262148
    slli	s4, s4, 1
    add	ra, s4, ra
    sc.ddc	cnull, (ra)
    cspecialr	cs11, pcc
    li	t3, 0
    csetflags	cs11, cs11, t3
    cspecialw	mtcc, cs11
    jr.cap	cs11
    cgettag	s2, cs2
    andi	a6, a6, 255
    lui	sp, 262148
    slli	sp, sp, 1
    lc.ddc	cgp, (a6)
    andi	sp, sp, 255
    lui	zero, 262148
    slli	zero, zero, 1
    sw	sp, 0(cs3)
    lui	zero, 262148
    slli	zero, zero, 1
    csetaddr	csp, csp, zero
    ccleartag	cnull, cgp
    sc	cnull, 0(csp)
    cincoffset	csp, csp, 16
    sc	cnull, 0(csp)
    cincoffset	csp, csp, 16
    sc	cgp, 0(csp)
    cincoffset	csp, csp, 16
    sc	cgp, 0(csp)
    cincoffset	csp, csp, 16
    sc	cnull, 0(csp)
    cincoffset	csp, csp, 16
    cincoffset	csp, csp, -80
    cloadtags	a7, (csp)
    cgettag	s2, cs2
    lui	s3, 262148
    slli	s3, s3, 1
    csetaddr	cgp, cgp, s3
    ccleartag	cs3, cnull
    sc	cnull, 0(cgp)
    cincoffset	cgp, cgp, 16
    sc	cs3, 0(cgp)
    cincoffset	cgp, cgp, 16
    sc	cnull, 0(cgp)
    cincoffset	cgp, cgp, 16
    sc	cnull, 0(cgp)
    cincoffset	cgp, cgp, 16
    sc	cnull, 0(cgp)
    cincoffset	cgp, cgp, 16
    cincoffset	cgp, cgp, -80
    cloadtags	a7, (cgp)
    cspecialr	ca7, mepcc
    cgettag	s4, cs4
    cspecialrw	cs3, mepcc, ca6
    cspecialr	ctp, pcc
    li	tp, 0
    slli	tp, tp, 11
    mv	tp, tp
    slli	tp, tp, 11
    mv	tp, tp
    slli	tp, tp, 11
    mv	tp, tp
    slli	tp, tp, 11
    addi	tp, tp, 15
    slli	tp, tp, 8
    addi	tp, tp, 210
    cincoffset	ctp, ctp, tp
    li	tp, 0
    slli	tp, tp, 11
    mv	tp, tp
    slli	tp, tp, 11
    mv	tp, tp
    slli	tp, tp, 11
    mv	tp, tp
    slli	tp, tp, 11
    addi	tp, tp, 1389
    slli	tp, tp, 8
    addi	tp, tp, 28
    csetbounds	ctp, ctp, tp
    jalr.cap	ctp, ctp
    andi	gp, gp, 255
    lui	zero, 262148
    slli	zero, zero, 1
    add	gp, zero, gp
    lb	tp, 0(cgp)
    cspecialr	ctp, pcc
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
    addi	s4, s4, 184
    cincoffset	ctp, ctp, s4
    li	s4, 0
    slli	s4, s4, 11
    mv	s4, s4
    slli	s4, s4, 11
    mv	s4, s4
    slli	s4, s4, 11
    addi	s4, s4, 1
    slli	s4, s4, 11
    addi	s4, s4, 1729
    slli	s4, s4, 8
    addi	s4, s4, 251
    csetbounds	ctp, ctp, s4
    jalr.cap	ctp, ctp
    lui	s3, 262148
    slli	s3, s3, 1
    csetaddr	csp, csp, s3
    ccleartag	cs3, cs4
    sc	cs3, 0(csp)
    cincoffset	csp, csp, 16
    sc	cs4, 0(csp)
    cincoffset	csp, csp, 16
    sc	cs3, 0(csp)
    cincoffset	csp, csp, 16
    sc	cs3, 0(csp)
    cincoffset	csp, csp, 16
    sc	cs3, 0(csp)
    cincoffset	csp, csp, 16
    cincoffset	csp, csp, -80
    cloadtags	tp, (csp)
    csetbounds	cnull, cs2, 0
    li	ra, 2
    slli	ra, ra, 12
    csetoffset	cnull, cnull, ra
    cincoffset	cnull, cnull, -138
    cspecialw	mscratchc, cra
    jr.cap	cnull
    cspecialrw	ca7, mscratchc, csp
    cspecialrw	cgp, mscratchc, ca6
    cgettag	a7, ca7
    cgettag	a6, ca6
    cspecialrw	cra, pcc, csp
    addi	s3, tp, 314
    cgettag	a7, ca7
    andi	s4, s4, 255
    lui	gp, 262148
    slli	gp, gp, 1
    sh	s4, 0(cs3)
    cspecialr	ca7, mscratchc
    jalr.cap	ca7, cnull
    cspecialw	mscratchc, cs3
    cspecialrw	csp, mscratchc, ca6
    cgettag	s4, cs4
    cspecialr	cs11, pcc
    li	t3, 0
    csetflags	cs11, cs11, t3
    cspecialw	mtcc, cs11
    jr.cap	cs11
    li	ra, -3
    candperm	cs3, cs3, ra
    cseal	csp, csp, cra
    cseal	cs3, cs3, cra
    cinvoke	csp, cs3
    cmove	ct6, cra
    andi	s4, s4, 255
    lui	a6, 262148
    slli	a6, a6, 1
    sc.ddc	cs4, (s4)
    csetflags	ctp, cs3, s4
    cgettag	a6, ca6
    li	s4, -3
    candperm	ca7, ca7, s4
    cseal	ca6, ca6, csp
    cseal	ca7, ca7, csp
    cinvoke	ca6, ca7
    cmove	ct6, cra
    cspecialr	cgp, pcc
    li	s4, 0
    slli	s4, s4, 11
    mv	s4, s4
    slli	s4, s4, 11
    mv	s4, s4
    slli	s4, s4, 11
    mv	s4, s4
    slli	s4, s4, 11
    addi	s4, s4, 13
    slli	s4, s4, 8
    addi	s4, s4, 197
    cincoffset	cgp, cgp, s4
    li	s4, 0
    slli	s4, s4, 11
    mv	s4, s4
    slli	s4, s4, 11
    mv	s4, s4
    slli	s4, s4, 11
    addi	s4, s4, 1
    slli	s4, s4, 11
    addi	s4, s4, 1474
    slli	s4, s4, 8
    addi	s4, s4, 250
    csetbounds	cgp, cgp, s4
    jalr.cap	cgp, cgp
    cspecialrw	ctp, 2, ctp
    andi	s4, s4, 255
    lui	tp, 262148
    slli	tp, tp, 1
    sh	s4, 0(csp)
    lh	gp, 385(cs4)
    lui	sp, 262148
    slli	sp, sp, 1
    csetaddr	cra, cra, sp
    ccleartag	csp, cgp
    sc	cgp, 0(cra)
    cincoffset	cra, cra, 16
    sc	cgp, 0(cra)
    cincoffset	cra, cra, 16
    sc	csp, 0(cra)
    cincoffset	cra, cra, 16
    sc	cgp, 0(cra)
    cincoffset	cra, cra, 16
    sc	csp, 0(cra)
    cincoffset	cra, cra, 16
    cincoffset	cra, cra, -80
    cloadtags	a7, (cra)
    cspecialrw	ca6, 2, ctp
    andi	s3, s3, 255
    lui	tp, 262148
    slli	tp, tp, 1
    add	s3, tp, s3
    lc.ddc	cnull, (s3)
    cspecialrw	ctp, pcc, cra
    cgettag	a6, ca6
    andi	s4, s4, 255
    lui	sp, 262148
    slli	sp, sp, 1
    sw	s4, 0(csp)
    cgettag	zero, cnull
    lui	sp, 262148
    slli	sp, sp, 1
    csetaddr	cnull, cnull, sp
    ccleartag	csp, ctp
    sc	ctp, 0(cnull)
    cincoffset	cnull, cnull, 16
    sc	csp, 0(cnull)
    cincoffset	cnull, cnull, 16
    sc	csp, 0(cnull)
    cincoffset	cnull, cnull, 16
    sc	csp, 0(cnull)
    cincoffset	cnull, cnull, 16
    sc	csp, 0(cnull)
    cincoffset	cnull, cnull, 16
    cincoffset	cnull, cnull, -80
    cloadtags	zero, (cnull)
    andi	gp, gp, 255
    lui	s3, 262148
    slli	s3, s3, 1
    add	gp, s3, gp
    sh	gp, 0(cgp)
    li	a6, -3
    candperm	cgp, cgp, a6
    li	a6, -257
    candperm	cgp, cgp, a6
    cseal	cnull, cnull, ca6
    cseal	cgp, cgp, ca6
    cinvoke	cnull, cgp
    cmove	ct6, cra
    andi	a7, a7, 255
    lui	ra, 262148
    slli	ra, ra, 1
    add	a7, ra, a7
    lc.ddc	csp, (a7)
    andi	zero, zero, 255
    lui	tp, 262148
    slli	tp, tp, 1
    lc.ddc	cs2, (zero)
    cspecialrw	ca6, mscratchc, csp
    jalr.cap	ca6, cnull
    cspecialrw	csp, mscratchc, ctp
    cspecialw	mscratchc, cra
    cgettag	gp, cgp
    cspecialrw	cs3, mscratchc, ctp
    jalr.cap	cs3, cnull
    cspecialrw	csp, mscratchc, ca7
    cspecialrw	cra, mscratchc, csp
    cspecialrw	csp, mtcc, csp
    andi	tp, tp, 255
    lui	a6, 262148
    slli	a6, a6, 1
    lb	tp, 0(ctp)
    lui	zero, 262148
    slli	zero, zero, 1
    csetaddr	cs4, cs4, zero
    ccleartag	cnull, ctp
    sc	ctp, 0(cs4)
    cincoffset	cs4, cs4, 16
    sc	ctp, 0(cs4)
    cincoffset	cs4, cs4, 16
    sc	ctp, 0(cs4)
    cincoffset	cs4, cs4, 16
    sc	cnull, 0(cs4)
    cincoffset	cs4, cs4, 16
    sc	cnull, 0(cs4)
    cincoffset	cs4, cs4, 16
    cincoffset	cs4, cs4, -80
    cloadtags	s3, (cs4)
    cspecialr	ct3, pcc
    li	t4, 1
    csetflags	ct3, ct3, t4
    cspecialw	mtcc, ct3
    jr.cap	ct3
    cspecialr	ca6, pcc
    li	a6, 0
    slli	a6, a6, 11
    mv	a6, a6
    slli	a6, a6, 11
    mv	a6, a6
    slli	a6, a6, 11
    mv	a6, a6
    slli	a6, a6, 11
    addi	a6, a6, 14
    slli	a6, a6, 8
    addi	a6, a6, 194
    cincoffset	ca6, ca6, a6
    li	a6, 0
    slli	a6, a6, 11
    mv	a6, a6
    slli	a6, a6, 11
    mv	a6, a6
    slli	a6, a6, 11
    mv	a6, a6
    slli	a6, a6, 11
    addi	a6, a6, 1313
    slli	a6, a6, 8
    addi	a6, a6, 244
    csetbounds	ca6, ca6, a6
    jalr.cap	ca6, ca6
    andi	gp, gp, 255
    lui	a6, 262148
    slli	a6, a6, 1
    add	gp, a6, gp
    lbu	s4, 0(cgp)
    lui	zero, 262148
    slli	zero, zero, 1
    csetaddr	ca7, ca7, zero
    ccleartag	cnull, cs2
    sc	cnull, 0(ca7)
    cincoffset	ca7, ca7, 16
    sc	cs2, 0(ca7)
    cincoffset	ca7, ca7, 16
    sc	cs2, 0(ca7)
    cincoffset	ca7, ca7, 16
    sc	cnull, 0(ca7)
    cincoffset	ca7, ca7, 16
    sc	cnull, 0(ca7)
    cincoffset	ca7, ca7, 16
    cincoffset	ca7, ca7, -80
    cloadtags	a6, (ca7)
    cgetlen	gp, ctp
    cspecialr	ca7, pcc
    li	gp, 0
    slli	gp, gp, 11
    mv	gp, gp
    slli	gp, gp, 11
    mv	gp, gp
    slli	gp, gp, 11
    mv	gp, gp
    slli	gp, gp, 11
    addi	gp, gp, 1
    slli	gp, gp, 8
    addi	gp, gp, 254
    cincoffset	ca7, ca7, gp
    li	gp, 0
    slli	gp, gp, 11
    mv	gp, gp
    slli	gp, gp, 11
    mv	gp, gp
    slli	gp, gp, 11
    mv	gp, gp
    slli	gp, gp, 11
    addi	gp, gp, 1068
    slli	gp, gp, 8
    addi	gp, gp, 34
    csetbounds	ca7, ca7, gp
    jalr.cap	ca7, ca7
    cgettag	s4, cs4
    andi	gp, gp, 255
    lui	zero, 262148
    slli	zero, zero, 1
    add	gp, zero, gp
    sc.ddc	ctp, (gp)
    cspecialr	cs8, pcc
    li	s9, 0
    csetflags	cs8, cs8, s9
    cspecialw	mtcc, cs8
    jr.cap	cs8
    cgettag	a7, ca7
    cgettag	s3, cs3
    auipcc	ctp, 641294
    cspecialr	ca6, pcc
    li	a7, -1025
    candperm	ca6, ca6, a7
    cspecialw	mtcc, ca6
    jalr.cap	ca6, cnull
    cgettag	s2, cs2
    cspecialrw	cra, 2, cra
    cgettag	a6, ca6
    cspecialrw	ca7, ddc, cs3
    li	zero, -3
    candperm	ca7, ca7, zero
    cseal	csp, csp, csp
    cseal	ca7, ca7, csp
    cinvoke	csp, ca7
    cmove	ct6, cra
    cspecialr	cs8, pcc
    li	s9, 1
    csetflags	cs8, cs8, s9
    cspecialw	mtcc, cs8
    jr.cap	cs8
    cgettag	a6, ca6
    andi	ra, ra, 255
    lui	ra, 262148
    slli	ra, ra, 1
    add	ra, ra, ra
    sc.ddc	ca7, (ra)
    cseal	cs4, cs4, cra
    cseal	cs4, cs4, cra
    cinvoke	cs4, cs4
    cmove	ct6, cra
    andi	zero, zero, 255
    lui	tp, 262148
    slli	tp, tp, 1
    lc.ddc	cgp, (zero)
    cspecialrw	csp, mepcc, ca6
    csetbounds	cs3, cgp, 19
    li	gp, 2
    slli	gp, gp, 12
    csetoffset	cs3, cs3, gp
    cincoffset	cs3, cs3, -1344
    cgettag	s2, cs2
    cgettag	zero, cnull
    andi	s4, s4, 255
    lui	s2, 262148
    slli	s2, s2, 1
    lc.ddc	ca7, (s4)
    cgettype	s2, cs2
    andi	s2, s2, 255
    lui	ra, 262148
    slli	ra, ra, 1
    sc.cap	cra, (cs2)
    cgettag	a7, ca7
    csetbounds	cs4, cgp, 27
    nop
    slli	zero, zero, 12
    csetoffset	cs4, cs4, zero
    cincoffset	cs4, cs4, 20
    cspecialrw	cs4, pcc, ctp
    cspecialr	cs2, pcc
    li	a6, 0
    slli	a6, a6, 11
    mv	a6, a6
    slli	a6, a6, 11
    mv	a6, a6
    slli	a6, a6, 11
    mv	a6, a6
    slli	a6, a6, 11
    addi	a6, a6, 13
    slli	a6, a6, 8
    addi	a6, a6, 239
    cincoffset	cs2, cs2, a6
    li	a6, 0
    slli	a6, a6, 11
    mv	a6, a6
    slli	a6, a6, 11
    mv	a6, a6
    slli	a6, a6, 11
    addi	a6, a6, 1
    slli	a6, a6, 11
    addi	a6, a6, 542
    slli	a6, a6, 8
    addi	a6, a6, 201
    csetbounds	cs2, cs2, a6
    jalr.cap	cs2, cs2
    li	s3, -3
    candperm	ctp, ctp, s3
    cseal	cs2, cs2, cs4
    cinvoke	cs2, ctp
    cmove	ct6, cra
    cspecialw	ddc, csp
    cgettag	gp, cgp
    cgettag	zero, cnull
    candperm	ctp, ctp, a7
    cgetflags	ra, cgp
    cspecialr	cs8, pcc
    li	s9, 1
    csetflags	cs8, cs8, s9
    cspecialw	mtcc, cs8
    jr.cap	cs8
    cgetperm	s3, ca7
    csetoffset	cra, ctp, s2
    andi	gp, gp, 255
    lui	s2, 262148
    slli	s2, s2, 1
    add	gp, s2, gp
    sc.cap	cs4, (cgp)
    srli	s3, zero, 19
    cspecialr	cs2, pcc
    li	tp, -1025
    candperm	cs2, cs2, tp
    cspecialw	mtcc, cs2
    jalr.cap	cs2, cnull
    cspecialr	cs11, pcc
    li	t3, 0
    csetflags	cs11, cs11, t3
    cspecialw	mtcc, cs11
    jr.cap	cs11
    cspecialr	csp, pcc
    li	zero, -1025
    candperm	csp, csp, zero
    cspecialw	mtcc, csp
    jalr.cap	csp, cnull
    lui	a6, 262148
    slli	a6, a6, 1
    csetaddr	ca7, ca7, a6
    ccleartag	ca6, csp
    sc	ca6, 0(ca7)
    cincoffset	ca7, ca7, 16
    sc	ca6, 0(ca7)
    cincoffset	ca7, ca7, 16
    sc	ca6, 0(ca7)
    cincoffset	ca7, ca7, 16
    sc	ca6, 0(ca7)
    cincoffset	ca7, ca7, 16
    sc	ca6, 0(ca7)
    cincoffset	ca7, ca7, 16
    cincoffset	ca7, ca7, -80
    cloadtags	ra, (ca7)
    cgettag	s2, cs2
    cspecialrw	ctp, mepcc, cs4
    andi	s3, s3, 255
    lui	a7, 262148
    slli	a7, a7, 1
    add	s3, a7, s3
    sc.cap	ca7, (cs3)
    cgettag	sp, csp
    cseal	cs2, cs2, cra
    cseal	csp, csp, cra
    cinvoke	cs2, csp
    cmove	ct6, cra
    andi	a6, a6, 255
    lui	a7, 262148
    slli	a7, a7, 1
    sb	a6, 0(cs2)
    andi	gp, gp, 255
    lui	s4, 262148
    slli	s4, s4, 1
    add	gp, s4, gp
    lh	ra, 0(cgp)
    lui	gp, 262148
    slli	gp, gp, 1
    csetaddr	cnull, cnull, gp
    ccleartag	cgp, cs2
    sc	cs2, 0(cnull)
    cincoffset	cnull, cnull, 16
    sc	cgp, 0(cnull)
    cincoffset	cnull, cnull, 16
    sc	cs2, 0(cnull)
    cincoffset	cnull, cnull, 16
    sc	cs2, 0(cnull)
    cincoffset	cnull, cnull, 16
    sc	cs2, 0(cnull)
    cincoffset	cnull, cnull, 16
    cincoffset	cnull, cnull, -80
    cloadtags	a7, (cnull)
    andi	s2, s2, 255
    lui	s4, 262148
    slli	s4, s4, 1
    sc.ddc	ca7, (s2)
    andi	s2, s2, 255
    lui	ra, 262148
    slli	ra, ra, 1
    lhu	s4, 0(cs2)
    csetbounds	csp, ca6, 5
    li	s2, 0
    slli	s2, s2, 12
    csetoffset	csp, csp, s2
    cincoffset	csp, csp, 19
    andi	zero, zero, 255
    lui	sp, 262148
    slli	sp, sp, 1
    sc.cap	cs3, (cnull)
    cgettag	a6, ca6
    cspecialrw	csp, mtcc, cgp
    lui	s2, 262148
    slli	s2, s2, 1
    csetaddr	cs3, cs3, s2
    ccleartag	cs2, ca6
    sc	cs2, 0(cs3)
    cincoffset	cs3, cs3, 16
    sc	cs2, 0(cs3)
    cincoffset	cs3, cs3, 16
    sc	ca6, 0(cs3)
    cincoffset	cs3, cs3, 16
    sc	ca6, 0(cs3)
    cincoffset	cs3, cs3, 16
    sc	ca6, 0(cs3)
    cincoffset	cs3, cs3, 16
    cincoffset	cs3, cs3, -80
    cloadtags	tp, (cs3)
    csetbounds	cs4, cnull, 7
    li	s2, 2
    slli	s2, s2, 12
    csetoffset	cs4, cs4, s2
    cincoffset	cs4, cs4, -845
    li	a7, -3
    candperm	cs2, cs2, a7
    cseal	cs2, cs2, csp
    cinvoke	csp, cs2
    cmove	ct6, cra
    li	s2, -3
    candperm	ctp, ctp, s2
    cseal	ctp, ctp, cs3
    cseal	ctp, ctp, cs3
    cinvoke	ctp, ctp
    cmove	ct6, cra
    cspecialrw	ca7, mepcc, ca7
    lui	gp, 262148
    slli	gp, gp, 1
    csetaddr	csp, csp, gp
    ccleartag	cgp, csp
    sc	cgp, 0(csp)
    cincoffset	csp, csp, 16
    sc	cgp, 0(csp)
    cincoffset	csp, csp, 16
    sc	cgp, 0(csp)
    cincoffset	csp, csp, 16
    sc	cgp, 0(csp)
    cincoffset	csp, csp, 16
    sc	cgp, 0(csp)
    cincoffset	csp, csp, 16
    cincoffset	csp, csp, -80
    cloadtags	gp, (csp)
    cgettag	ra, cra
    andi	s3, s3, 255
    lui	a6, 262148
    slli	a6, a6, 1
    add	s3, a6, s3
    sw	s3, 0(csp)
    cspecialrw	ca7, ddc, cra
    cgettag	s2, cs2
    cgettag	s3, cs3
    cspecialrw	ca7, mscratchc, ca6
    csetbounds	cgp, csp, 20
    nop
    slli	zero, zero, 12
    csetoffset	cgp, cgp, zero
    cincoffset	cgp, cgp, 26
    csetbounds	csp, cs2, 31
    nop
    slli	zero, zero, 12
    csetoffset	csp, csp, zero
    cincoffset	csp, csp, 23
    cspecialr	cgp, pcc
    li	sp, 0
    slli	sp, sp, 11
    mv	sp, sp
    slli	sp, sp, 11
    mv	sp, sp
    slli	sp, sp, 11
    mv	sp, sp
    slli	sp, sp, 11
    addi	sp, sp, 13
    slli	sp, sp, 8
    addi	sp, sp, 176
    cincoffset	cgp, cgp, sp
    li	sp, 0
    slli	sp, sp, 11
    mv	sp, sp
    slli	sp, sp, 11
    mv	sp, sp
    slli	sp, sp, 11
    addi	sp, sp, 1
    slli	sp, sp, 11
    addi	sp, sp, 283
    slli	sp, sp, 8
    addi	sp, sp, 146
    csetbounds	cgp, cgp, sp
    jalr.cap	cgp, cgp
    cspecialr	cs10, pcc
    li	s11, 1
    csetflags	cs10, cs10, s11
    cspecialw	mtcc, cs10
    jr.cap	cs10
    cspecialrw	cgp, mscratchc, cra
    jalr.cap	cgp, cnull
    cspecialrw	ctp, mscratchc, cs2
    cspecialw	mscratchc, cgp
    li	ra, -3
    candperm	cs4, cs4, ra
    cseal	cs4, cs4, cs3
    cinvoke	ctp, cs4
    cmove	ct6, cra
    cspecialrw	cs3, mscratchc, ca7
    jalr.cap	cs3, cnull
    cspecialrw	csp, mscratchc, cra
    cspecialr	cs3, mscratchc

