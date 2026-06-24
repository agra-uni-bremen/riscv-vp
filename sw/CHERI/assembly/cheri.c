//By bitwise and, replaces start to end bits by 0
#define REPLACEMENT_MASK(start, end) (~(((0b1 << (end - start + 1)) - 1) << start))
//Set or unset pos bit of instr
#define SET_BIT(instr, pos) (instr | (1 << pos))
#define UNSET_BIT(instr, pos) (instr & (~(1 << pos)))

typedef struct CapEncoding {
	unsigned addr;
	unsigned caps;
} CapEncoding;

typedef struct Capability {
	unsigned addr;
	unsigned base;
	unsigned len;
	unsigned perms;
	unsigned otype;
} Capability;

unsigned DDC_perms() { return (0b111111111101); }

unsigned PCC_perms() { return (0b111110010111); }

unsigned Otype(unsigned choice) {
	switch (choice) {
		case 0: return (0b1111);
		case 1: return (0b1110);
		case 2: return (0b1101);
		default: return (0);
	}
}

//Create an auxiliary encoding of DDC
CapEncoding create_ddc_aux() {
	const unsigned CAP_BE_BOT = 0;
	const unsigned CAP_TE_BOT = 8;
	const unsigned CAP_L7 = 14;
	const unsigned CAP_IE = 15;
	const unsigned CAP_OTYPE_BOT = 16;
	const unsigned CAP_PERM_BOT = 20;
	
	const unsigned ddc_perms = DDC_perms();
	const unsigned ddc_otype = Otype(0);
	const unsigned ddc_addr = 0;
	const unsigned ddc_IE = 0b1;
	const unsigned ddc_L7 = 0b1;
	const unsigned ddc_B = 0b1; //Contains B and BE both
	const unsigned ddc_T = 0b111110; //Contains T and TE both
	
	const unsigned ddc_caps = (ddc_perms << CAP_PERM_BOT) | (ddc_otype << CAP_OTYPE_BOT) | (ddc_IE << CAP_IE) | \
			(ddc_L7 << CAP_L7) | (ddc_T << CAP_TE_BOT) | (ddc_B << CAP_BE_BOT);
			
	CapEncoding ddc_aux;
	ddc_aux.addr = ddc_addr;
	ddc_aux.caps = ddc_caps;
	
	return (ddc_aux);
}

//To set arbitrary permissions to a capability
CapEncoding setPerms(CapEncoding C, unsigned newperms) {
	const unsigned CAP_PERM_BOT = 20;
	const unsigned CAP_PERM_TOP = 31;
	C.caps &= REPLACEMENT_MASK(CAP_PERM_BOT, CAP_PERM_TOP);
	C.caps |= (newperms << CAP_PERM_BOT);
	
	return (C);
}

//To set object type of a capability
CapEncoding setOtype(CapEncoding C, unsigned otype) {
	const unsigned CAP_OTYPE_BOT = 16;
	const unsigned CAP_OTYPE_TOP = 19;
	C.caps &= REPLACEMENT_MASK(CAP_OTYPE_BOT, CAP_OTYPE_TOP);
	C.caps |= ( (otype & Otype(0)) << CAP_OTYPE_BOT );
	
	return (C);
}

unsigned getBase(CapEncoding C) {
	unsigned base;
	asm volatile (
		"add a2, zero, %[x]\n\t"
		"csethigh a2, a2, %[y]\n\t"
		"cgetbase %0, a2\n\t"
		: "=r" (base)
		: [x] "r" (C.addr), [y] "r" (C.caps)
		: "a2"	
	);
	
	return (base);	
}



unsigned getTop(CapEncoding C) {
	unsigned top;
	asm volatile (
		"add a2, zero, %[x]\n\t"
		"csethigh a2, a2, %[y]\n\t"
		"cgettop %0, a2\n\t"
		: "=r" (top)
		: [x] "r" (C.addr), [y] "r" (C.caps)
		: "a2"	
	);
	
	return (top);
}

unsigned long buildCap(Capability newcap, unsigned authority) {
	
	switch (authority) {
		case 0:
		CapEncoding C = create_ddc_aux();
		C = setPerms(C, newcap.perms);
		C = setOtype(C, newcap.otype);
		unsigned mem_addr = 0x10;
		//unsigned mem_addr = 0x100;
		unsigned mem_ptr = 0x11;
		asm volatile (
		    "add a2, zero, %[x]\n\t"
		    "csethigh a2, a2, %[y]\n\t"
		    "csetbounds a2, a2, %[l]\n\t"
		    "csetaddr a2, a2, %[z]\n\t"
		    "cbuildcap a3, zero, a2\n\t" /* authority is cs1 */
		    "cgetbase t0, a3\n\t"
		    "cgettop t1, a3\n\t"
		    "scddc %[addr], a3\n\t"
		    "lcddc a4, %[addr]\n\t"
		    //"cfromptr a2, a3, %[w]\n\t"
		    //"cloadtags t2, a4\n\t"
		    : /* Outputs */
		    : [x] "r" (newcap.base), 
		      [y] "r" (C.caps),
		      [l] "r" (newcap.len),
		      [z] "r" (newcap.addr),
		      [w] "r" (mem_ptr),
		      [addr] "r" (mem_addr) /* Inputs */
		    : "t0", "t1", "t2", "a2", "a3", "a4"  /* Clobbers */
		);
		return (mem_addr);
		
		default:
		return (-1);
	}
}


unsigned long newBuildCap(Capability newcap, unsigned authority) {

    unsigned long rs2_val = 0;          
    unsigned long rand_addr_a2[100];

    // Generate random test addresses
    for (int i = 0; i < 100; i++) {
        rand_addr_a2[i] = 0x6000 + (i * 32);   // a2: no tag
    }

    switch (authority) {
    case 0: {
        CapEncoding C = create_ddc_aux();
        C = setPerms(C, newcap.perms);
        C = setOtype(C, newcap.otype);

        // Build the capability ONCE
        unsigned long cap_val;
        asm volatile (
            "add a2, zero, %[x]\n\t"
            "csethigh a2, a2, %[y]\n\t"
            "csetbounds a2, a2, %[l]\n\t"
            "csetaddr a2, a2, %[z]\n\t"
            "cbuildcap a3, zero, a2\n\t"
            "mv %[out], a3\n\t"
            : [out] "=r" (cap_val)
            : [x] "r" (newcap.base),
              [y] "r" (C.caps),
              [l] "r" (newcap.len),
              [z] "r" (newcap.addr)
            : "a2", "a3"
        );

        // Execute scddc 100 times using the pre-built capability
        for (int i = 0; i < 100; i++) {
            rs2_val = rand_addr_a2[i];
            asm volatile (
                /* store prebuilt capability into memory */
                "scddc %[addr], %[cap]\n\t"       // store tagged
                "scddc %[addr], %[val]\n\t"       // store untagged
                :
                : [addr] "r" (rs2_val), 
                  [cap] "r" (cap_val),
                  [val] "r" (rs2_val)
                : "a2", "a3"
            );
        }

        return rs2_val;   // return last tagged-address
    }

    default:
        return -1;
    }
}


void checktags(unsigned addr) {
	CapEncoding C = create_ddc_aux();
	asm volatile (
		"add a5, zero, %[x]\n\t"
		"csethigh a5, a5, %[y]\n\t"
		"cbuildcap a6, zero, a5\n\t"
		"cloadtags t2, a6\n\t"
		: // Outputs
		: [x] "r" (addr),
		  [y] "r" (C.caps) // Inputs
		: "t2", "a5", "a6" // Clobbers
	);
	return;
}

