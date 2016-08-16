#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <psp2/moduleinfo.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/modulemgr.h>
#include <psp2/io/dirent.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>

#include "graphics.h"

int WriteFile(char *file, void *buf, int size) {
	SceUID fd = sceIoOpen(file, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, 0777);
	if (fd < 0)
		return fd;

	int written = sceIoWrite(fd, buf, size);

	sceIoClose(fd);
	return written;
}

int ebootSegs(void) {
	psvDebugScreenInit();
	psvDebugScreenPrintf("*** BEGIN ***\n");

	SceUID module = 0;
	
	module = sceKernelLoadModule("app0:eboot.bin", 0, NULL);

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
				WriteFile(string, info.segments[i].vaddr, info.segments[i].memsz);
			}
		}
	}

	WriteFile("ux0:moduleinfo.bin", &info, sizeof(SceKernelModuleInfo));
	
	return 0;
}

int main(int argc, char *argv[]) {
	psvDebugScreenInit();
	psvDebugScreenPrintf("*** DONE ***\n");

	ebootSegs();
	
	psvDebugScreenPrintf("Tool will close in 10 seconds!\n");
	
	sceKernelDelayThread(10*1000*1000);

	sceKernelExitProcess(0);
	
	return 0;
}
