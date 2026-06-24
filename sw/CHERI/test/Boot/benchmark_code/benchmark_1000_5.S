
    andi	gp, gp, 255
    lui	gp, 262148
    slli	gp, gp, 1
    add	gp, gp, gp
    sc.ddc	cnull, (gp)
    cspecialrw	cra, mscratchc, csp
    cspecialrw	cs3, mscratchc, ctp
    cspecialrw	ctp, mscratchc, ca7
    andi	s3, s3, 255
    lui	ra, 262148
    slli	ra, ra, 1
    add	s3, ra, s3
    lc.ddc	cgp, (s3)
    andi	sp, sp, 255
    lui	zero, 262148
    slli	zero, zero, 1
    sw	sp, 0(ca6)
    andi	a6, a6, 255
    lui	ra, 262148
    slli	ra, ra, 1
    sc.ddc	ca6, (a6)
    andi	s2, s2, 255
    lui	gp, 262148
    slli	gp, gp, 1
    sc.ddc	ca6, (s2)
    li	tp, -3
    candperm	cs2, cs2, tp
    cseal	cnull, cnull, ca6
    cseal	cs2, cs2, ca6
    cinvoke	cnull, cs2
    cmove	ct6, cra
    cspecialrw	ctp, mscratchc, csp
    cspecialrw	ca6, mscratchc, cs2
    cspecialrw	cs2, mscratchc, cs2
    andi	tp, tp, 255
    lui	zero, 262148
    slli	zero, zero, 1
    sc.cap	cra, (ctp)
    andi	s4, s4, 255
    lui	s4, 262148
    slli	s4, s4, 1
    lh	gp, 0(cs4)
    csetbounds	cs4, cs2, 27
    li	zero, 1
    slli	zero, zero, 12
    csetoffset	cs4, cs4, zero
    cincoffset	cs4, cs4, 1371
    csetflags	cra, cnull, zero
    cspecialw	mscratchc, cra
    cspecialrw	cra, mscratchc, cs2
    cspecialrw	cra, mscratchc, cra
    cspecialr	cs9, pcc
    li	s10, 1
    csetflags	cs9, cs9, s10
    cspecialw	mtcc, cs9
    cspecialrw	ctp, mepcc, cs4
    andi	ra, ra, 255
    lui	gp, 262148
    slli	gp, gp, 1
    add	ra, gp, ra
    lb	tp, 0(cra)
    andi	gp, gp, 255
    lui	s3, 262148
    slli	s3, s3, 1
    add	gp, s3, gp
    sc.ddc	cra, (gp)
    cspecialrw	csp, mscratchc, ctp
    cspecialrw	csp, mscratchc, ca6
    cspecialr	ca7, mscratchc
    cgetperm	a7, cra
    andi	a7, a7, 255
    lui	ra, 262148
    slli	ra, ra, 1
    add	a7, ra, a7
    sc.cap	cgp, (ca7)
    ccleartag	csp, cs2
    cspecialrw	cs4, mscratchc, cs4
    cspecialrw	cra, mscratchc, csp
    cspecialrw	cs3, mscratchc, ca7
    cspecialrw	cs4, mscratchc, csp
    cspecialrw	csp, mscratchc, cs2
    cspecialrw	ctp, mscratchc, cs2
    cgettag	s2, cs2
    cgettype	s3, ca7
    cspecialr	cs3, pcc
    li	s4, 0
    slli	s4, s4, 11
    mv	s4, s4
    slli	s4, s4, 11
    mv	s4, s4
    slli	s4, s4, 11
    mv	s4, s4
    slli	s4, s4, 11
    addi	s4, s4, 4
    slli	s4, s4, 8
    addi	s4, s4, 169
    cincoffset	cs3, cs3, s4
    li	s4, 0
    slli	s4, s4, 11
    mv	s4, s4
    slli	s4, s4, 11
    mv	s4, s4
    slli	s4, s4, 11
    mv	s4, s4
    slli	s4, s4, 11
    addi	s4, s4, 912
    slli	s4, s4, 8
    addi	s4, s4, 106
    csetbounds	cs3, cs3, s4
    li	sp, -3
    candperm	cs4, cs4, sp
    cseal	cs4, cs4, cs2
    cinvoke	ca7, cs4
    cmove	ct6, cra
    cspecialr	cgp, pcc
    nop
    slli	zero, zero, 11
    nop
    slli	zero, zero, 11
    nop
    slli	zero, zero, 11
    nop
    slli	zero, zero, 11
    li	zero, 1
    slli	zero, zero, 8
    li	zero, 200
    cincoffset	cgp, cgp, zero
    nop
    slli	zero, zero, 11
    nop
    slli	zero, zero, 11
    nop
    slli	zero, zero, 11
    nop
    slli	zero, zero, 11
    li	zero, 1428
    slli	zero, zero, 8
    li	zero, 44
    csetbounds	cgp, cgp, zero
    andi	s4, s4, 255
    lui	tp, 262148
    slli	tp, tp, 1
    sh	s4, 0(cra)
    andi	gp, gp, 255
    lui	sp, 262148
    slli	sp, sp, 1
    add	gp, sp, gp
    sc.cap	csp, (cgp)
    csetbounds	cs2, ca7, 3
    li	s3, 0
    slli	s3, s3, 12
    csetoffset	cs2, cs2, s3
    cincoffset	cs2, cs2, -415
    andi	tp, tp, 255
    lui	tp, 262148
    slli	tp, tp, 1
    sc.ddc	cgp, (tp)
    andi	gp, gp, 255
    lui	a7, 262148
    slli	a7, a7, 1
    add	gp, a7, gp
    sc.cap	cs3, (cgp)
    cgettag	sp, csp
    lui	s2, 262148
    slli	s2, s2, 1
    csetaddr	cs3, cs3, s2
    ccleartag	cs2, cs4
    sc	cs2, 0(cs3)
    cincoffset	cs3, cs3, 16
    sc	cs2, 0(cs3)
    cincoffset	cs3, cs3, 16
    sc	cs2, 0(cs3)
    cincoffset	cs3, cs3, 16
    sc	cs4, 0(cs3)
    cincoffset	cs3, cs3, 16
    sc	cs2, 0(cs3)
    cincoffset	cs3, cs3, 16
    cincoffset	cs3, cs3, -80
    cloadtags	zero, (cs3)
    andi	s4, s4, 255
    lui	s3, 262148
    slli	s3, s3, 1
    sh	s4, 0(csp)
    cgettag	zero, cnull
    cspecialr	cs4, pcc
    li	a6, -1025
    candperm	cs4, cs4, a6
    cspecialw	mtcc, cs4
    andi	zero, zero, 255
    lui	s3, 262148
    slli	s3, s3, 1
    sc.cap	cgp, (cnull)
    cspecialrw	ca6, pcc, ca6
    andi	s2, s2, 255
    lui	a7, 262148
    slli	a7, a7, 1
    sc.ddc	ca7, (s2)
    cgettag	s3, cs3
    andi	s4, s4, 255
    lui	zero, 262148
    slli	zero, zero, 1
    lc.ddc	ca7, (s4)
    csetbounds	cnull, ca6, 9
    li	a7, 0
    slli	a7, a7, 12
    csetoffset	cnull, cnull, a7
    cincoffset	cnull, cnull, 28
    cspecialr	cs4, pcc
    li	ra, 0
    slli	ra, ra, 11
    mv	ra, ra
    slli	ra, ra, 11
    mv	ra, ra
    slli	ra, ra, 11
    mv	ra, ra
    slli	ra, ra, 11
    addi	ra, ra, 7
    slli	ra, ra, 8
    addi	ra, ra, 127
    cincoffset	cs4, cs4, ra
    li	ra, 0
    slli	ra, ra, 11
    mv	ra, ra
    slli	ra, ra, 11
    mv	ra, ra
    slli	ra, ra, 11
    addi	ra, ra, 1
    slli	ra, ra, 11
    addi	ra, ra, 1616
    slli	ra, ra, 8
    addi	ra, ra, 58
    csetbounds	cs4, cs4, ra
    andi	s2, s2, 255
    lui	s4, 262148
    slli	s4, s4, 1
    sc.cap	csp, (cs2)
    andi	ra, ra, 255
    lui	s3, 262148
    slli	s3, s3, 1
    add	ra, s3, ra
    sc.cap	cgp, (cra)
    cgettag	s2, cs2
    cgettag	tp, ctp
    cspecialr	cs3, pcc
    li	gp, 0
    slli	gp, gp, 11
    mv	gp, gp
    slli	gp, gp, 11
    mv	gp, gp
    slli	gp, gp, 11
    mv	gp, gp
    slli	gp, gp, 11
    addi	gp, gp, 10
    slli	gp, gp, 8
    addi	gp, gp, 247
    cincoffset	cs3, cs3, gp
    li	gp, 0
    slli	gp, gp, 11
    mv	gp, gp
    slli	gp, gp, 11
    mv	gp, gp
    slli	gp, gp, 11
    addi	gp, gp, 1
    slli	gp, gp, 11
    addi	gp, gp, 1288
    slli	gp, gp, 8
    addi	gp, gp, 34
    csetbounds	cs3, cs3, gp
    cgettag	ra, cra
    cspecialrw	cra, pcc, cs3
    cspecialr	cs10, pcc
    li	s11, 0
    csetflags	cs10, cs10, s11
    cspecialw	mtcc, cs10
    li	s2, -3
    candperm	csp, csp, s2
    cseal	cra, cra, ca6
    cseal	csp, csp, ca6
    cinvoke	cra, csp
    cmove	ct6, cra
    cspecialrw	cgp, ddc, ctp
    cgettag	s2, cs2
    cspecialr	cs7, pcc
    li	s8, 0
    csetflags	cs7, cs7, s8
    cspecialw	mtcc, cs7
    cspecialr	ctp, mepcc
    cloadtags	s4, (cra)
    andi	a7, a7, 255
    lui	a6, 262148
    slli	a6, a6, 1
    add	a7, a6, a7
    sc.ddc	ca6, (a7)
    andi	s4, s4, 255
    lui	zero, 262148
    slli	zero, zero, 1
    lc.ddc	csp, (s4)
    cgettag	sp, csp
    cgettag	s4, cs4
    cunseal	csp, csp, cs2
    andi	s3, s3, 255
    lui	tp, 262148
    slli	tp, tp, 1
    add	s3, tp, s3
    lhu	ra, 0(cs3)
    andi	s3, s3, 255
    lui	tp, 262148
    slli	tp, tp, 1
    add	s3, tp, s3
    sc.cap	cs4, (cs3)
    csetbounds	cs2, cs3, 30
    li	tp, 1
    slli	tp, tp, 12
    csetoffset	cs2, cs2, tp
    cincoffset	cs2, cs2, -467
    cspecialr	ct3, pcc
    li	t4, 0
    csetflags	ct3, ct3, t4
    cspecialw	mtcc, ct3
    li	sp, -3
    candperm	cra, cra, sp
    cseal	csp, csp, cs4
    cseal	cra, cra, cs4
    cinvoke	csp, cra
    cmove	ct6, cra
    andi	sp, sp, 255
    lui	gp, 262148
    slli	gp, gp, 1
    sb	sp, 0(cs2)
    andi	a6, a6, 255
    lui	s4, 262148
    slli	s4, s4, 1
    sc.ddc	cs2, (a6)
    andi	s3, s3, 255
    lui	gp, 262148
    slli	gp, gp, 1
    add	s3, gp, s3
    sh	s3, 0(cnull)
    cgettype	s4, cgp
    andi	sp, sp, 255
    lui	s3, 262148
    slli	s3, s3, 1
    sc.ddc	cgp, (sp)
    li	a6, -3
    candperm	ctp, ctp, a6
    cseal	cnull, cnull, cs2
    cinvoke	cnull, ctp
    cmove	ct6, cra
    andi	s4, s4, 255
    lui	sp, 262148
    slli	sp, sp, 1
    sw	s4, 0(csp)
    andi	s4, s4, 255
    lui	gp, 262148
    slli	gp, gp, 1
    sc.cap	ca6, (cs4)
    addi	zero, a7, -3
    candperm	ca6, ca6, a7
    li	a7, -3
    candperm	ca7, ca7, a7
    cinvoke	ca6, ca7
    cmove	ct6, cra
    csetbounds	csp, csp, 16
    li	ra, 0
    slli	ra, ra, 12
    csetoffset	csp, csp, ra
    cincoffset	csp, csp, 27
    cspecialr	cra, pcc
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
    addi	a6, a6, 192
    cincoffset	cra, cra, a6
    li	a6, 0
    slli	a6, a6, 11
    mv	a6, a6
    slli	a6, a6, 11
    mv	a6, a6
    slli	a6, a6, 11
    mv	a6, a6
    slli	a6, a6, 11
    addi	a6, a6, 229
    slli	a6, a6, 8
    addi	a6, a6, 60
    csetbounds	cra, cra, a6
    andi	a6, a6, 255
    lui	gp, 262148
    slli	gp, gp, 1
    sw	a6, 0(csp)
    cgettag	s2, cs2
    andi	a7, a7, 255
    lui	zero, 262148
    slli	zero, zero, 1
    add	a7, zero, a7
    sb	a7, 0(ca6)
    lui	s4, 262148
    slli	s4, s4, 1
    csetaddr	cgp, cgp, s4
    ccleartag	cs4, cs4
    sc	cs4, 0(cgp)
    cincoffset	cgp, cgp, 16
    sc	cs4, 0(cgp)
    cincoffset	cgp, cgp, 16
    sc	cs4, 0(cgp)
    cincoffset	cgp, cgp, 16
    sc	cs4, 0(cgp)
    cincoffset	cgp, cgp, 16
    sc	cs4, 0(cgp)
    cincoffset	cgp, cgp, 16
    cincoffset	cgp, cgp, -80
    cloadtags	a7, (cgp)
    cgettag	s3, csp
    li	gp, -3
    candperm	ca7, ca7, gp
    li	gp, -257
    candperm	ca7, ca7, gp
    cseal	cs3, cs3, cs4
    cseal	ca7, ca7, cs4
    cinvoke	cs3, ca7
    cmove	ct6, cra
    andi	a6, a6, 255
    lui	ra, 262148
    slli	ra, ra, 1
    sc.cap	csp, (ca6)
    ori	s4, sp, -1826
    sh	s4, -209(cs4)
    lui	s3, 262148
    slli	s3, s3, 1
    csetaddr	ca7, ca7, s3
    ccleartag	cs3, ca6
    sc	ca6, 0(ca7)
    cincoffset	ca7, ca7, 16
    sc	cs3, 0(ca7)
    cincoffset	ca7, ca7, 16
    sc	ca6, 0(ca7)
    cincoffset	ca7, ca7, 16
    sc	ca6, 0(ca7)
    cincoffset	ca7, ca7, 16
    sc	cs3, 0(ca7)
    cincoffset	ca7, ca7, 16
    cincoffset	ca7, ca7, -80
    cloadtags	s4, (ca7)
    andi	tp, tp, 255
    lui	sp, 262148
    slli	sp, sp, 1
    sc.ddc	ca7, (tp)
    andi	a7, a7, 255
    lui	ra, 262148
    slli	ra, ra, 1
    add	a7, ra, a7
    sc.ddc	cs2, (a7)
    csetbounds	csp, cs4, 29
    li	a6, 2
    slli	a6, a6, 12
    csetoffset	csp, csp, a6
    cincoffset	csp, csp, -1795
    andi	tp, tp, 255
    lui	gp, 262148
    slli	gp, gp, 1
    lc.ddc	cra, (tp)
    cgettag	s4, cs4
    cgettag	s4, cs4
    cspecialr	ca6, pcc
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
    addi	a7, a7, 142
    cincoffset	ca6, ca6, a7
    li	a7, 0
    slli	a7, a7, 11
    mv	a7, a7
    slli	a7, a7, 11
    mv	a7, a7
    slli	a7, a7, 11
    mv	a7, a7
    slli	a7, a7, 11
    addi	a7, a7, 510
    slli	a7, a7, 8
    addi	a7, a7, 88
    csetbounds	ca6, ca6, a7
    csetbounds	cs2, cnull, 10
    li	zero, 3
    slli	zero, zero, 12
    csetoffset	cs2, cs2, zero
    cincoffset	cs2, cs2, -1703
    lui	s4, 262148
    slli	s4, s4, 1
    csetaddr	ca6, ca6, s4
    ccleartag	cs4, csp
    sc	csp, 0(ca6)
    cincoffset	ca6, ca6, 16
    sc	csp, 0(ca6)
    cincoffset	ca6, ca6, 16
    sc	csp, 0(ca6)
    cincoffset	ca6, ca6, 16
    sc	csp, 0(ca6)
    cincoffset	ca6, ca6, 16
    sc	cs4, 0(ca6)
    cincoffset	ca6, ca6, 16
    cincoffset	ca6, ca6, -80
    cloadtags	a7, (ca6)
    cgettag	s2, cs2
    andi	a6, a6, 255
    lui	tp, 262148
    slli	tp, tp, 1
    sc.cap	cnull, (ca6)
    andi	s3, s3, 255
    lui	gp, 262148
    slli	gp, gp, 1
    add	s3, gp, s3
    sc.cap	ctp, (cs3)
    andi	gp, gp, 255
    lui	s4, 262148
    slli	s4, s4, 1
    add	gp, s4, gp
    sc.cap	ctp, (cgp)
    andi	ra, ra, 255
    lui	ra, 262148
    slli	ra, ra, 1
    add	ra, ra, ra
    lc.ddc	cgp, (ra)
    andi	a6, a6, 255
    lui	s2, 262148
    slli	s2, s2, 1
    sc.cap	cnull, (ca6)
    andi	sp, sp, 255
    lui	gp, 262148
    slli	gp, gp, 1
    sc.cap	ctp, (csp)
    li	s3, -3
    candperm	cs2, cs2, s3
    cseal	ca6, ca6, cs2
    cseal	cs2, cs2, cs2
    cinvoke	ca6, cs2
    cmove	ct6, cra
    andi	zero, zero, 255
    lui	sp, 262148
    slli	sp, sp, 1
    sc.ddc	cra, (zero)
    andi	ra, ra, 255
    lui	zero, 262148
    slli	zero, zero, 1
    add	ra, zero, ra
    lc.ddc	cra, (ra)
    cspecialr	cs10, pcc
    li	s11, 1
    csetflags	cs10, cs10, s11
    cspecialw	mtcc, cs10
    cgettag	gp, cgp
    cspecialrw	cra, mscratchc, cgp
    cspecialrw	cs2, mscratchc, ca6
    cspecialrw	csp, mscratchc, ctp
    cgettag	s2, cs2
    andi	s2, s2, 255
    lui	a6, 262148
    slli	a6, a6, 1
    lc.ddc	cs4, (s2)
    andi	sp, sp, 255
    lui	zero, 262148
    slli	zero, zero, 1
    sc.cap	cs3, (csp)
    andi	s3, s3, 255
    lui	s3, 262148
    slli	s3, s3, 1
    add	s3, s3, s3
    sc.cap	cs3, (cs3)
    andi	gp, gp, 255
    lui	sp, 262148
    slli	sp, sp, 1
    add	gp, sp, gp
    sc.ddc	cgp, (gp)
    andi	tp, tp, 255
    lui	gp, 262148
    slli	gp, gp, 1
    sc.cap	cs2, (ctp)
    andi	s2, s2, 255
    lui	s4, 262148
    slli	s4, s4, 1
    sc.cap	ctp, (cs2)
    cspecialr	ctp, pcc
    li	a7, -1025
    candperm	ctp, ctp, a7
    cspecialw	mtcc, ctp
    lui	ra, 262148
    slli	ra, ra, 1
    csetaddr	cgp, cgp, ra
    ccleartag	cra, ca6
    sc	ca6, 0(cgp)
    cincoffset	cgp, cgp, 16
    sc	ca6, 0(cgp)
    cincoffset	cgp, cgp, 16
    sc	ca6, 0(cgp)
    cincoffset	cgp, cgp, 16
    sc	cra, 0(cgp)
    cincoffset	cgp, cgp, 16
    sc	ca6, 0(cgp)
    cincoffset	cgp, cgp, 16
    cincoffset	cgp, cgp, -80
    cloadtags	a7, (cgp)
    li	s4, -3
    candperm	ca6, ca6, s4
    cseal	ca6, ca6, cs3
    cinvoke	ca6, ca6
    cmove	ct6, cra
    cspecialr	cs6, pcc
    li	s7, 1
    csetflags	cs6, cs6, s7
    cspecialw	mtcc, cs6
    lui	s3, 262148
    slli	s3, s3, 1
    csetaddr	cs3, cs3, s3
    ccleartag	cs3, cs2
    sc	cs2, 0(cs3)
    cincoffset	cs3, cs3, 16
    sc	cs2, 0(cs3)
    cincoffset	cs3, cs3, 16
    sc	cs3, 0(cs3)
    cincoffset	cs3, cs3, 16
    sc	cs3, 0(cs3)
    cincoffset	cs3, cs3, 16
    sc	cs3, 0(cs3)
    cincoffset	cs3, cs3, 16
    cincoffset	cs3, cs3, -80
    cloadtags	a6, (cs3)
    xori	sp, s3, 875
    lui	a7, 262148
    slli	a7, a7, 1
    csetaddr	ctp, ctp, a7
    ccleartag	ca7, cnull
    sc	cnull, 0(ctp)
    cincoffset	ctp, ctp, 16
    sc	cnull, 0(ctp)
    cincoffset	ctp, ctp, 16
    sc	cnull, 0(ctp)
    cincoffset	ctp, ctp, 16
    sc	ca7, 0(ctp)
    cincoffset	ctp, ctp, 16
    sc	ca7, 0(ctp)
    cincoffset	ctp, ctp, 16
    cincoffset	ctp, ctp, -80
    cloadtags	zero, (ctp)
    li	s3, -3
    candperm	ca6, ca6, s3
    cseal	cs2, cs2, ca7
    cseal	ca6, ca6, ca7
    cinvoke	cs2, ca6
    cmove	ct6, cra
    andi	ra, ra, 255
    lui	a7, 262148
    slli	a7, a7, 1
    add	ra, a7, ra
    lhu	ra, 0(cra)
    li	sp, -3
    candperm	csp, csp, sp
    cseal	cs4, cs4, cs3
    cinvoke	cs4, csp
    cmove	ct6, cra
    andi	gp, gp, 255
    lui	ra, 262148
    slli	ra, ra, 1
    add	gp, ra, gp
    sc.ddc	csp, (gp)
    cgettag	s2, cs2
    csetbounds	cra, cs4, 25
    li	s4, 2
    slli	s4, s4, 12
    csetoffset	cra, cra, s4
    cincoffset	cra, cra, 1513
    andi	s4, s4, 255
    lui	s2, 262148
    slli	s2, s2, 1
    sc.ddc	ca7, (s4)
    cspecialr	csp, pcc
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
    addi	sp, sp, 122
    cincoffset	csp, csp, sp
    li	sp, 0
    slli	sp, sp, 11
    mv	sp, sp
    slli	sp, sp, 11
    mv	sp, sp
    slli	sp, sp, 11
    mv	sp, sp
    slli	sp, sp, 11
    addi	sp, sp, 743
    slli	sp, sp, 8
    addi	sp, sp, 218
    csetbounds	csp, csp, sp
    csetbounds	cnull, ctp, 18
    li	s4, 1
    slli	s4, s4, 12
    csetoffset	cnull, cnull, s4
    cincoffset	cnull, cnull, -2032
    andi	a6, a6, 255
    lui	gp, 262148
    slli	gp, gp, 1
    sc.cap	cs4, (ca6)
    cspecialrw	cs2, pcc, ca7
    andi	gp, gp, 255
    lui	a7, 262148
    slli	a7, a7, 1
    add	gp, a7, gp
    sc.ddc	cgp, (gp)
    cgettag	s2, cs2
    cgetbase	s4, ctp
    andi	ra, ra, 255
    lui	gp, 262148
    slli	gp, gp, 1
    add	ra, gp, ra
    sc.cap	cnull, (cra)
    cspecialrw	ca7, mscratchc, cs3
    cspecialr	ca7, mscratchc
    cspecialrw	cgp, mscratchc, cs3
    andi	ra, ra, 255
    lui	a6, 262148
    slli	a6, a6, 1
    add	ra, a6, ra
    sc.cap	ctp, (cra)
    cgettag	s3, cs3
    andi	tp, tp, 255
    lui	s4, 262148
    slli	s4, s4, 1
    sc.ddc	cnull, (tp)
    li	ra, -3
    candperm	csp, csp, ra
    cseal	cgp, cgp, ca6
    cseal	csp, csp, ca6
    cinvoke	cgp, csp
    cmove	ct6, cra
    andi	zero, zero, 255
    lui	sp, 262148
    slli	sp, sp, 1
    sc.ddc	cnull, (zero)
    andi	a6, a6, 255
    lui	zero, 262148
    slli	zero, zero, 1
    sc.cap	cgp, (ca6)
    csetbounds	cs4, cs2, 22
    li	s3, 1
    slli	s3, s3, 12
    csetoffset	cs4, cs4, s3
    cincoffset	cs4, cs4, 294
    cgetoffset	ra, cgp
    andi	tp, tp, 255
    lui	ra, 262148
    slli	ra, ra, 1
    sc.ddc	ca7, (tp)
    andi	s2, s2, 255
    lui	gp, 262148
    slli	gp, gp, 1
    sc.cap	cgp, (cs2)
    cspecialrw	cs2, mscratchc, cra
    cspecialrw	cgp, mscratchc, cs4
    cspecialrw	ca6, mscratchc, ctp
    cspecialr	csp, pcc
    li	s4, 0
    slli	s4, s4, 11
    mv	s4, s4
    slli	s4, s4, 11
    mv	s4, s4
    slli	s4, s4, 11
    mv	s4, s4
    slli	s4, s4, 11
    addi	s4, s4, 7
    slli	s4, s4, 8
    addi	s4, s4, 46
    cincoffset	csp, csp, s4
    li	s4, 0
    slli	s4, s4, 11
    mv	s4, s4
    slli	s4, s4, 11
    mv	s4, s4
    slli	s4, s4, 11
    mv	s4, s4
    slli	s4, s4, 11
    addi	s4, s4, 1395
    slli	s4, s4, 8
    addi	s4, s4, 137
    csetbounds	csp, csp, s4
    lui	gp, 262148
    slli	gp, gp, 1
    csetaddr	csp, csp, gp
    ccleartag	cgp, cs4
    sc	cs4, 0(csp)
    cincoffset	csp, csp, 16
    sc	cs4, 0(csp)
    cincoffset	csp, csp, 16
    sc	cs4, 0(csp)
    cincoffset	csp, csp, 16
    sc	cs4, 0(csp)
    cincoffset	csp, csp, 16
    sc	cs4, 0(csp)
    cincoffset	csp, csp, 16
    cincoffset	csp, csp, -80
    cloadtags	a7, (csp)
    andi	tp, tp, 255
    lui	zero, 262148
    slli	zero, zero, 1
    sc.ddc	cs4, (tp)
    andi	s4, s4, 255
    lui	ra, 262148
    slli	ra, ra, 1
    sc.ddc	cs3, (s4)
    csetbounds	cra, csp, 1
    li	s4, 0
    slli	s4, s4, 12
    csetoffset	cra, cra, s4
    cincoffset	cra, cra, 19
    cgettag	a6, ca6
    andi	ra, ra, 255
    lui	s4, 262148
    slli	s4, s4, 1
    add	ra, s4, ra
    sc.cap	cra, (cra)
    cgettag	a7, ca7
    csetbounds	cra, csp, 19
    li	s2, 0
    slli	s2, s2, 12
    csetoffset	cra, cra, s2
    cincoffset	cra, cra, 20
    andi	gp, gp, 255
    lui	tp, 262148
    slli	tp, tp, 1
    add	gp, tp, gp
    lc.ddc	ctp, (gp)
    csetbounds	cs4, cnull, 32
    li	ra, 0
    slli	ra, ra, 12
    csetoffset	cs4, cs4, ra
    cincoffset	cs4, cs4, 3
    lui	a6, 262148
    slli	a6, a6, 1
    csetaddr	csp, csp, a6
    ccleartag	ca6, cs4
    sc	ca6, 0(csp)
    cincoffset	csp, csp, 16
    sc	cs4, 0(csp)
    cincoffset	csp, csp, 16
    sc	ca6, 0(csp)
    cincoffset	csp, csp, 16
    sc	ca6, 0(csp)
    cincoffset	csp, csp, 16
    sc	ca6, 0(csp)
    cincoffset	csp, csp, 16
    cincoffset	csp, csp, -80
    cloadtags	s2, (csp)
    cspecialrw	cs4, mscratchc, ca6
    cspecialrw	cra, mscratchc, csp
    cspecialrw	csp, mscratchc, cra
    cspecialw	mtdc, csp
    cgetbase	s2, cnull
    cgettag	gp, cgp
    cspecialr	cs7, pcc
    li	s8, 0
    csetflags	cs7, cs7, s8
    cspecialw	mtcc, cs7
    andi	tp, tp, 255
    lui	sp, 262148
    slli	sp, sp, 1
    sc.cap	csp, (ctp)
    addi	zero, sp, -3
    candperm	cra, cra, sp
    li	sp, -3
    candperm	ctp, ctp, sp
    li	sp, -257
    candperm	cra, cra, sp
    cseal	cra, cra, ca6
    cseal	ctp, ctp, ca6
    cinvoke	cra, ctp
    cmove	ct6, cra
    li	a7, -3
    candperm	cs4, cs4, a7
    cseal	cs3, cs3, ctp
    cseal	cs4, cs4, ctp
    cinvoke	cs3, cs4
    cmove	ct6, cra
    cgettag	s4, cs4
    li	ra, -3
    candperm	csp, csp, ra
    li	ra, -257
    candperm	cs2, cs2, ra
    cseal	cs2, cs2, csp
    cseal	csp, csp, csp
    cinvoke	cs2, csp
    cmove	ct6, cra
    cspecialrw	cs4, mscratchc, cra
    cspecialrw	csp, mscratchc, csp
    cspecialrw	cs4, mscratchc, ctp
    ccopytype	cs4, cs2, ctp
    andi	zero, zero, 255
    lui	a7, 262148
    slli	a7, a7, 1
    sc.ddc	cra, (zero)
    cspecialrw	cra, mscratchc, ca7
    cspecialw	mscratchc, cs2
    cspecialrw	cs4, mscratchc, ca7
    cgettag	a7, ca7
    # ebreak
    lui	ra, 262148
    slli	ra, ra, 1
    csetaddr	cs4, cs4, ra
    ccleartag	cra, ca7
    sc	cra, 0(cs4)
    cincoffset	cs4, cs4, 16
    sc	cra, 0(cs4)
    cincoffset	cs4, cs4, 16
    sc	ca7, 0(cs4)
    cincoffset	cs4, cs4, 16
    sc	cra, 0(cs4)
    cincoffset	cs4, cs4, 16
    sc	cra, 0(cs4)
    cincoffset	cs4, cs4, 16
    cincoffset	cs4, cs4, -80
    cloadtags	s2, (cs4)
    cgettag	s3, cs3
    andi	zero, zero, 255
    lui	s3, 262148
    slli	s3, s3, 1
    lh	a6, 0(cnull)
    andi	a6, a6, 255
    lui	gp, 262148
    slli	gp, gp, 1
    sc.cap	csp, (ca6)
    cgettag	ra, cra
    cspecialrw	ca7, mepcc, cs4
    cspecialrw	ca6, mscratchc, ca7
    andi	ra, ra, 255
    lui	sp, 262148
    slli	sp, sp, 1
    add	ra, sp, ra
    sc.cap	cgp, (cra)
    andi	s2, s2, 255
    lui	s3, 262148
    slli	s3, s3, 1
    sc.ddc	cs2, (s2)
    andi	s2, s2, 255
    lui	a7, 262148
    slli	a7, a7, 1
    sc.cap	cgp, (cs2)
    csetbounds	cgp, cnull, 2
    li	s2, 0
    slli	s2, s2, 12
    csetoffset	cgp, cgp, s2
    cincoffset	cgp, cgp, 11
    cspecialr	cs2, pcc
    li	zero, -1025
    candperm	cs2, cs2, zero
    cspecialw	mtcc, cs2
    ctestsubset	zero, ddc, cgp
    andi	gp, gp, 255
    lui	s2, 262148
    slli	s2, s2, 1
    add	gp, s2, gp
    sc.ddc	cs2, (gp)
    andi	s2, s2, 255
    lui	tp, 262148
    slli	tp, tp, 1
    sw	s2, 0(ca7)
    andi	tp, tp, 255
    lui	s4, 262148
    slli	s4, s4, 1
    sc.cap	csp, (ctp)
    li	ra, -3
    candperm	ctp, ctp, ra
    cseal	cgp, cgp, csp
    cseal	ctp, ctp, csp
    cinvoke	cgp, ctp
    cmove	ct6, cra
    andi	s3, s3, 255
    lui	s4, 262148
    slli	s4, s4, 1
    add	s3, s4, s3
    lc.ddc	ca6, (s3)
    cspecialrw	csp, mscratchc, cra
    cspecialrw	ca7, mscratchc, cgp
    cspecialrw	cra, mscratchc, cs3
    andi	ra, ra, 255
    lui	sp, 262148
    slli	sp, sp, 1
    add	ra, sp, ra
    sc.cap	cnull, (cra)
    
    
