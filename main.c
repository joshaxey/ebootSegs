#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/modulemgr.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/types.h>
#include <psp2/io/dirent.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>

#include "graphics.h"

int ebootSegs(void);

int main(void) {

	ebootSegs();

	return 0;
}

int ebootSegs(void) {
	psvDebugScreenInit();

	psvDebugScreenPrintf("*** BEGIN ***\n");

	SceUID module = sceKernelLoadModule("app0:eboot.bin", 0, NULL);

	if (module < 0) {
		return module;
	}

	SceKernelModuleInfo info;
	info.size = sizeof(SceKernelModuleInfo);

	if (sceKernelGetModuleInfo(module, &info) >= 0) {
		int i;
		for (i = 0; i < 4; i++) {
			if (info.segments[i].vaddr) {
				char string[128];
				sprintf(string, "ux0:ebootseg%d.bin", i);
				WriteFile(string, info.segments[i].vaddr, info.segments[i].vsize);
			}
		}
	}

	WriteFile("ux0:moduleinfo.bin", &info, sizeof(SceKernelModuleInfo));

	psvDebugScreenPrintf("*** DONE ***");

	sceKernelDelayThread(10 * 1000 * 1000);

	sceKernelExitProcess(0);

	return 0;
}
