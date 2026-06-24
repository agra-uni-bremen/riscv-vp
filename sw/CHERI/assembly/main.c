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

unsigned DDC_perms();
unsigned PCC_perms();
unsigned Otype(unsigned choice);
CapEncoding create_ddc_aux();
unsigned getBase(Capability C);
unsigned getTop(Capability C);
unsigned long buildCap(Capability newcap, unsigned authority);
unsigned long newBuildCap(Capability newcap, unsigned authority);
void checktags(unsigned addr);

/*
int main() {
	Capability C = create_ddc_aux();
	
	//unsigned base = getBase(C);
	//return (base);
	
	unsigned top = getTop(C);
	return (top);
}
*/

int main() {
	Capability newcap;
	newcap.perms = DDC_perms();
	newcap.otype = Otype(0);
	newcap.addr = 0b100001111;
	newcap.base = 0b1000000111;
	newcap.len = 0b111111000;
	unsigned long addr = buildCap(newcap, 0);
	
	return (0);
}
