#include "CPUInfo.h"

using namespace std;

static void cpuID(unsigned i, unsigned regs[4]) {
#ifdef _WIN32
	__cpuid((int *)regs, (int)i);
#else
	asm volatile
		("cpuid" : "=a" (regs[0]), "=b" (regs[1]), "=c" (regs[2]), "=d" (regs[3])
		: "a" (i), "c" (0));
	// ECX is set to zero for CPUID function 4
#endif
}

CPUInfo CPUInfo::get() 
{
	CPUInfo res;

	unsigned regs[4];

	// Get vendor
	char vendor[12];
	cpuID(0, regs);
	((unsigned *)vendor)[0] = regs[1]; // EBX
	((unsigned *)vendor)[1] = regs[3]; // EDX
	((unsigned *)vendor)[2] = regs[2]; // ECX
	res.vendor = string(vendor, 12);

	// Get CPU features
	cpuID(1, regs);
	unsigned cpuFeatures = regs[3]; // EDX

	// Logical core count per CPU
	cpuID(1, regs);
	res.logical = (regs[1] >> 16) & 0xff; // EBX[23:16]
	res.cores = res.logical;

	if (res.vendor == "GenuineIntel") {
		// Get DCP cache info
		cpuID(4, regs);
		res.cores = ((regs[0] >> 26) & 0x3f) + 1; // EAX[31:26] + 1

	} else if (res.vendor == "AuthenticAMD") {
		// Get NC: Number of CPU cores - 1
		cpuID(0x80000008, regs);
		res.cores = ((unsigned)(regs[2] & 0xff)) + 1; // ECX[7:0] + 1
	}

	// Detect hyper-threads  
	res.hyperThreads = cpuFeatures & (1 << 28) && res.cores < res.logical;

	return res;
}

