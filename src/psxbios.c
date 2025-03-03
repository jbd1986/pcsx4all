/***************************************************************************
 *   Copyright (C) 2007 Ryan Schultz, PCSX-df Team, PCSX team              *
 *   schultz.ryan@gmail.com, http://rschultz.ath.cx/code.php               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Steet, Fifth Floor, Boston, MA 02111-1307 USA.            *
 ***************************************************************************/

/*
* Internal emulated HLE BIOS.
*/

#include "psxbios.h"
#include "psxhw.h"
#include "gpu.h"
#include <zlib.h>

//We try to emulate bios :) HELP US :P

//#define PSXBIOS_LOG printf
#ifdef PSXBIOS_LOG

char *biosA0n[256] = {
// 0x00
	"open",		"lseek",	"read",		"write",
	"close",	"ioctl",	"exit",		"sys_a0_07",
	"getc",		"putc",		"todigit",	"atof",
	"strtoul",	"strtol",	"abs",		"labs",
// 0x10
	"atoi",		"atol",		"atob",		"setjmp",
	"longjmp",	"strcat",	"strncat",	"strcmp",
	"strncmp",	"strcpy",	"strncpy",	"strlen",
	"index",	"rindex",	"strchr",	"strrchr",
// 0x20
	"strpbrk",	"strspn",	"strcspn",	"strtok",
	"strstr",	"toupper",	"tolower",	"bcopy",
	"bzero",	"bcmp",		"memcpy",	"memset",
	"memmove",	"memcmp",	"memchr",	"rand",
// 0x30
	"srand",	"qsort",	"strtod",	"malloc",
	"free",		"lsearch",	"bsearch",	"calloc",
	"realloc",	"InitHeap",	"_exit",	"getchar",
	"putchar",	"gets",		"puts",		"printf",
// 0x40
	"sys_a0_40",		"LoadTest",					"Load",		"Exec",
	"FlushCache",		"InstallInterruptHandler",	"GPU_dw",	"mem2vram",
	"SendGPUStatus",	"GPU_cw",					"GPU_cwb",	"SendPackets",
	"sys_a0_4c",		"GetGPUStatus",				"GPU_sync",	"SystemError",
// 0x50
	"SystemError",		"LoadExec",				"GetSysSp",		"SystemError",
	"_96_init()",		"_bu_init()",			"_96_remove()",	"ReturnZero",
	"ReturnZero",		"ReturnZero",			"ReturnZero",	"dev_tty_init",
	"dev_tty_open",		"sys_a0_5d",			"dev_tty_ioctl","dev_cd_open",
// 0x60
	"dev_cd_read",		"dev_cd_close",			"dev_cd_firstfile",	"dev_cd_nextfile",
	"dev_cd_chdir",		"dev_card_open",		"dev_card_read",	"dev_card_write",
	"dev_card_close",	"dev_card_firstfile",	"dev_card_nextfile","dev_card_erase",
	"dev_card_undelete","dev_card_format",		"dev_card_rename",	"dev_card_6f",
// 0x70
	"_bu_init",			"_96_init",		"_96_remove",		"ReturnZero",
	"ReturnZero",		"ReturnZero",	"ReturnZero",		"ReturnZero",
	"_96_CdSeekL",		"ReturnZero",	"ReturnZero",		"ReturnZero",
	"_96_CdGetStatus",	"sys_a0_7d",	"_96_CdRead",		"ReturnZero",
// 0x80
	"ReturnZero",		"sys_a0_81",	"ReturnZero",		"ReturnZero",
	"ReturnZero",		"_96_CdStop",	"ReturnZero",		"ReturnZero",
	"ReturnZero",		"ReturnZero",	"ReturnZero",		"ReturnZero",
	"ReturnZero",		"ReturnZero",	"ReturnZero",		"ReturnZero",
// 0x90
	"sys_a0_90",		"sys_a0_91",	"sys_a0_92",		"sys_a0_93",
	"sys_a0_94",		"sys_a0_95",	"AddCDROMDevice",	"AddMemCardDevide",
	"DisableKernelIORedirection",		"EnableKernelIORedirection", "SystemError", "SystemError",
	"SetConf",			"GetConf",		"sys_a0_9e",		"SetMem",
// 0xa0
	"_boot",			"SystemError",	"EnqueueCdIntr",	"DequeueCdIntr",
	"sys_a0_a4",		"ReadSector",	"get_cd_status",	"bufs_cb_0",
	"bufs_cb_1",		"bufs_cb_2",	"bufs_cb_3",		"_card_info",
	"_card_load",		"_card_auto",	"bufs_cd_4",		"sys_a0_af",
// 0xb0
	"sys_a0_b0",		"sys_a0_b1",	"do_a_long_jmp",	"sys_a0_b3",
	"?? sub_function",
};

char *biosB0n[256] = {
// 0x00
	"SysMalloc",		"sys_b0_01",	"sys_b0_02",	"sys_b0_03",
	"sys_b0_04",		"sys_b0_05",	"sys_b0_06",	"DeliverEvent",
	"OpenEvent",		"CloseEvent",	"WaitEvent",	"TestEvent",
	"EnableEvent",		"DisableEvent",	"OpenTh",		"CloseTh",
// 0x10
	"ChangeTh",			"sys_b0_11",	"InitPAD",		"StartPAD",
	"StopPAD",			"PAD_init",		"PAD_dr",		"ReturnFromExecption",
	"ResetEntryInt",	"HookEntryInt",	"SystemError",	"SystemError",
	"SystemError",		"SystemError",	"SystemError",	"SystemError",
// 0x20
	"UnDeliverEvent",	"SystemError",	"SystemError",	"SystemError",
	"sys_b0_24",		"sys_b0_25",	"sys_b0_26",	"sys_b0_27",
	"sys_b0_28",		"sys_b0_29",	"SystemError",	"SystemError",
	"sys_b0_2c",		"sys_b0_2d",	"sys_b0_2e",	"sys_b0_2f",
// 0x30
	"sys_b0_30",		"sys_b0_31",	"open",			"lseek",
	"read",				"write",		"close",		"ioctl",
	"exit",				"sys_b0_39",	"getc",			"putc",
	"getchar",			"putchar",		"gets",			"puts",
// 0x40
	"cd",				"format",		"firstfile",	"nextfile",
	"rename",			"delete",		"undelete",		"AddDevice",
	"RemoteDevice",		"PrintInstalledDevices", "InitCARD", "StartCARD",
	"StopCARD",			"sys_b0_4d",	"_card_write",	"_card_read",
// 0x50
	"_new_card",		"Krom2RawAdd",	"SystemError",	"sys_b0_53",
	"_get_errno",		"_get_error",	"GetC0Table",	"GetB0Table",
	"_card_chan",		"sys_b0_59",	"SystemError",	"ChangeClearPAD",
	"_card_status",		"_card_wait",
};

char *biosC0n[256] = {
// 0x00
	"InitRCnt",			  "InitException",		"SysEnqIntRP",		"SysDeqIntRP",
	"get_free_EvCB_slot", "get_free_TCB_slot",	"ExceptionHandler",	"InstallExeptionHandler",
	"SysInitMemory",	  "SysInitKMem",		"ChangeClearRCnt",	"SystemError",
	"InitDefInt",		  "sys_c0_0d",			"ReturnZero",		"ReturnZero",
// 0x10
	"ReturnZero",		  "ReturnZero",			"InstallDevices",	"FlushStfInOutPut",
	"ReturnZero",		  "_cdevinput",			"_cdevscan",		"_circgetc",
	"_circputc",		  "ioabort",			"sys_c0_1a",		"KernelRedirect",
	"PatchAOTable",
};

#endif

//#define r0 (psxRegs.GPR.n.r0)
#define at (psxRegs.GPR.n.at)
#define v0 (psxRegs.GPR.n.v0)
#define v1 (psxRegs.GPR.n.v1)
#define a0 (psxRegs.GPR.n.a0)
#define a1 (psxRegs.GPR.n.a1)
#define a2 (psxRegs.GPR.n.a2)
#define a3 (psxRegs.GPR.n.a3)
#define t0 (psxRegs.GPR.n.t0)
#define t1 (psxRegs.GPR.n.t1)
#define t2 (psxRegs.GPR.n.t2)
#define t3 (psxRegs.GPR.n.t3)
#define t4 (psxRegs.GPR.n.t4)
#define t5 (psxRegs.GPR.n.t5)
#define t6 (psxRegs.GPR.n.t6)
#define t7 (psxRegs.GPR.n.t7)
#define t8 (psxRegs.GPR.n.t8)
#define t9 (psxRegs.GPR.n.t9)
#define s0 (psxRegs.GPR.n.s0)
#define s1 (psxRegs.GPR.n.s1)
#define s2 (psxRegs.GPR.n.s2)
#define s3 (psxRegs.GPR.n.s3)
#define s4 (psxRegs.GPR.n.s4)
#define s5 (psxRegs.GPR.n.s5)
#define s6 (psxRegs.GPR.n.s6)
#define s7 (psxRegs.GPR.n.s7)
#define k0 (psxRegs.GPR.n.k0)
#define k1 (psxRegs.GPR.n.k1)
#define gp (psxRegs.GPR.n.gp)
#define sp (psxRegs.GPR.n.sp)
#define fp (psxRegs.GPR.n.s8)
#define ra (psxRegs.GPR.n.ra)
#define pc0 (psxRegs.pc)

#define Ra0 ((char*)PSXM(a0))
#define Ra1 ((char*)PSXM(a1))
#define Ra2 ((char*)PSXM(a2))
#define Ra3 ((char*)PSXM(a3))
#define Rv0 ((char*)PSXM(v0))
#define Rsp ((char*)PSXM(sp))


typedef struct {
	u32 desc;
	s32 status;
	s32 mode;
	u32 fhandler;
} EvCB[32];

#define EvStUNUSED	0x0000
#define EvStWAIT	0x1000
#define EvStACTIVE	0x2000
#define EvStALREADY 0x4000

#define EvMdINTR	0x1000
#define EvMdNOINTR	0x2000

typedef struct {
	s32 status;
	s32 mode;
	u32 reg[32];
	u32 func;
} TCB;

typedef struct {                   
	u32 _pc0;      
	u32 gp0;      
	u32 t_addr;   
	u32 t_size;   
	u32 d_addr;   
	u32 d_size;   
	u32 b_addr;   
	u32 b_size;   
	u32 S_addr;
	u32 s_size;
	u32 _sp,_fp,_gp,ret,base;
} EXEC;

struct DIRENTRY {
	/* 0Ah-1Eh Filename in ASCII, terminated by 00h (max 20 chars, plus ending 00h) */
	char name[20];
	s32 attr;
	s32 size;
	u32 next;
	s32 head;
	char system[4];
};

typedef struct {
	char name[32];
	u32  mode;
	u32  offset;
	u32  size;
	u32  mcfile;
} FileDesc;

static u32 *jmp_int = NULL;
static int *pad_buf = NULL;
static char *pad_buf1 = NULL, *pad_buf2 = NULL;
static int pad_buf1len, pad_buf2len;//shadow add
static int pad_stopped = 0;

static u32 regs[35];
static EvCB *Event;
static EvCB *HwEV; // 0xf0
static EvCB *EvEV; // 0xf1
static EvCB *RcEV; // 0xf2
static EvCB *UeEV; // 0xf3
static EvCB *SwEV; // 0xf4
static EvCB *ThEV; // 0xff
static u32 *heap_addr = NULL;
static u32 *heap_end = NULL;
static u32 heap_size = 0;
static u32 SysIntRP[8];
static int CardState = -1;
static TCB Thread[8];
static int CurThread = 0;
static FileDesc FDesc[32];
static u32 card_active_chan;

/* To avoid any issues with different behaviour when using the libc's own strlen instead.
 * We want to mimic the PSX's behaviour in this case for bufile. */
static size_t strlen_internal(char* p) 
{
	size_t size_of_array = 0;
	while (*p++) size_of_array++;
	return size_of_array;
}

INLINE void softCall(u32 pc) {
	pc0 = pc;
	ra = 0x80001000;

	while (pc0 != 0x80001000) psxCpu->ExecuteBlock(0x80001000);
}

INLINE void softCall2(u32 pc) {
	u32 sra = ra;
	pc0 = pc;
	ra = 0x80001000;

	while (pc0 != 0x80001000) psxCpu->ExecuteBlock(0x80001000);
	ra = sra;
}

INLINE void DeliverEvent(u32 ev, u32 spec) {
	if (Event[ev][spec].status != EvStACTIVE) return;

//	Event[ev][spec].status = EvStALREADY;
	if (Event[ev][spec].mode == EvMdINTR) {
		softCall2(Event[ev][spec].fhandler);
	} else Event[ev][spec].status = EvStALREADY;
}

static unsigned interrupt_r26=0x8004E8B0;

INLINE void SaveRegs(void) {
	memcpy(regs, psxRegs.GPR.r, 32*4);
	regs[32] = psxRegs.GPR.n.lo;
	regs[33] = psxRegs.GPR.n.hi;
	regs[34] = psxRegs.pc;
}

INLINE void LoadRegs(void) {
	memcpy(psxRegs.GPR.r, regs, 32*4);
	psxRegs.GPR.n.lo = regs[32];
	psxRegs.GPR.n.hi = regs[33];
}

/* Bu functions calls */

char ffile[64], *pfile;
int nfile;

static void buopen(int mcd, char *ptr, u8 cfg)
{
	int i;
	char *fptr = ptr;

	strcpy(FDesc[1 + mcd].name, Ra0+5);
	FDesc[1 + mcd].offset = 0;
	FDesc[1 + mcd].mode   = a1;

	for (i=1; i<16; i++) {
		fptr += 128;
		if ((*fptr & 0xF0) != 0x50) continue;
		if (strcmp(FDesc[1 + mcd].name, fptr+0xa)) continue;
		FDesc[1 + mcd].mcfile = i;
		v0 = 1 + mcd;
		break;
	}
	if (a1 & 0x200 && v0 == -1) { /* FCREAT */
		fptr = ptr;
		for (i=1; i<16; i++) {
			int j, xord, nblk = a1 >> 16;
			char *pptr, *fptr2;

			fptr += 128;
			if ((*fptr & 0xF0) != 0xa0) continue;

			FDesc[1 + mcd].mcfile = i;
			fptr[0] = 0x51;
			fptr[4] = 0x00;
			fptr[5] = 0x20 * nblk;
			fptr[6] = 0x00;
			fptr[7] = 0x00;
			strcpy(fptr+0xa, FDesc[1 + mcd].name);
			pptr = fptr2 = fptr;
			for(j=2; j<=nblk; j++) {
				int k;
				for(i++; i<16; i++) {
					fptr2 += 128;

					memset(fptr2, 0, 128);
					fptr2[0] = j < nblk ? 0x52 : 0x53;
					pptr[8] = i - 1;
					pptr[9] = 0;
					for (k=0, xord=0; k<127; k++) xord^= pptr[k];
					pptr[127] = xord;
					pptr = fptr2;
					break;
				}
				/* shouldn't this return ENOSPC if i == 16? */
			}
			pptr[8] = pptr[9] = 0xff;
			for (j=0, xord=0; j<127; j++) xord^= pptr[j];
			pptr[127] = xord;
			v0 = 1 + mcd;
			/* just go ahead and resave them all */
			enum MemcardNum mcd_num = (cfg == 0) ? MCD1 : MCD2;
			sioMcdWrite(mcd_num, ptr, 128, 128 * 15);
			break;
		}
		/* shouldn't this return ENOSPC if i == 16? */
	}
}

#define buread(Ra1, mcd, length) { \
	/*printf("read %d: %x,%x (%s)\n", FDesc[1 + mcd].mcfile, FDesc[1 + mcd].offset, length, Mcd##mcd##Data + 128 * FDesc[1 + mcd].mcfile + 0xa);*/ \
	unsigned offset = 8192 * FDesc[1 + mcd].mcfile + FDesc[1 + mcd].offset; \
	sioMcdRead(((mcd == 1) ? MCD1 : MCD2), (char*)Ra1, offset, length); \
	if (FDesc[1 + mcd].mode & 0x8000) { \
	DeliverEvent(0x11, 0x2); /* 0xf0000011, 0x0004 */ \
	DeliverEvent(0x81, 0x2); /* 0xf4000001, 0x0004 */ \
	v0 = 0; } \
	else v0 = length; \
	FDesc[1 + mcd].offset += v0; \
}

#define buwrite(Ra1, mcd, length) { \
	unsigned offset = 8192 * FDesc[1 + mcd].mcfile + FDesc[1 + mcd].offset; \
	/*printf("write %d: %x,%x\n", FDesc[1 + mcd].mcfile, FDesc[1 + mcd].offset, length);*/ \
	sioMcdWrite((mcd==1) ? MCD1 : MCD2, (const char*)Ra1, offset, length); \
	FDesc[1 + mcd].offset += length; \
	if (FDesc[1 + mcd].mode & 0x8000) { \
	DeliverEvent(0x11, 0x2); /* 0xf0000011, 0x0004 */ \
	DeliverEvent(0x81, 0x2); /* 0xf4000001, 0x0004 */ \
	v0 = 0; } \
	else v0 = length; \
}

#define bufile(mcd) { \
	int i; \
	const char *mcd_data = sioMcdDataPtr((mcd==1) ? MCD1 : MCD2); \
	size_t size_of_name = strlen_internal(dir->name); \
	while (nfile < 16) { \
		int match=1; \
 \
		const char *ptr = mcd_data + 128 * (nfile + 1); \
		nfile++; \
		if ((*ptr & 0xF0) != 0x50) continue; \
		/* Bug link files show up as free block. */ \
		if (!ptr[0xa]) continue; \
		ptr+= 0xa; \
		if (pfile[0] == 0) { \
			strncpy(dir->name, ptr, sizeof(dir->name) - 1); \
			if (size_of_name < sizeof(dir->name)) dir->name[size_of_name] = '\0'; \
		} else for (i=0; i<20; i++) { \
			if (pfile[i] == ptr[i]) { \
				dir->name[i] = ptr[i]; \
				if (ptr[i] == 0) break; else continue; } \
			if (pfile[i] == '?') { \
				dir->name[i] = ptr[i]; continue; } \
			if (pfile[i] == '*') { \
				strcpy(dir->name+i, ptr+i); break; } \
			match = 0; break; \
		} \
		/*printf("%d : %s = %s + %s (match=%d)\n", nfile, dir->name, pfile, ptr, match);*/ \
		if (match == 0) { continue; } \
		dir->size = 8192; \
		v0 = _dir; \
		break; \
	} \
}

#define burename(mcd) { \
	int i; \
	enum MemcardNum mcd_num = (mcd == 1) ? MCD1 : MCD2; \
	char *mcd_data = sioMcdDataPtr(mcd_num); \
	for (i=1; i<16; i++) { \
		int namelen, j, cxor = 0; \
		char *ptr = mcd_data + 128 * i; \
		if ((*ptr & 0xF0) != 0x50) continue; \
		if (strcmp(Ra0+5, ptr+0xa)) continue; \
		namelen = strlen(Ra1+5); \
		memcpy(ptr+0xa, Ra1+5, namelen); \
		memset(ptr+0xa+namelen, 0, 0x75-namelen); \
		for (j=0; j<127; j++) cxor^= ptr[j]; \
		ptr[127] = cxor; \
		sioMcdWrite(mcd_num, NULL, 128 * i + 0xa, 0x76); \
		v0 = 1; \
		break; \
	} \
}

#define budelete(mcd) { \
	enum MemcardNum mcd_num = (mcd == 1) ? MCD1 : MCD2; \
	char *mcd_data = sioMcdDataPtr(mcd_num); \
	int i; \
	for (i=1; i<16; i++) { \
		char *ptr = mcd_data + 128 * i; \
		if ((*ptr & 0xF0) != 0x50) continue; \
		if (strcmp(Ra0+5, ptr+0xa)) continue; \
		*ptr = (*ptr & 0xf) | 0xA0; \
		sioMcdWrite(mcd_num, NULL, 128 * i, 1); \
		/*printf("delete %s\n", ptr+0xa);*/ \
		v0 = 1; \
		break; \
	} \
}



/*                                           *
//                                           *
//                                           *
//               System calls A0             */

/* Internally redirects to "FileRead(fd,tempbuf,1)".*/
/* For some strange reason, the returned character is sign-expanded; */
/* So if a return value of FFFFFFFFh could mean either character FFh, or error. */
void psxBios_getc(void) // 0x03, 0x35
{
	void *pa1 = Ra1;
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosA0n[0x03]);
#endif
	v0 = -1;

	if (pa1) {
		switch (a0) {
			case 2: buread(pa1, 1, 1); break;
			case 3: buread(pa1, 2, 1); break;
		}
	}
	
	v0 = (s32)v0;
	pc0 = ra;
}

/* Copy of psxBios_write, except size is 1. */
void psxBios_putc(void) // 0x09, 0x3B
{
	void *pa1 = Ra1;
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosA0n[0x09]);
#endif
	v0 = -1;
	if (!pa1) {
		pc0 = ra;
		return;
	}

	if (a0 == 1) { // stdout
		char *ptr = (char *)pa1;

		v0 = a2;
		while (a2 > 0) {
			printf("%c", *ptr++); a2--;
		}
		pc0 = ra; return;
	}

	switch (a0) {
		case 2: buwrite(pa1, 1, 1); break;
		case 3: buwrite(pa1, 2, 1); break;
	}

	pc0 = ra;
}

void psxBios_todigit(void) // 0x0a
{
	int c = a0;
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosA0n[0x0a]);
#endif
	c &= 0xFF;
	if (c >= 0x30 && c < 0x3A) {
		c -= 0x30;
	}
	else if (c > 0x60 && c < 0x7B) {
		c -= 0x20;
	}
	else if (c > 0x40 && c < 0x5B) {
		c = c - 0x41 + 10;
	}
	else if (c >= 0x80) {
		c = -1;
	}
	else
	{
		c = 0x0098967F;
	}
	v0 = c;
	pc0 = ra;
}

void psxBios_abs(void) { // 0x0e
	if ((s32)a0 < 0) v0 = -(s32)a0;
	else v0 = a0;
	pc0 = ra;
}

void psxBios_labs(void) { // 0x0f
	psxBios_abs();
}

void psxBios_atoi(void) { // 0x10
	s32 n = 0, f = 0;
	char *p = (char *)Ra0;

	for (;;p++) {
		switch (*p) {
			case ' ': case '\t': continue;
			case '-': f++;
			case '+': p++;
		}
		break;
	}

	while (*p >= '0' && *p <= '9') {
		n = n * 10 + *p++ - '0';
	}

	v0 = (f ? -n : n);
	pc0 = ra;
}

void psxBios_atol(void) { // 0x11
	psxBios_atoi();
}

void psxBios_setjmp(void) { // 13
	u32 *jmp_buf= (u32*)Ra0;
	int i;

#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosA0n[0x13]);
#endif

	jmp_buf[0] = ra;
	jmp_buf[1] = sp;
	jmp_buf[2] = fp;
	for (i=0; i<8; i++) // s0-s7
		jmp_buf[3+i] = psxRegs.GPR.r[16+i];
	jmp_buf[11] = gp;

	v0 = 0; pc0 = ra;
}

void psxBios_longjmp(void) { //14
	u32 *jmp_buf= (u32*)Ra0;
	int i;

#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosA0n[0x14]);
#endif

	ra = jmp_buf[0]; /* ra */
	sp = jmp_buf[1]; /* sp */
	fp = jmp_buf[2]; /* fp */
	for (i=0; i<8; i++) // s0-s7
	   psxRegs.GPR.r[16+i] = jmp_buf[3+i];		
	gp = jmp_buf[11]; /* gp */

	v0 = a1; pc0 = ra;
}

void psxBios_strcat(void) { // 0x15
	char *p1 = (char *)Ra0, *p2 = (char *)Ra1;

#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s: %s, %s\n", biosA0n[0x15], Ra0, Ra1);
#endif
	if (a0 == 0 || a1 == 0)
	{
		v0 = 0;
		pc0 = ra;
		return;
	}
	while (*p1++);
	--p1;
	while ((*p1++ = *p2++) != '\0');

	v0 = a0; pc0 = ra;
}

void psxBios_strncat(void) { // 0x16
	char *p1 = (char *)Ra0, *p2 = (char *)Ra1;
	s32 n = a2;

#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s: %s (%x), %s (%x), %d\n", biosA0n[0x16], Ra0, a0, Ra1, a1, a2);
#endif
	if (a0 == 0 || a1 == 0)
	{
		v0 = 0;
		pc0 = ra;
		return;
	}
	while (*p1++);
	--p1;
	while ((*p1++ = *p2++) != '\0') {
		if (--n < 0) {
			*--p1 = '\0';
			break;
		}
	}

	v0 = a0; pc0 = ra;
}

void psxBios_strcmp(void) { // 0x17
	char *p1 = (char *)Ra0, *p2 = (char *)Ra1;
	s32 n=0;
	if (a0 == 0 && a1 == 0)
	{
		v0 = 0;
		pc0 = ra;
		return;
	}
	else if (a0 == 0 && a1 != 0)
	{
		v0 = -1;
		pc0 = ra;
		return;
	}
	else if (a0 != 0 && a1 == 0)
	{
		v0 = 1;
		pc0 = ra;
		return;
	}
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s: %s (%x), %s (%x)\n", biosA0n[0x17], Ra0, a0, Ra1, a1);
#endif

	while (*p1 == *p2++) {
		n++;
		if (*p1++ == '\0') {
			v1=n-1;
			a0+=n;
			a1+=n;
			v0 = 0;
			pc0 = ra;
			return;
		}
	}

	v0 = (*p1 - *--p2);
	v1 = n;
	a0+=n;
	a1+=n;
	pc0 = ra;
}

void psxBios_strncmp(void) { // 0x18
	char *p1 = (char *)Ra0, *p2 = (char *)Ra1;
	s32 n = a2;
	if (a0 == 0 && a1 == 0)
	{
		v0 = 0;
		pc0 = ra;
		return;
	}
	else if (a0 == 0 && a1 != 0)
	{
		v0 = -1;
		pc0 = ra;
		return;
	}
	else if (a0 != 0 && a1 == 0)
	{
		v0 = 1;
		pc0 = ra;
		return;
	}
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s: %s (%x), %s (%x), %d\n", biosA0n[0x18], Ra0, a0, Ra1, a1, a2);
#endif

	while (--n >= 0 && *p1 == *p2++) {
		if (*p1++ == '\0') {
			v0 = 0;
			pc0 = ra;
			v1 = a2 - ((a2-n) - 1);
			a0 += (a2-n) - 1;
			a1 += (a2-n) - 1;
			a2 = n;
			return;
		}
	}

	v0 = (n < 0 ? 0 : *p1 - *--p2);
	pc0 = ra;
	v1 = a2 - ((a2-n) - 1);
	a0 += (a2-n) - 1;
	a1 += (a2-n) - 1;
	a2 = n;
}

void psxBios_strcpy(void) { // 0x19
	char *p1 = (char *)Ra0, *p2 = (char *)Ra1;
	if (a0 == 0 || a1 == 0)
	{
		v0 = 0;
		pc0 = ra;
		return;
	}
	while ((*p1++ = *p2++) != '\0');

	v0 = a0; pc0 = ra;
}

void psxBios_strncpy(void) { // 0x1a
	char *p1 = (char *)Ra0, *p2 = (char *)Ra1;
	s32 n = a2, i;
	if (a0 == 0 || a1 == 0)
	{
		v0 = 0;
		pc0 = ra;
		return;
	}
	for (i = 0; i < n; i++) {
		if ((*p1++ = *p2++) == '\0') {
			while (++i < n) {
				*p1++ = '\0';
			}
			v0 = a0; pc0 = ra;
			return;
		}
	}

	v0 = a0; pc0 = ra;
}

void psxBios_strlen(void) { // 0x1b
	char *p = (char *)Ra0;
	v0 = 0;
	if (a0 == 0)
	{
		pc0 = ra;
		return;
	}
	while (*p++) v0++;
	pc0 = ra;
}

void psxBios_index(void) { // 0x1c
	char *p = (char *)Ra0;
	if (a0 == 0)
	{
		v0 = 0;
		pc0 = ra;
		return;
	}
	do {
		if (*p == a1) {
			v0 = a0 + (p - (char *)Ra0);
			pc0 = ra;
			return;
		}
	} while (*p++ != '\0');

	v0 = 0; pc0 = ra;
}

void psxBios_rindex(void) { // 0x1d
	char *p = (char *)Ra0;
	v0 = 0;
	if (a0 == 0)
	{
		pc0 = ra;
		return;
	}
	do {
		if (*p == a1)
			v0 = a0 + (p - (char *)Ra0);
	} while (*p++ != '\0');

	pc0 = ra;
}

void psxBios_strchr(void) { // 0x1e
	psxBios_index();
}

void psxBios_strrchr(void) { // 0x1f
	psxBios_rindex();
}

void psxBios_strpbrk(void) { // 0x20
	char *p1 = (char *)Ra0, *p2 = (char *)Ra1, *scanp, c, sc;

	while ((c = *p1++) != '\0') {
		for (scanp = p2; (sc = *scanp++) != '\0';) {
			if (sc == c) {
				v0 = a0 + (p1 - 1 - (char *)Ra0);
				pc0 = ra;
				return;
			}
		}
	}
	
	/* 
	If there was no occurence, it returns 0 only if src[0]=00h, 
	otherwise returns the incoming "src" value.
	(which is the SAME return value as when a occurence did occur on 1st character).
	*/
	if (p1[0] == 0x00)
	{
		v0 = 0;
	}
	else
	{
		v0 = a0;
	}
	pc0 = ra;
}

void psxBios_strspn(void) { // 0x21
	char *p1, *p2;

	for (p1 = (char *)Ra0; *p1 != '\0'; p1++) {
		for (p2 = (char *)Ra1; *p2 != '\0' && *p2 != *p1; p2++);
		if (*p2 == '\0') break;
	}

	v0 = p1 - (char *)Ra0; pc0 = ra;
}

void psxBios_strcspn(void) { // 0x22
	char *p1, *p2;

	for (p1 = (char *)Ra0; *p1 != '\0'; p1++) {
		for (p2 = (char *)Ra1; *p2 != '\0' && *p2 != *p1; p2++);
		if (*p2 != '\0') break;
	}

	v0 = p1 - (char *)Ra0; pc0 = ra;
}

void psxBios_strtok(void) { // 0x23
	char *pcA0 = (char *)Ra0;
	char *pcRet = strtok(pcA0, (char *)Ra1);
	if(pcRet)
		v0 = a0 + pcRet - pcA0;
	else
		v0 = 0;
    pc0 = ra;
}

void psxBios_strstr(void) { // 0x24
	char *p = (char *)Ra0, *p1, *p2;

	while (*p != '\0') {
		p1 = p;
		p2 = (char *)Ra1;

		while (*p1 != '\0' && *p2 != '\0' && *p1 == *p2) {
			p1++; p2++;
		}

		if (*p2 == '\0') {
			v0 = a0 + (p - (char *)Ra0);
			pc0 = ra;
			return;
		}

		p++;
	}

	v0 = 0; pc0 = ra;
}

void psxBios_toupper(void) { // 0x25
	v0 = (s8)(a0 & 0xff);
	if (v0 >= 'a' && v0 <= 'z') v0 -= 'a' - 'A';
	pc0 = ra;
}

void psxBios_tolower(void) { // 0x26
	v0 = (s8)(a0 & 0xff);
	if (v0 >= 'A' && v0 <= 'Z') v0 += 'a' - 'A';
	pc0 = ra;
}

void psxBios_bcopy(void) { // 0x27
	char *p1 = (char *)Ra1, *p2 = (char *)Ra0;
	v0 = a0;
	if (a0 == 0 || a2 > 0x7FFFFFFF)
	{
		pc0 = ra;
		return;
	}
	while ((s32)a2-- > 0) *p1++ = *p2++;
	a2 = 0;
	pc0 = ra;
}

void psxBios_bzero(void) { // 0x28
	char *p = (char *)Ra0;
	v0 = a0;
	/* Same as memset here (See memset below) */
	if (a1 > 0x7FFFFFFF || a1 == 0)
	{
		v0 = 0;
		pc0 = ra;
		return;
	}
	else if (a0 == 0)
	{
		pc0 = ra;
		return;
	}
	while ((s32)a1-- > 0) *p++ = '\0';
	a1 = 0;
	pc0 = ra;
}

void psxBios_bcmp() { // 0x29
	char *p1 = (char *)Ra0, *p2 = (char *)Ra1;

	if (a0 == 0 || a1 == 0) { v0 = 0; pc0 = ra; return; }

	while ((s32)a2-- > 0) {
		if (*p1++ != *p2++) {
			v0 = *p1 - *p2; // BUG: compare the NEXT byte
			pc0 = ra;
			return;
		}
	}

	v0 = 0; pc0 = ra;
}

void psxBios_memcpy() { // 0x2a
	char *p1 = (char *)Ra0, *p2 = (char *)Ra1;
	s32 n=0;
	v0 = a0;
	if (a0 == 0 || a2 > 0x7FFFFFFF)
	{
		pc0 = ra;
		return;
	}
	while ((s32)a2-- > 0) {
		n++;
		*p1++ = *p2++;
	}
	a2 = 0;
	pc0 = ra;
}

void psxBios_memset() { // 0x2b
	char *p = (char *)Ra0;
	v0 = a0;
	if (a2 > 0x7FFFFFFF || a2 == 0)
	{
		v0 = 0;
		pc0 = ra;
		return;
	}
	if (a0 == 0)
	{
		pc0 = ra;
		return;
	}
	while ((s32)a2-- > 0) *p++ = (char)a1;
	a2 = 0;
	v0 = a0; pc0 = ra;
}

void psxBios_memmove() { // 0x2c
	char *p1 = (char *)Ra0, *p2 = (char *)Ra1;
	v0 = a0;
	if (a0 == 0 || a2 > 0x7FFFFFFF)
	{
		pc0 = ra;
		return;
	}
	if (p2 <= p1 && p2 + a2 > p1) {
		a2++; // BUG: copy one more byte here
		p1 += a2;
		p2 += a2;
		while ((s32)a2-- > 0) *--p1 = *--p2;
	} else {
		while ((s32)a2-- > 0) *p1++ = *p2++;
	}
	pc0 = ra;
}

void psxBios_memcmp() { // 0x2d
	psxBios_bcmp();
}

void psxBios_memchr() { // 0x2e
	char *p = (char *)Ra0;

	while ((s32)a2-- > 0) {
		if (*p++ != (s8)a1) continue;
		v0 = a0 + (p - (char *)Ra0 - 1);
		pc0 = ra;
		return;
	}

	v0 = 0; pc0 = ra;
}

void psxBios_rand(void) { // 2f
	u32 s = psxMu32(0x9010) * 1103515245 + 12345;
	v0 = (s >> 16) & 0x7fff;
	psxMu32ref(0x9010) = SWAPu32(s);
	pc0 = ra;
}

void psxBios_srand(void) { // 30
	psxMu32ref(0x9010) = SWAPu32(a0);
	pc0 = ra;
}

static u32 qscmpfunc, qswidth;

static inline int qscmp(char *a, char *b) {
	u32 sa0 = a0;

	a0 = sa0 + (a - (char *)PSXM(sa0));
	a1 = sa0 + (b - (char *)PSXM(sa0));

	softCall2(qscmpfunc);

	a0 = sa0;
	return (s32)v0;
}

static inline void qexchange(char *i, char *j) {
	char t;
	int n = qswidth;

	do {
		t = *i;
		*i++ = *j;
		*j++ = t;
	} while (--n);
}

static inline void q3exchange(char *i, char *j, char *k) {
	char t;
	int n = qswidth;

	do {
		t = *i;
		*i++ = *k;
		*k++ = *j;
		*j++ = t;
	} while (--n);
}

static void qsort_main(char *a, char *l) {
	char *i, *j, *lp, *hp;
	int c;
	unsigned int n;

start:
	if ((n = l - a) <= qswidth)
		return;
	n = qswidth * (n / (2 * qswidth));
	hp = lp = a + n;
	i = a;
	j = l - qswidth;
	while (TRUE) {
		if (i < lp) {
			if ((c = qscmp(i, lp)) == 0) {
				qexchange(i, lp -= qswidth);
				continue;
			}
			if (c < 0) {
				i += qswidth;
				continue;
			}
		}

loop:
		if (j > hp) {
			if ((c = qscmp(hp, j)) == 0) {
				qexchange(hp += qswidth, j);
				goto loop;
			}
			if (c > 0) {
				if (i == lp) {
					q3exchange(i, hp += qswidth, j);
					i = lp += qswidth;
					goto loop;
				}
				qexchange(i, j);
				j -= qswidth;
				i += qswidth;
				continue;
			}
			j -= qswidth;
			goto loop;
		}

		if (i == lp) {
			if (lp - a >= l - hp) {
				qsort_main(hp + qswidth, l);
				l = lp;
			} else {
				qsort_main(a, lp);
				a = hp + qswidth;
			}
			goto start;
		}

		q3exchange(j, lp -= qswidth, i);
		j = hp -= qswidth;
	}
}

void psxBios_qsort() { // 0x31
	qswidth = a2;
	qscmpfunc = a3;
	qsort_main((char *)Ra0, (char *)Ra0 + a1 * a2);

	pc0 = ra;
}

void psxBios_malloc(void) { // 33
	unsigned int *chunk, *newchunk = NULL;
	unsigned int dsize = 0, csize, cstat;
	int colflag;
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosA0n[0x33]);
#endif
	if (!a0 || (!heap_size || !heap_addr)) {
		v0 = 0;
		pc0 = ra;
		return;
	}

	// scan through heap and combine free chunks of space
	chunk = heap_addr;
	colflag = 0;
	while(chunk < heap_end) {
		// get size and status of actual chunk
		csize = ((u32)*chunk) & 0xfffffffc;
		cstat = ((u32)*chunk) & 1;

		// most probably broken heap descriptor
		// this fixes Burning Road
		if (*chunk == 0) {
			newchunk = chunk;
			dsize = ((uptr)heap_end - (uptr)chunk) - 4;
			colflag = 1;
			break;
		}

		// it's a free chunk
		if(cstat == 1) {
			if(colflag == 0) {
				newchunk = chunk;
				dsize = csize;
				colflag = 1;			// let's begin a new collection of free memory
			}
			else dsize += (csize+4);	// add the new size including header
		}
		// not a free chunk: did we start a collection ?
		else {
			if(colflag == 1) {			// collection is over
				colflag = 0;
				*newchunk = SWAP32(dsize | 1);
			}
		}

		// next chunk
		chunk = (u32*)((uptr)chunk + csize + 4);
	}
	// if neccessary free memory on end of heap
	if (colflag == 1)
		*newchunk = SWAP32(dsize | 1);

	chunk = heap_addr;
	csize = ((u32)*chunk) & 0xfffffffc;
	cstat = ((u32)*chunk) & 1;
	dsize = (a0 + 3) & 0xfffffffc;

	// exit on uninitialized heap
	if (chunk == NULL) {
		printf("malloc %x,%x: Uninitialized Heap!\n", v0, a0);
		v0 = 0;
		pc0 = ra;
		return;
	}

	// search an unused chunk that is big enough until the end of the heap
	while ((dsize > csize || cstat==0) && chunk < heap_end ) {
		chunk = (u32*)((uptr)chunk + csize + 4);

			// catch out of memory
			if(chunk >= heap_end) {
				printf("malloc %x,%x: Out of memory error!\n",
					v0, a0);
				v0 = 0; pc0 = ra;
				return;
			}

		csize = ((u32)*chunk) & 0xfffffffc;
		cstat = ((u32)*chunk) & 1;
	}

	// allocate memory
	if(dsize == csize) {
		// chunk has same size
		*chunk &= 0xfffffffc;
	} else if (dsize > csize) {
		v0 = 0; pc0 = ra;
		return;
	} else {
		// split free chunk
		*chunk = SWAP32(dsize);
		newchunk = (u32*)((uptr)chunk + dsize + 4);
		*newchunk = SWAP32(((csize - dsize - 4) & 0xfffffffc) | 1);
	}

	// return pointer to allocated memory
	v0 = ((uptr)chunk - (uptr)psxM) + 4;
	v0|= 0x80000000;
	//printf ("malloc %x,%x\n", v0, a0);
	pc0 = ra;
}

void psxBios_free(void) { // 34

#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosA0n[0x34]);
#endif

	//printf("free %x: %x bytes\n", a0, *(u32*)(Ra0-4));

	if (a0)
		*(u32*)(Ra0-4) |= 1;	// set chunk to free
	pc0 = ra;
}

void psxBios_calloc(void) { // 37
	void *pv0;
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosA0n[0x37]);
#endif

	a0 = a0 * a1;
	psxBios_malloc();
	pv0 = Rv0;
	if (pv0)
		memset(pv0, 0, a0);
}

void psxBios_realloc(void) { // 38
	u32 block = a0;
	u32 size = a1;
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosA0n[0x38]);
#endif

	a0 = block;
	/* If "old_buf" is zero, executes malloc(new_size), and returns r2=new_buf (or 0=failed). */
	if (block == 0)
	{
		psxBios_malloc();
	}
	/* Else, if "new_size" is zero, executes free(old_buf), and returns r2=garbage. */
	else if (size == 0)
	{
		psxBios_free();
	}
	/* Else, executes malloc(new_size), bcopy(old_buf,new_buf,new_size), and free(old_buf), and returns r2=new_buf (or 0=failed). */
	/* Note that it is not quite implemented this way here. */
	else
	{
		psxBios_free();
		a0 = size;
		psxBios_malloc();
	}
}


/* InitHeap(void *block , int n) */
void psxBios_InitHeap(void) { // 39
	unsigned int size;

#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosA0n[0x39]);
#endif

	if (((a0 & 0x1fffff) + a1)>= 0x200000) size = 0x1ffffc - (a0 & 0x1fffff);
	else size = a1;

	size &= 0xfffffffc;

	heap_addr = (u32 *)Ra0;
	heap_size = size;
	heap_end = (u32 *)((u8 *)heap_addr + heap_size);
	/* HACKFIX: Commenting out this line fixes GTA2 crash */
	//*heap_addr = SWAP32(size | 1);

	//printf("InitHeap %x,%x : %x %x\n",a0,a1, (uptr)heap_addr-(uptr)psxM, size);

	pc0 = ra;
}

void psxBios_getchar() { //0x3b
	v0 = getchar(); pc0 = ra;
}

void psxBios_printf(void) { // 3f
	char tmp[1024];
	char tmp2[1024];
	u32 save[4];
	char *ptmp = tmp;
	int n=1, i=0, j, k=0;
	void *psp;

	psp = PSXM(sp);
	if (psp) {
		memcpy(save, psp, 4 * 4);
		psxMu32ref(sp) = SWAP32((u32)a0);
		psxMu32ref(sp + 4) = SWAP32((u32)a1);
		psxMu32ref(sp + 8) = SWAP32((u32)a2);
		psxMu32ref(sp + 12) = SWAP32((u32)a3);
	}

	while (Ra0[i]) {
		switch (Ra0[i]) {
			case '%':
				j = 0;
				tmp2[j++] = '%';
				k++;
_start:
				switch (Ra0[++i]) {
					case '.':
					case 'l':
						tmp2[j++] = Ra0[i]; goto _start;
					default:
						if (Ra0[i] >= '0' && Ra0[i] <= '9') {
							tmp2[j++] = Ra0[i];
							goto _start;
						}
						break;
				}
				tmp2[j++] = Ra0[i];
				tmp2[j] = 0;

				switch (Ra0[i]) {
					case 'f': case 'F':
						ptmp+= sprintf(ptmp, tmp2, (float)psxMu32(sp + n * 4)); n++; break;
					case 'a': case 'A':
					case 'e': case 'E':
					case 'g': case 'G':
						ptmp+= sprintf(ptmp, tmp2, (double)psxMu32(sp + n * 4)); n++; break;
					case 'p':
					case 'i': case 'u':
					case 'd': case 'D':
					case 'o': case 'O':
					case 'x': case 'X':
						ptmp+= sprintf(ptmp, tmp2, (unsigned int)psxMu32(sp + n * 4)); n++; break;
					case 'c':
						ptmp+= sprintf(ptmp, tmp2, (unsigned char)psxMu32(sp + n * 4)); n++; break;
					case 's':
						ptmp+= sprintf(ptmp, tmp2, (char*)PSXM(psxMu32(sp + n * 4))); n++; break;
					case '%':
						*ptmp++ = Ra0[i]; break;
				}
				i++;
				break;
			default:
				*ptmp++ = Ra0[i++];
		}
	}
	*ptmp = 0;

	if (psp)
		memcpy(psp, save, 4 * 4);

	//printf(tmp);
	//printf("PRINTF I=%i, J=%i, N=%i, K=%i\n",i,j,n,k);

	pc0 = ra;
	psxRegs.GPR.n.hi = 0x00000008;
}

void psxBios_format() { // 0x41
	if (strcmp(Ra0, "bu00:") == 0) {
		sioMcdFormat(MCD1);
		v0 = 1;
	} else if (strcmp(Ra0, "bu10:") == 0) {
		sioMcdFormat(MCD2);
		v0 = 1;
	} else {
		v0 = 0;
	}
	pc0 = ra;
}

/*
 *	long Load(char *name, struct EXEC *header);
 */

void psxBios_Load(void) { // 42
	EXE_HEADER eheader;
	void *pa1;

#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s: %s, %x\n", biosA0n[0x42], Ra0, a1);
#endif

	pa1 = Ra1;
	if (pa1 && LoadCdromFile(Ra0, &eheader) == 0) {
		memcpy(pa1, ((char*)&eheader)+16, sizeof(EXEC));
		v0 = 1;
	} else v0 = 0;

	pc0 = ra;
}

/*
 *	int Exec(struct EXEC *header , int argc , char **argv);
 */

void psxBios_Exec(void) { // 43
	EXEC *header = (EXEC*)Ra0;
	u32 tmp;

#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s: %x, %x, %x\n", biosA0n[0x43], a0, a1, a2);
#endif

	header->_sp = sp;
	header->_fp = fp;
	header->_sp = sp;
	header->_gp = gp;
	header->ret = ra;
	header->base = s0;

	if (header->S_addr != 0) {
		tmp = header->S_addr + header->s_size;
		sp = tmp;
		fp = sp;
	}

	gp = header->gp0;

	s0 = a0;

	a0 = a1;
	a1 = a2;

	ra = 0x8000;
	pc0 = header->_pc0;
}

void psxBios_FlushCache(void) { // 44
#ifdef PSXBIOS_LOG
    PSXBIOS_LOG("psxBios_%s\n", biosA0n[0x44]);
#endif
    psxCpu->Notify(R3000ACPU_NOTIFY_CACHE_ISOLATED, NULL);
    psxCpu->Notify(R3000ACPU_NOTIFY_CACHE_UNISOLATED, NULL);
 
    pc0 = ra;
}

void psxBios_GPU_dw(void) { // 0x46
	int size;
	s32 *ptr;

#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosA0n[0x46]);
#endif

	GPU_writeData(0xa0000000);
	GPU_writeData((a1<<16)|(a0&0xffff));
	GPU_writeData((a3<<16)|(a2&0xffff));
	size = (a2*a3+1)/2;
	ptr = (s32*)PSXM(Rsp[4]);  //that is correct?
#ifndef __arm__
	do {
		GPU_writeData(SWAP32(*ptr));
		ptr++;
	} while(--size);
#else
	GPU_writeDataMem((u32*)ptr,size);
#endif
	pc0 = ra;
}  

void psxBios_mem2vram(void) { // 0x47
	int size;

	GPU_writeData(0xa0000000);
	GPU_writeData((a1<<16)|(a0&0xffff));
	GPU_writeData((a3<<16)|(a2&0xffff));
	size = (a2*a3+1)/2;
	GPU_writeStatus(0x04000002);
	psxHwWrite32(0x1f8010f4,0);
	psxHwWrite32(0x1f8010f0,psxHwRead32(0x1f8010f0)|0x800);
	psxHwWrite32(0x1f8010a0,Rsp[4]);//might have a buggy...
	psxHwWrite32(0x1f8010a4,((size/16)<<16)|16);
	psxHwWrite32(0x1f8010a8,0x01000201);

	pc0 = ra;
}

void psxBios_SendGPU(void) { // 0x48
	GPU_writeStatus(a0);
	gpuSyncPluginSR();
	pc0 = ra;
}

void psxBios_GPU_cw(void) { // 0x49
	GPU_writeData(a0);
	pc0 = ra;
}

void psxBios_GPU_cwb(void) { // 0x4a
	s32 *ptr = (s32*)Ra0;
	int size = a1;
#ifndef __arm__
	while(size--) {
		GPU_writeData(SWAP32(*ptr));
		ptr++;
	}
#else
	GPU_writeDataMem((u32*)ptr,size);
#endif
	pc0 = ra;
}
   
void psxBios_GPU_SendPackets(void) { //4b:	
	GPU_writeStatus(0x04000002);
	psxHwWrite32(0x1f8010f4,0);
	psxHwWrite32(0x1f8010f0,psxHwRead32(0x1f8010f0)|0x800);
	psxHwWrite32(0x1f8010a0,a0);
	psxHwWrite32(0x1f8010a4,0);
	psxHwWrite32(0x1f8010a8,0x010000401);
	pc0 = ra;
}

void psxBios_sys_a0_4c(void) { // 0x4c GPU relate
	psxHwWrite32(0x1f8010a8,0x00000401);
	GPU_writeData(0x0400000);
	GPU_writeData(0x0200000);
	GPU_writeData(0x0100000);
	v0 = 0x1f801814;
	pc0 = ra;
}

void psxBios_GPU_GetGPUStatus(void) { // 0x4d
	v0 = GPU_readStatus();
	pc0 = ra;
}

#undef s_addr

void psxBios_LoadExec(void) { // 51
	EXEC *header = (EXEC*)PSXM(0xf000);
	u32 s_addr, s_size;

#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s: %s: %x,%x\n", biosA0n[0x51], Ra0, a1, a2);
#endif
	s_addr = a1; s_size = a2;

	a1 = 0xf000;	
	psxBios_Load();

	header->S_addr = s_addr;
	header->s_size = s_size;

	a0 = 0xf000; a1 = 0; a2 = 0;
	psxBios_Exec();
}

//senquack - updated to match PCSX Rearmed:
void psxBios__bu_init(void) { // 70
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosA0n[0x70]);
#endif

	DeliverEvent(0x11, 0x2); // 0xf0000011, 0x0004
	DeliverEvent(0x81, 0x2); // 0xf4000001, 0x0004

	pc0 = ra;
}

void psxBios__96_init(void) { // 71
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosA0n[0x71]);
#endif

	pc0 = ra;
}

void psxBios__96_remove(void) { // 72
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosA0n[0x72]);
#endif

	pc0 = ra;
	ResetIoCycle();
}

void psxBios_SetMem(void) { // 9f
	u32 _new = psxHu32(0x1060);

#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s: %x, %x\n", biosA0n[0x9f], a0, a1);
#endif

	switch(a0) {
		case 2:
			psxHu32ref(0x1060) = SWAP32(_new);
			psxMu32ref(0x060) = a0;
			//printf("Change effective memory : %d MBytes\n",a0);
			break;

		case 8:
			psxHu32ref(0x1060) = SWAP32(_new | 0x300);
			psxMu32ref(0x060) = a0;
			//printf("Change effective memory : %d MBytes\n",a0);
	
		default:
			//printf("Effective memory must be 2/8 MBytes\n");
		break;
	}

	pc0 = ra;
}

void psxBios__card_info(void) { // ab
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s: %x\n", biosA0n[0xab], a0);
#endif
	u32 ret;
	card_active_chan = a0;

	switch (card_active_chan) 
	{
	case 0x00: case 0x01: case 0x02: case 0x03:
		ret = Config.Mcd1[0] ? 0x2 : 0x8;
		break;
	case 0x10: case 0x11: case 0x12: case 0x13:
		ret = Config.Mcd2[0] ? 0x2 : 0x8;
		break;
	default:
#ifdef PSXBIOS_LOG
		PSXBIOS_LOG("psxBios_%s: UNKNOWN PORT 0x%x\n", biosA0n[0xab], card_active_chan);
#endif
		ret = 0x11;
		break;
	}

	// COTS password option
	/*if (Config.NoMemcard)
		ret = 0x8;*/

//	DeliverEvent(0x11, 0x2); // 0xf0000011, 0x0004
	DeliverEvent(0x81, ret); // 0xf4000001, 0x0004

	v0 = 1;
	pc0 = ra;
}

void psxBios__card_load(void) { // ac
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s: %x\n", biosA0n[0xac], a0);
#endif

	card_active_chan = a0;

//	DeliverEvent(0x11, 0x2); // 0xf0000011, 0x0004
	DeliverEvent(0x81, 0x2); // 0xf4000001, 0x0004

	v0 = 1;
	pc0 = ra;
}

/* System calls B0 */

void psxBios_SetRCnt(void) { // 02
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosB0n[0x02]);
#endif

	a0&= 0x3;
	if (a0 != 3) {
		u32 mode=0;

		psxRcntWtarget(a0, a1);
		if (a2&0x1000) mode|= 0x050; // Interrupt Mode
		if (a2&0x0100) mode|= 0x008; // Count to 0xffff
		if (a2&0x0010) mode|= 0x001; // Timer stop mode
		if (a0 == 2) { if (a2&0x0001) mode|= 0x200; } // System Clock mode
		else         { if (a2&0x0001) mode|= 0x100; } // System Clock mode

		psxRcntWmode(a0, mode);
	}
	pc0 = ra;
}

void psxBios_GetRCnt(void) { // 03
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosB0n[0x03]);
#endif

	a0&= 0x3;
	if (a0 != 3) v0 = psxRcntRcount(a0);
	else v0 = 0;
	pc0 = ra;
}

void psxBios_StartRCnt(void) { // 04
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosB0n[0x04]);
#endif

	a0&= 0x3;
	ResetIoCycle();
	if (a0 != 3) psxHu32ref(0x1074)|= SWAP32((u32)((1<<(a0+4))));
	else psxHu32ref(0x1074)|= SWAPu32(0x1);
	v0 = 1; pc0 = ra;
}

void psxBios_StopRCnt(void) { // 05
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosB0n[0x05]);
#endif

	a0&= 0x3;
	ResetIoCycle();
	if (a0 != 3) psxHu32ref(0x1074)&= SWAP32((u32)(~(1<<(a0+4))));
	else psxHu32ref(0x1074)&= SWAPu32(~0x1);
	pc0 = ra;
}

void psxBios_ResetRCnt(void) { // 06
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosB0n[0x06]);
#endif

	a0&= 0x3;
	if (a0 != 3) {
		psxRcntWmode(a0, 0);
		psxRcntWtarget(a0, 0);
		psxRcntWcount(a0, 0);
	}
	pc0 = ra;
}


/* gets ev for use with Event */
#define GetEv() \
	ev = (a0 >> 24) & 0xf; \
	if (ev == 0xf) ev = 0x5; \
	ev*= 32; \
	ev+= a0&0x1f;

/* gets spec for use with Event */
#define GetSpec() \
	spec = 0; \
	switch (a1) { \
		case 0x0301: spec = 16; break; \
		case 0x0302: spec = 17; break; \
		default: \
			for (i=0; i<16; i++) if (a1 & (1 << i)) { spec = i; break; } \
			break; \
	}

void psxBios_DeliverEvent(void) { // 07
	int ev, spec;
	int i;

	GetEv();
	GetSpec();

#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s %x,%x\n", biosB0n[0x07], ev, spec);
#endif

	DeliverEvent(ev, spec);

	pc0 = ra;
}

void psxBios_OpenEvent(void) { // 08
	int ev, spec;
	int i;

	GetEv();
	GetSpec();

#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s %x,%x (class:%x, spec:%x, mode:%x, func:%x)\n", biosB0n[0x08], ev, spec, a0, a1, a2, a3);
#endif

	Event[ev][spec].status = EvStWAIT;
	Event[ev][spec].mode = a2;
	if (a2 == EvMdINTR) Event[ev][spec].fhandler = a3;

	v0 = ev | (spec << 8);
	pc0 = ra;
}

void psxBios_CloseEvent(void) { // 09
	int ev, spec;

	ev   = a0 & 0xff;
	spec = (a0 >> 8) & 0xff;

#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s %x,%x\n", biosB0n[0x09], ev, spec);
#endif

	Event[ev][spec].status = EvStUNUSED;

	v0 = 1;
	pc0 = ra;
}

void psxBios_WaitEvent(void) { // 0a
	int ev, spec;

	ev   = a0 & 0xff;
	spec = (a0 >> 8) & 0xff;
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s %x,%x\n", biosB0n[0x0a], ev, spec);
#endif
	if (Event[ev][spec].status == EvStUNUSED)
	{
		v0 = 0;
		pc0 = ra;	
		return;
	}

	if (Event[ev][spec].status == EvStALREADY) 
	{
		/* Callback events (mode=EvMdINTR) do never set the ready flag (and thus WaitEvent would hang forever). */
		if (!(Event[ev][spec].mode == EvMdINTR)) Event[ev][spec].status = EvStACTIVE;
		v0 = 1;
		pc0 = ra;
		return;
	}

	v0 = 0;
	pc0 = ra;
	ResetIoCycle();
}

void psxBios_TestEvent(void) { // 0b
	int ev, spec;

	ev   = a0 & 0xff;
	spec = (a0 >> 8) & 0xff;

	if (Event[ev][spec].status == EvStALREADY) 
	{
		if (!(Event[ev][spec].mode == EvMdINTR)) Event[ev][spec].status = EvStACTIVE;
		v0 = 1;
	} 
	else 
	{
		v0 = 0;
	}

#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s %x,%x: %x\n", biosB0n[0x0b], ev, spec, v0);
#endif

	pc0 = ra;
}

void psxBios_EnableEvent(void) { // 0c
	int ev, spec;

	ev   = a0 & 0xff;
	spec = (a0 >> 8) & 0xff;

#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s %x,%x\n", biosB0n[0x0c], ev, spec);
#endif

	Event[ev][spec].status = EvStACTIVE;

	v0 = 1;
	pc0 = ra;
}

void psxBios_DisableEvent(void) { // 0d
	int ev, spec;

	ev   = a0 & 0xff;
	spec = (a0 >> 8) & 0xff;

#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s %x,%x\n", biosB0n[0x0d], ev, spec);
#endif

	Event[ev][spec].status = EvStWAIT;

	v0 = 1;
	pc0 = ra;
}

/*
 *	long OpenTh(long (*func)(), unsigned long sp, unsigned long gp);
 */

void psxBios_OpenTh(void) { // 0e
	int th;

	for (th=1; th<8; th++)
	{
		if (Thread[th].status == 0) break;
	}
	if (th == 8) {
		// Feb 2019 - Added out-of-bounds fix caught by cppcheck:
		// When no free TCB is found, return 0xffffffff according to Nocash doc.
#ifdef PSXBIOS_LOG
		PSXBIOS_LOG("\t%s() WARNING! No Free TCBs found!\n", __func__);
#endif
		v0 = 0xffffffff;
		pc0 = ra;
		return;
	}
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s: %x\n", biosB0n[0x0e], th);
#endif

	Thread[th].status = 1;
	Thread[th].func    = a0;
	Thread[th].reg[29] = a1;
	Thread[th].reg[28] = a2;

	v0 = th; pc0 = ra;
}

/*
 *	int CloseTh(long thread);
 */

void psxBios_CloseTh(void) { // 0f
	int th = a0 & 0xff;

#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s: %x\n", biosB0n[0x0f], th);
#endif

	if (Thread[th].status == 0) {
		v0 = 0;
	} else {
		Thread[th].status = 0;
		v0 = 1;
	}

	pc0 = ra;
}

/*
 *	int ChangeTh(long thread);
 */

void psxBios_ChangeTh(void) { // 10
	int th = a0 & 0xff;

#ifdef PSXBIOS_LOG
//	PSXBIOS_LOG("psxBios_%s: %x\n", biosB0n[0x10], th);
#endif

	if (Thread[th].status == 0 || CurThread == th) {
		v0 = 0;

		pc0 = ra;
	} else {
		v0 = 1;

		if (Thread[CurThread].status == 2) {
			Thread[CurThread].status = 1;
			Thread[CurThread].func = ra;
			memcpy(Thread[CurThread].reg, psxRegs.GPR.r, 32*4);
		}

		memcpy(psxRegs.GPR.r, Thread[th].reg, 32*4);
		pc0 = Thread[th].func;
		Thread[th].status = 2;
		CurThread = th;
	}
}

void psxBios_InitPAD(void) { // 0x12
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosB0n[0x12]);
#endif

	pad_buf1 = (char*)Ra0;
	pad_buf1len = a1;
	pad_buf2 = (char*)Ra2;
	pad_buf2len = a3;

	v0 = 1; pc0 = ra;
}

void psxBios_StartPAD(void) { // 13
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosB0n[0x13]);
#endif

	pad_stopped = 0;
	ResetIoCycle();
	psxHwWrite16(0x1f801074, (unsigned short)(psxHwRead16(0x1f801074) | 0x1));
	psxRegs.CP0.n.Status |= 0x401;
	pc0 = ra;
}

void psxBios_StopPAD(void) { // 14
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosB0n[0x14]);
#endif

	pad_stopped = 1;
	if (pad_buf == 0){
	pad_buf1 = NULL;
	pad_buf2 = NULL;
	}
	pc0 = ra;
}

void psxBios_PAD_init(void) { // 15
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosB0n[0x15]);
#endif
	if (!(a0 == 0x20000000 || a0 == 0x20000001))
	{
		v0 = 0;
		pc0 = ra;
		return;
	}
	ResetIoCycle();
	psxHwWrite16(0x1f801074, (u16)(psxHwRead16(0x1f801074) | 0x1));
	pad_buf = (int*)Ra1;
	*pad_buf = -1;
	psxRegs.CP0.n.Status |= 0x401;
	v0 = 2;
	pc0 = ra;
}

void psxBios_PAD_dr(void) { // 16
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosB0n[0x16]);
#endif

	v0 = -1;
	pc0 = ra;
}

void psxBios_ReturnFromException(void) { // 17
	LoadRegs();

	pc0 = psxRegs.CP0.n.EPC;
	k0 = interrupt_r26;
	//ra = 0x8004E6EC;
	if (psxRegs.CP0.n.Cause & 0x80000000) pc0+=4;

	psxRegs.CP0.n.Status = (psxRegs.CP0.n.Status & 0xfffffff0) |
						  ((psxRegs.CP0.n.Status & 0x3c) >> 2);
	ResetIoCycle();
}

void psxBios_ResetEntryInt(void) { // 18
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosB0n[0x18]);
#endif

	jmp_int = NULL;
	pc0 = ra;
}

void psxBios_HookEntryInt(void) { // 19
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosB0n[0x19]);
#endif

	jmp_int = (u32*)Ra0;
	pc0 = ra;
}

void psxBios_UnDeliverEvent(void) { // 0x20
	int ev, spec;
	int i;

	GetEv();
	GetSpec();

#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s %x,%x\n", biosB0n[0x20], ev, spec);
#endif

	if (Event[ev][spec].status == EvStALREADY &&
		Event[ev][spec].mode == EvMdNOINTR)
		Event[ev][spec].status = EvStACTIVE;

	pc0 = ra;
}

/*
 *	int open(char *name , int mode);
 */

void psxBios_open(void) { // 0x32
	const char *pa0 = Ra0;

#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s: %s,%x\n", biosB0n[0x32], Ra0, a1);
#endif

	v0 = -1;

	if (pa0) {
		if (!strncmp(pa0, "bu00", 4)) {
			buopen(1, sioMcdDataPtr(MCD1), MCD1);
		}

		if (!strncmp(pa0, "bu10", 4)) {
			buopen(2, sioMcdDataPtr(MCD2), MCD2);
		}
	}

	pc0 = ra;
}

/*
 *	int lseek(int fd , int offset , int whence);
 */

void psxBios_lseek(void) { // 0x33
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s: %x, %x, %x\n", biosB0n[0x33], a0, a1, a2);
#endif

	switch (a2) {
		case 0: // SEEK_SET
			FDesc[a0].offset = a1;
			v0 = a1;
//			DeliverEvent(0x11, 0x2); // 0xf0000011, 0x0004
//			DeliverEvent(0x81, 0x2); // 0xf4000001, 0x0004
			break;

		case 1: // SEEK_CUR
			FDesc[a0].offset+= a1;
			v0 = FDesc[a0].offset;
			break;
	}

	pc0 = ra;
}

/*
 *	int read(int fd , void *buf , int nbytes);
 */

void psxBios_read(void) { // 0x34
	void *pa1 = Ra1;

#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s: %x, %x, %x\n", biosB0n[0x34], a0, a1, a2);
#endif

	v0 = -1;

	if (pa1) {
		switch (a0) {
			case 2: buread(pa1, 1, a2); break;
			case 3: buread(pa1, 2, a2); break;
		}
	}

	pc0 = ra;
}

/*
 *	int write(int fd , void *buf , int nbytes);
 */

void psxBios_write(void) { // 0x35/0x03
	void *pa1 = Ra1;

#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s: %x,%x,%x\n", biosB0n[0x35], a0, a1, a2);
#endif

	v0 = -1;
	if (!pa1) {
		pc0 = ra;
		return;
	}

	if (a0 == 1) { // stdout
		char *ptr = (char *)pa1;

		v0 = a2;
		while (a2 > 0) {
			printf("%c", *ptr++); a2--;
		}
		pc0 = ra; return;
	}

	switch (a0) {
		case 2: buwrite(pa1, 1, a2); break;
		case 3: buwrite(pa1, 2, a2); break;
	}
  		
	pc0 = ra;

}

/*
 *	int close(int fd);
 */

void psxBios_close(void) { // 0x36
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s: %x\n", biosB0n[0x36], a0);
#endif

	v0 = a0;
	pc0 = ra;
}

void psxBios_putchar(void) { // 3d
	//printf("%c", (char)a0);
	pc0 = ra;
}

void psxBios_puts(void) { // 3e/3f
	//printf(Ra0);
	pc0 = ra;
}

/*
 *	struct DIRENTRY* firstfile(char *name,struct DIRENTRY *dir);
 */
 
void psxBios_firstfile(void) { // 42
	struct DIRENTRY *dir = (struct DIRENTRY *)Ra1;
	const char *pa0 = Ra0;
	u32 _dir = a1;

#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s: %s\n", biosB0n[0x42], Ra0);
#endif

	v0 = 0;

	if (pa0) {
		strcpy(ffile, pa0);
		pfile = ffile+5;
		nfile = 0;
		if (!strncmp(pa0, "bu00", 4)) {
			// firstfile() calls _card_read() internally, so deliver it's event
			DeliverEvent(0x11, 0x2);
			bufile(1);
		} else if (!strncmp(pa0, "bu10", 4)) {
			// firstfile() calls _card_read() internally, so deliver it's event
			DeliverEvent(0x11, 0x2);
			bufile(2);
		}
	}

	pc0 = ra;
}

/*
 *	struct DIRENTRY* nextfile(struct DIRENTRY *dir);
 */

void psxBios_nextfile(void) { // 43
	struct DIRENTRY *dir = (struct DIRENTRY *)Ra0;
	u32 _dir = a0;

#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s: %s\n", biosB0n[0x43], dir->name);
#endif

	v0 = 0;

	if (!strncmp(ffile, "bu00", 4)) {
		bufile(1);
	}

	if (!strncmp(ffile, "bu10", 4)) {
		bufile(2);
	}

	pc0 = ra;
}

/*
 *	int rename(char *old, char *new);
 */

void psxBios_rename(void) { // 44
	const char *pa0 = Ra0;
	const char *pa1 = Ra1;

#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s: %s,%s\n", biosB0n[0x44], Ra0, Ra1);
#endif

	v0 = 0;

	if (pa0 && pa1) {
		if (!strncmp(pa0, "bu00", 4) && !strncmp(pa1, "bu00", 4)) {
			burename(1);
		}

		if (!strncmp(pa0, "bu10", 4) && !strncmp(pa1, "bu10", 4)) {
			burename(2);
		}
	}

	pc0 = ra;
}

/*
 *	int delete(char *name);
 */

void psxBios_delete(void) { // 45
	const char *pa0 = Ra0;

#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s: %s\n", biosB0n[0x45], Ra0);
#endif

	v0 = 0;

	if (pa0) {
		if (!strncmp(pa0, "bu00", 4)) {
			budelete(1);
		}

		if (!strncmp(pa0, "bu10", 4)) {
			budelete(2);
		}
	}

	pc0 = ra;
}

void psxBios_InitCARD(void) { // 4a
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s: %x\n", biosB0n[0x4a], a0);
#endif

	CardState = 0;

	pc0 = ra;
}

void psxBios_StartCARD(void) { // 4b
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosB0n[0x4b]);
#endif

	if (CardState == 0) CardState = 1;

	pc0 = ra;
	ResetIoCycle();
}

void psxBios_StopCARD(void) { // 4c
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosB0n[0x4c]);
#endif

	if (CardState == 1) CardState = 0;

	pc0 = ra;
}

void psxBios__card_write(void) { // 0x4e
	const char *pa2 = Ra2;
	int port;

#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s: %x,%x,%x\n", biosB0n[0x4e], a0, a1, a2);
#endif
	/* Function also accepts sector 400h (a bug) */
	if (!(a1 <= 0x400))
	{
		/* Invalid sectors */
		v0 = 0; pc0 = ra;
		return;
	}
	card_active_chan = a0;
	port = a0 >> 4;

	if (pa2) {
		sioMcdWrite(((port == 0) ? MCD1 : MCD2), pa2, a1 * 128, 128);
	}

	DeliverEvent(0x11, 0x2); // 0xf0000011, 0x0004
//	DeliverEvent(0x81, 0x2); // 0xf4000001, 0x0004

	v0 = 1; pc0 = ra;
}

void psxBios__card_read(void) { // 0x4f
	char *pa2 = Ra2;
	int port;

#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosB0n[0x4f]);
#endif
	/* Function also accepts sector 400h (a bug) */
	if (!(a1 <= 0x400))
	{
		/* Invalid sectors */
		v0 = 0; pc0 = ra;
		return;
	}
	card_active_chan = a0;
	port = a0 >> 4;

	if (pa2) {
		sioMcdRead(((port == 0) ? MCD1 : MCD2), pa2, a1 * 128, 128);
	}

	DeliverEvent(0x11, 0x2); // 0xf0000011, 0x0004
//	DeliverEvent(0x81, 0x2); // 0xf4000001, 0x0004

	v0 = 1; pc0 = ra;
}

void psxBios__new_card(void) { // 0x50
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosB0n[0x50]);
#endif

	pc0 = ra;
}

void psxBios_Krom2RawAdd() { // 0x51
	int i = 0;

	const u32 table_8140[][2] = {
		{0x8140, 0x0000}, {0x8180, 0x0762}, {0x81ad, 0x0cc6}, {0x81b8, 0x0ca8},
		{0x81c0, 0x0f00}, {0x81c8, 0x0d98}, {0x81cf, 0x10c2}, {0x81da, 0x0e6a},
		{0x81e9, 0x13ce}, {0x81f0, 0x102c}, {0x81f8, 0x1590}, {0x81fc, 0x111c},
		{0x81fd, 0x1626}, {0x824f, 0x113a}, {0x8259, 0x20ee}, {0x8260, 0x1266},
		{0x827a, 0x24cc}, {0x8281, 0x1572}, {0x829b, 0x28aa}, {0x829f, 0x187e},
		{0x82f2, 0x32dc}, {0x8340, 0x2238}, {0x837f, 0x4362}, {0x8380, 0x299a},
		{0x8397, 0x4632}, {0x839f, 0x2c4c}, {0x83b7, 0x49f2}, {0x83bf, 0x2f1c},
		{0x83d7, 0x4db2}, {0x8440, 0x31ec}, {0x8461, 0x5dde}, {0x8470, 0x35ca},
		{0x847f, 0x6162}, {0x8480, 0x378c}, {0x8492, 0x639c}, {0x849f, 0x39a8},
		{0xffff, 0}
	};

	const u32 table_889f[][2] = {
		{0x889f, 0x3d68},  {0x8900, 0x40ec},  {0x897f, 0x4fb0},  {0x8a00, 0x56f4},
		{0x8a7f, 0x65b8},  {0x8b00, 0x6cfc},  {0x8b7f, 0x7bc0},  {0x8c00, 0x8304},
		{0x8c7f, 0x91c8},  {0x8d00, 0x990c},  {0x8d7f, 0xa7d0},  {0x8e00, 0xaf14},
		{0x8e7f, 0xbdd8},  {0x8f00, 0xc51c},  {0x8f7f, 0xd3e0},  {0x9000, 0xdb24},
		{0x907f, 0xe9e8},  {0x9100, 0xf12c},  {0x917f, 0xfff0},  {0x9200, 0x10734},
		{0x927f, 0x115f8}, {0x9300, 0x11d3c}, {0x937f, 0x12c00}, {0x9400, 0x13344},
		{0x947f, 0x14208}, {0x9500, 0x1494c}, {0x957f, 0x15810}, {0x9600, 0x15f54},
		{0x967f, 0x16e18}, {0x9700, 0x1755c}, {0x977f, 0x18420}, {0x9800, 0x18b64},
		{0xffff, 0}
	};

	if (a0 >= 0x8140 && a0 <= 0x84be) {
		while (table_8140[i][0] <= a0) i++;
		a0 -= table_8140[i - 1][0];
		v0 = 0xbfc66000 + (a0 * 0x1e + table_8140[i - 1][1]);
	} else if (a0 >= 0x889f && a0 <= 0x9872) {
		while (table_889f[i][0] <= a0) i++;
		a0 -= table_889f[i - 1][0];
		v0 = 0xbfc66000 + (a0 * 0x1e + table_889f[i - 1][1]);
	} else {
		v0 = 0xffffffff;
	}

	pc0 = ra;
}

/* According to a user, this allows Final Fantasy Tactics to save/load properly */
void psxBios__get_error(void) // 55
{ 
	v0 = 0;
	pc0 = ra;
}

void psxBios_GetC0Table(void) { // 56
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosB0n[0x56]);
#endif

	v0 = 0x674;
	pc0 = ra;
}

void psxBios_GetB0Table(void) { // 57
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosB0n[0x57]);
#endif

	v0 = 0x874;
	pc0 = ra;
}

void psxBios__card_chan(void) { // 0x58
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosB0n[0x58]);
#endif

	v0 = card_active_chan;
	pc0 = ra;
}

void psxBios__card_wait(void) { // 5d
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s\n", biosB0n[0x5d]);
#endif
	/*
	01h=ready
	02h=busy/read
	04h=busy/write
	08h=busy/info
	11h=failed/timeout (eg. when no cartridge inserted)
	21h=failed/general error
	
	Games like Fade To Black and Final Fantasy 8 use this and card_info for memory card detection.
	Unfortunately, it's not implemented properly enough for either games to work.
	*/
	v0 = 0x1;
	pc0 = ra;
}


void psxBios_ChangeClearPad(void) { // 5b
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s: %x\n", biosB0n[0x5b], a0);
#endif	

	pc0 = ra;
}

void psxBios__card_status() { // 5c
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s: %x\n", biosB0n[0x5c], a0);
#endif

	v0 = 1;
	pc0 = ra;
}

/* System calls C0 */

/*
 * int SysEnqIntRP(int index , long *queue);
 */

void psxBios_SysEnqIntRP(void) { // 02
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s: %x\n", biosC0n[0x02] ,a0);
#endif

	SysIntRP[a0] = a1;

	v0 = 0;
	pc0 = ra;
}

/*
 * int SysDeqIntRP(int index , long *queue);
 */

void psxBios_SysDeqIntRP(void) { // 03
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s: %x\n", biosC0n[0x03], a0);
#endif

	SysIntRP[a0] = 0;

	v0 = 0;
	pc0 = ra;
}

void psxBios_ChangeClearRCnt(void) { // 0a
	u32 *ptr;

#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("psxBios_%s: %x, %x\n", biosC0n[0x0a], a0, a1);
#endif

	ptr = (u32*)PSXM((a0 << 2) + 0x8600);
	v0 = *ptr;
	*ptr = a1;

//	psxRegs.CP0.n.Status|= 0x404;
	pc0 = ra;

}

void psxBios_dummy(void) { 
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("unk %x call: %x\n", pc0 & 0x1fffff, t1);
#endif
	pc0 = ra; 
}

// Nocash documentation says that it is supposed to make the PSX crash. 
// However, it also says that on a PS2, it returns 0 instead so let's do that instead.
void psxBios_SystemError(void) { 
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("unk %x call: %x\n", pc0 & 0x1fffff, t1);
#endif
	v0 = 0;
	pc0 = ra; 
}

void psxBios_ReturnZero(void) { 
#ifdef PSXBIOS_LOG
	PSXBIOS_LOG("unk %x call: %x\n", pc0 & 0x1fffff, t1);
#endif
	v0 = 0;
	pc0 = ra; 
}

void (*biosA0[256])(void);
void (*biosB0[256])(void);
void (*biosC0[256])(void);

#include "sjisfont.h"

void psxBiosInit(void) {
	u32 base, size;
	u32 *ptr; 
	int i;
	uLongf len;

	for(i = 0; i < 256; i++) {
		biosA0[i] = NULL;
		biosB0[i] = NULL;
		biosC0[i] = NULL;
	}
	biosA0[0x3e] = psxBios_puts;
	biosA0[0x3f] = psxBios_printf;

	biosB0[0x3d] = psxBios_putchar;
	biosB0[0x3f] = psxBios_puts;

	if (!Config.HLE) return;

	for(i = 0; i < 256; i++) {
		if (biosA0[i] == NULL) biosA0[i] = psxBios_dummy;
		if (biosB0[i] == NULL) biosB0[i] = psxBios_dummy;
		if (biosC0[i] == NULL) biosC0[i] = psxBios_dummy;
	}

    biosA0[0x00] = psxBios_open;
	biosA0[0x01] = psxBios_lseek;
	biosA0[0x02] = psxBios_read;
	biosA0[0x03] = psxBios_write;
	biosA0[0x04] = psxBios_close;
	//biosA0[0x05] = psxBios_ioctl;
	//biosA0[0x06] = psxBios_exit;
	//biosA0[0x07] = psxBios_sys_a0_07;
	biosA0[0x08] = psxBios_getc;
	biosA0[0x09] = psxBios_putc;
	biosA0[0x0a] = psxBios_todigit;
	//biosA0[0x0b] = psxBios_atof;
	//biosA0[0x0c] = psxBios_strtoul;
	//biosA0[0x0d] = psxBios_strtol;
	biosA0[0x0e] = psxBios_abs;
	biosA0[0x0f] = psxBios_labs;
    biosA0[0x10] = psxBios_atoi;
    biosA0[0x11] = psxBios_atol;
	//biosA0[0x12] = psxBios_atob;
	biosA0[0x13] = psxBios_setjmp;
	biosA0[0x14] = psxBios_longjmp;
	biosA0[0x15] = psxBios_strcat;
	biosA0[0x16] = psxBios_strncat;
	biosA0[0x17] = psxBios_strcmp;
	biosA0[0x18] = psxBios_strncmp;
	biosA0[0x19] = psxBios_strcpy;
	biosA0[0x1a] = psxBios_strncpy;
	biosA0[0x1b] = psxBios_strlen;
	biosA0[0x1c] = psxBios_index;
	biosA0[0x1d] = psxBios_rindex;
	biosA0[0x1e] = psxBios_strchr;
	biosA0[0x1f] = psxBios_strrchr;
	biosA0[0x20] = psxBios_strpbrk;
	biosA0[0x21] = psxBios_strspn;
	biosA0[0x22] = psxBios_strcspn;
	biosA0[0x23] = psxBios_strtok;
	biosA0[0x24] = psxBios_strstr;
    biosA0[0x25] = psxBios_toupper;
    biosA0[0x26] = psxBios_tolower;
    biosA0[0x27] = psxBios_bcopy;
    biosA0[0x28] = psxBios_bzero;
    biosA0[0x29] = psxBios_bcmp;
	biosA0[0x2a] = psxBios_memcpy;
	biosA0[0x2b] = psxBios_memset;
	biosA0[0x2c] = psxBios_memmove;
	biosA0[0x2d] = psxBios_memcmp;
	biosA0[0x2e] = psxBios_memchr;
	biosA0[0x2f] = psxBios_rand;
	biosA0[0x30] = psxBios_srand;
	biosA0[0x31] = psxBios_qsort;
	//biosA0[0x32] = psxBios_strtod;
	biosA0[0x33] = psxBios_malloc;
	biosA0[0x34] = psxBios_free;
    //biosA0[0x35] = psxBios_lsearch;
    //biosA0[0x36] = psxBios_bsearch;
    biosA0[0x37] = psxBios_calloc;
    biosA0[0x38] = psxBios_realloc;
	biosA0[0x39] = psxBios_InitHeap;
    //biosA0[0x3a] = psxBios__exit;
	biosA0[0x3b] = psxBios_getchar;
	biosA0[0x3c] = psxBios_putchar;	
	//biosA0[0x3d] = psxBios_gets;
	//biosA0[0x40] = psxBios_sys_a0_40;
	//biosA0[0x41] = psxBios_LoadTest;
	biosA0[0x42] = psxBios_Load;
	biosA0[0x43] = psxBios_Exec;
	biosA0[0x44] = psxBios_FlushCache;
    //biosA0[0x45] = psxBios_InstallInterruptHandler;
	biosA0[0x46] = psxBios_GPU_dw;
	biosA0[0x47] = psxBios_mem2vram;
	biosA0[0x48] = psxBios_SendGPU;
	biosA0[0x49] = psxBios_GPU_cw;
    biosA0[0x4a] = psxBios_GPU_cwb;
	biosA0[0x4b] = psxBios_GPU_SendPackets;
    biosA0[0x4c] = psxBios_sys_a0_4c;
	biosA0[0x4d] = psxBios_GPU_GetGPUStatus;
	//biosA0[0x4e] = psxBios_GPU_sync;	
	biosA0[0x4f] = psxBios_SystemError;
	biosA0[0x50] = psxBios_SystemError;
	biosA0[0x51] = psxBios_LoadExec;
	//biosA0[0x52] = psxBios_GetSysSp; // Nocash says SystemError, PSX doc says GetSysSp ?
	biosA0[0x53] = psxBios_SystemError;
	//biosA0[0x54] = psxBios__96_init_a54;
	//biosA0[0x55] = psxBios__bu_init_a55;
	//biosA0[0x56] = psxBios__96_remove_a56;
	biosA0[0x57] = psxBios_ReturnZero;
	biosA0[0x58] = psxBios_ReturnZero;
	biosA0[0x59] = psxBios_ReturnZero;
	//biosA0[0x5a] = psxBios_sys_a0_5a;
	//biosA0[0x5b] = psxBios_dev_tty_init;
	//biosA0[0x5c] = psxBios_dev_tty_open;
	//biosA0[0x5d] = psxBios_sys_a0_5d;
	//biosA0[0x5e] = psxBios_dev_tty_ioctl;
	//biosA0[0x5f] = psxBios_dev_cd_open;
	//biosA0[0x60] = psxBios_dev_cd_read;
	//biosA0[0x61] = psxBios_dev_cd_close;
	//biosA0[0x62] = psxBios_dev_cd_firstfile;
	//biosA0[0x63] = psxBios_dev_cd_nextfile;
	//biosA0[0x64] = psxBios_dev_cd_chdir;
	//biosA0[0x65] = psxBios_dev_card_open;
	//biosA0[0x66] = psxBios_dev_card_read;
	//biosA0[0x67] = psxBios_dev_card_write;
	//biosA0[0x68] = psxBios_dev_card_close;
	//biosA0[0x69] = psxBios_dev_card_firstfile;
	//biosA0[0x6a] = psxBios_dev_card_nextfile;
	//biosA0[0x6b] = psxBios_dev_card_erase;
	//biosA0[0x6c] = psxBios_dev_card_undelete;
	//biosA0[0x6d] = psxBios_dev_card_format;
	//biosA0[0x6e] = psxBios_dev_card_rename;
	//biosA0[0x6f] = psxBios_dev_card_6f;
	biosA0[0x70] = psxBios__bu_init;
	biosA0[0x71] = psxBios__96_init;
	biosA0[0x72] = psxBios__96_remove;
	biosA0[0x73] = psxBios_ReturnZero;
	biosA0[0x74] = psxBios_ReturnZero;
	biosA0[0x75] = psxBios_ReturnZero;
	biosA0[0x76] = psxBios_ReturnZero;
	biosA0[0x77] = psxBios_ReturnZero;
	//biosA0[0x78] = psxBios__96_CdSeekL;
	biosA0[0x79] = psxBios_ReturnZero;
	biosA0[0x7a] = psxBios_ReturnZero;
	biosA0[0x7b] = psxBios_ReturnZero;
	//biosA0[0x7c] = psxBios__96_CdGetStatus;
	biosA0[0x7d] = psxBios_ReturnZero;
	//biosA0[0x7e] = psxBios__96_CdRead;
	biosA0[0x7f] = psxBios_ReturnZero;
	biosA0[0x80] = psxBios_ReturnZero;
	//biosA0[0x81] = psxBios_sys_a0_81;
	biosA0[0x82] = psxBios_ReturnZero;		
	biosA0[0x83] = psxBios_ReturnZero;
	biosA0[0x84] = psxBios_ReturnZero;
	//biosA0[0x85] = psxBios__96_CdStop;	
	biosA0[0x86] = psxBios_ReturnZero;
	biosA0[0x87] = psxBios_ReturnZero;
	biosA0[0x88] = psxBios_ReturnZero;
	biosA0[0x89] = psxBios_ReturnZero;
	biosA0[0x8a] = psxBios_ReturnZero;
	biosA0[0x8b] = psxBios_ReturnZero;
	biosA0[0x8c] = psxBios_ReturnZero;
	biosA0[0x8d] = psxBios_ReturnZero;
	biosA0[0x8e] = psxBios_ReturnZero;
	biosA0[0x8f] = psxBios_ReturnZero;
	//biosA0[0x90] = psxBios_sys_a0_90;
	//biosA0[0x91] = psxBios_sys_a0_91;
	//biosA0[0x92] = psxBios_sys_a0_92;
	//biosA0[0x93] = psxBios_sys_a0_93;
	//biosA0[0x94] = psxBios_sys_a0_94;
	//biosA0[0x95] = psxBios_sys_a0_95;
	//biosA0[0x96] = psxBios_AddCDROMDevice;
	//biosA0[0x97] = psxBios_AddMemCardDevide;
	//biosA0[0x98] = psxBios_DisableKernelIORedirection;
	//biosA0[0x99] = psxBios_EnableKernelIORedirection;
	biosA0[0x9a] = psxBios_SystemError;
	biosA0[0x9b] = psxBios_SystemError;
	//biosA0[0x9c] = psxBios_SetConf;
	//biosA0[0x9d] = psxBios_GetConf;
	//biosA0[0x9e] = psxBios_sys_a0_9e;
	biosA0[0x9f] = psxBios_SetMem;
	//biosA0[0xa0] = psxBios__boot;
	//biosA0[0xa1] = psxBios_SystemError;
	//biosA0[0xa2] = psxBios_EnqueueCdIntr;
	//biosA0[0xa3] = psxBios_DequeueCdIntr;
	//biosA0[0xa4] = psxBios_sys_a0_a4;
	//biosA0[0xa5] = psxBios_ReadSector;
	//biosA0[0xa6] = psxBios_get_cd_status;
	//biosA0[0xa7] = psxBios_bufs_cb_0;
	//biosA0[0xa8] = psxBios_bufs_cb_1;
	//biosA0[0xa9] = psxBios_bufs_cb_2;
	//biosA0[0xaa] = psxBios_bufs_cb_3;
	biosA0[0xab] = psxBios__card_info;
	biosA0[0xac] = psxBios__card_load;
	//biosA0[0axd] = psxBios__card_auto;
	//biosA0[0xae] = psxBios_bufs_cd_4;
	//biosA0[0xaf] = psxBios_sys_a0_af;
	biosA0[0xb0] = psxBios_ReturnZero;
	biosA0[0xb1] = psxBios_ReturnZero;
	//biosA0[0xb2] = psxBios_do_a_long_jmp
	biosA0[0xb3] = psxBios_ReturnZero;
	//biosA0[0xb4] = psxBios_sub_function;
//*******************B0 CALLS****************************
	//biosB0[0x00] = psxBios_SysMalloc;
	//biosB0[0x01] = psxBios_sys_b0_01;
	biosB0[0x02] = psxBios_SetRCnt;
	biosB0[0x03] = psxBios_GetRCnt;
	biosB0[0x04] = psxBios_StartRCnt;
	biosB0[0x05] = psxBios_StopRCnt;
	biosB0[0x06] = psxBios_ResetRCnt;
	biosB0[0x07] = psxBios_DeliverEvent;
	biosB0[0x08] = psxBios_OpenEvent;
	biosB0[0x09] = psxBios_CloseEvent;
	biosB0[0x0a] = psxBios_WaitEvent;
	biosB0[0x0b] = psxBios_TestEvent;
	biosB0[0x0c] = psxBios_EnableEvent;
	biosB0[0x0d] = psxBios_DisableEvent;
    biosB0[0x0e] = psxBios_OpenTh;
    biosB0[0x0f] = psxBios_CloseTh;
    biosB0[0x10] = psxBios_ChangeTh;
    //biosB0[0x11] = psxBios_psxBios_b0_11;
	biosB0[0x12] = psxBios_InitPAD;
	biosB0[0x13] = psxBios_StartPAD;
	biosB0[0x14] = psxBios_StopPAD;
	biosB0[0x15] = psxBios_PAD_init;
	biosB0[0x16] = psxBios_PAD_dr;
	biosB0[0x17] = psxBios_ReturnFromException;
	biosB0[0x18] = psxBios_ResetEntryInt;
	biosB0[0x19] = psxBios_HookEntryInt;
	biosB0[0x1a] = psxBios_SystemError;
	biosB0[0x1b] = psxBios_SystemError;
	biosB0[0x1c] = psxBios_SystemError;
	biosB0[0x1d] = psxBios_SystemError;
	biosB0[0x1e] = psxBios_SystemError;
	biosB0[0x1f] = psxBios_SystemError;
	biosB0[0x20] = psxBios_UnDeliverEvent;
	biosB0[0x21] = psxBios_SystemError;
	biosB0[0x22] = psxBios_SystemError;
	biosB0[0x23] = psxBios_SystemError;
	//biosB0[0x24] = psxBios_sys_b0_24;
	//biosB0[0x25] = psxBios_sys_b0_25;
	//biosB0[0x26] = psxBios_sys_b0_26;
	//biosB0[0x27] = psxBios_sys_b0_27;
	//biosB0[0x28] = psxBios_sys_b0_28;
	//biosB0[0x29] = psxBios_sys_b0_29;
	biosB0[0x2a] = psxBios_SystemError;
	biosB0[0x2b] = psxBios_SystemError;
	//biosB0[0x2c] = psxBios_sys_b0_2c;
	//biosB0[0x2d] = psxBios_sys_b0_2d;
	//biosB0[0x2e] = psxBios_sys_b0_2e;
	//biosB0[0x2f] = psxBios_sys_b0_2f;
	//biosB0[0x30] = psxBios_sys_b0_30;
	//biosB0[0x31] = psxBios_sys_b0_31;
    biosB0[0x32] = psxBios_open;
	biosB0[0x33] = psxBios_lseek;
	biosB0[0x34] = psxBios_read;
	biosB0[0x35] = psxBios_write;
    biosB0[0x36] = psxBios_close;
	//biosB0[0x37] = psxBios_ioctl;
	//biosB0[0x38] = psxBios_exit;
	//biosB0[0x39] = psxBios_sys_b0_39;
	//biosB0[0x3a] = psxBios_getc;
	//biosB0[0x3b] = psxBios_putc;
	biosB0[0x3c] = psxBios_getchar;
	//biosB0[0x3e] = psxBios_gets;
	//biosB0[0x40] = psxBios_cd;
	biosB0[0x41] = psxBios_format;
	biosB0[0x42] = psxBios_firstfile;
	biosB0[0x43] = psxBios_nextfile;
	biosB0[0x44] = psxBios_rename;
	biosB0[0x45] = psxBios_delete;
	//biosB0[0x46] = psxBios_undelete;
	//biosB0[0x47] = psxBios_AddDevice;
	//biosB0[0x48] = psxBios_RemoteDevice;
	//biosB0[0x49] = psxBios_PrintInstalledDevices;
	biosB0[0x4a] = psxBios_InitCARD;
	biosB0[0x4b] = psxBios_StartCARD;
	biosB0[0x4c] = psxBios_StopCARD;
	//biosB0[0x4d] = psxBios_sys_b0_4d;
	biosB0[0x4e] = psxBios__card_write;
	biosB0[0x4f] = psxBios__card_read;
	biosB0[0x50] = psxBios__new_card;
	biosB0[0x51] = psxBios_Krom2RawAdd;
	//biosB0[0x52] = psxBios_sys_b0_52;
	//biosB0[0x53] = psxBios_sys_b0_53;
	//biosB0[0x54] = psxBios__get_errno;
	biosB0[0x55] = psxBios__get_error;
	biosB0[0x56] = psxBios_GetC0Table;
	biosB0[0x57] = psxBios_GetB0Table;
	biosB0[0x58] = psxBios__card_chan;
	//biosB0[0x59] = psxBios_sys_b0_59;
	biosB0[0x5a] = psxBios_SystemError;
	biosB0[0x5b] = psxBios_ChangeClearPad;
	biosB0[0x5c] = psxBios__card_status;
	biosB0[0x5d] = psxBios__card_wait;
//*******************C0 CALLS****************************
	//biosC0[0x00] = psxBios_InitRCnt;
	//biosC0[0x01] = psxBios_InitException;
	biosC0[0x02] = psxBios_SysEnqIntRP;
	biosC0[0x03] = psxBios_SysDeqIntRP;
	//biosC0[0x04] = psxBios_get_free_EvCB_slot;
	//biosC0[0x05] = psxBios_get_free_TCB_slot;
	//biosC0[0x06] = psxBios_ExceptionHandler;
	//biosC0[0x07] = psxBios_InstallExeptionHandler;
	//biosC0[0x08] = psxBios_SysInitMemory;
	//biosC0[0x09] = psxBios_SysInitKMem;
	biosC0[0x0a] = psxBios_ChangeClearRCnt;	
	biosC0[0x0b] = psxBios_SystemError;
	//biosC0[0x0c] = psxBios_InitDefInt;
    //biosC0[0x0d] = psxBios_sys_c0_0d;
	biosC0[0x0e] = psxBios_ReturnZero;
	biosC0[0x0f] = psxBios_ReturnZero;
	biosC0[0x10] = psxBios_ReturnZero;
	biosC0[0x11] = psxBios_ReturnZero;
	//biosC0[0x12] = psxBios_InstallDevices;
	//biosC0[0x13] = psxBios_FlushStfInOutPut;
	biosC0[0x14] = psxBios_ReturnZero;
	//biosC0[0x15] = psxBios__cdevinput;
	//biosC0[0x16] = psxBios__cdevscan;
	//biosC0[0x17] = psxBios__circgetc;
	//biosC0[0x18] = psxBios__circputc;
	//biosC0[0x19] = psxBios_ioabort;
	//biosC0[0x1a] = psxBios_sys_c0_1a
	//biosC0[0x1b] = psxBios_KernelRedirect;
	//biosC0[0x1c] = psxBios_PatchAOTable;
//************** THE END ***************************************
/**/
	base = 0x1000;
	size = sizeof(EvCB) * 32;
	Event = (EvCB *)&psxR[base]; base+= size*6;
	memset(Event, 0, size * 6);
	HwEV = Event;
	EvEV = Event + 32;
	RcEV = Event + 32*2;
	UeEV = Event + 32*3;
	SwEV = Event + 32*4;
	ThEV = Event + 32*5;

	ptr = (u32*)&psxM[0x0874]; // b0 table
	ptr[0] = SWAPu32(0x4c54 - 0x884);

	ptr = (u32*)&psxM[0x0674]; // c0 table
	ptr[6] = SWAPu32(0xc80);

	memset(SysIntRP, 0, sizeof(SysIntRP));
	memset(Thread, 0, sizeof(Thread));
	Thread[0].status = 2; // main thread

	jmp_int = NULL;
	pad_buf = NULL;
	pad_buf1 = NULL;
	pad_buf2 = NULL;
	pad_buf1len = pad_buf2len = 0;
	heap_addr = NULL;
	heap_end = NULL;
	heap_size = 0;
	CardState = -1;
	CurThread = 0;
	memset(FDesc, 0, sizeof(FDesc));
	card_active_chan = 0;

	psxMu32ref(0x0150) = SWAPu32(0x160);
	psxMu32ref(0x0154) = SWAPu32(0x320);
	psxMu32ref(0x0160) = SWAPu32(0x248);
	strcpy((char*)&psxM[0x248], "bu");
/*	psxMu32ref(0x0ca8) = SWAPu32(0x1f410004);
	psxMu32ref(0x0cf0) = SWAPu32(0x3c020000);
	psxMu32ref(0x0cf4) = SWAPu32(0x2442641c);
	psxMu32ref(0x09e0) = SWAPu32(0x43d0);
	psxMu32ref(0x4d98) = SWAPu32(0x946f000a);
*/
	// opcode HLE
	psxRu32ref(0x0000) = SWAPu32((0x3b << 26) | 4);
	/* Whatever this does, it actually breaks CTR, even without the uninitiliazed memory patch. 
	Normally games shouldn't read from address 0 yet they do. See explanation below in details. */
	//psxMu32ref(0x0000) = SWAPu32((0x3b << 26) | 0);
	psxMu32ref(0x00a0) = SWAPu32((0x3b << 26) | 1);
	psxMu32ref(0x00b0) = SWAPu32((0x3b << 26) | 2);
	psxMu32ref(0x00c0) = SWAPu32((0x3b << 26) | 3);
	psxMu32ref(0x4c54) = SWAPu32((0x3b << 26) | 0);
	psxMu32ref(0x8000) = SWAPu32((0x3b << 26) | 5);
	psxMu32ref(0x07a0) = SWAPu32((0x3b << 26) | 0);
	psxMu32ref(0x0884) = SWAPu32((0x3b << 26) | 0);
	psxMu32ref(0x0894) = SWAPu32((0x3b << 26) | 0);

	// memory size 2 MB
	psxMu32ref(0x6c80) = SWAPu32(0x000085c8);

	// initial RNG seed
	psxMu32ref(0x9010) = SWAPu32(0xac20cc00);

	// fonts
	len = 0x80000 - 0x66000;
	uncompress((Bytef *)(psxR + 0x66000), &len, font_8140, sizeof(font_8140));
	len = 0x80000 - 0x69d68;
	uncompress((Bytef *)(psxR + 0x69d68), &len, font_889f, sizeof(font_889f));

	// memory size 2 MB
	psxHu32ref(0x1060) = SWAPu32(0x00000b88);
	
	/*	Some games like R-Types, CTR, Fade to Black read from adress 0x00000000 due to uninitialized pointers.
		See Garbage Area at Address 00000000h in Nocash PSX Specfications for more information.
		Here are some examples of games not working with this fix in place :
		R-type won't get past the Irem logo if not implemented.
		Crash Team Racing will softlock after the Sony logo.
	*/
	
	psxMu32ref(0x0000) = SWAPu32(0x00000003);
	/*
	But overwritten by 00000003h after soon.
	psxMu32ref(0x0000) = SWAPu32(0x00001A3C);
	*/
	psxMu32ref(0x0004) = SWAPu32(0x800C5A27);
	psxMu32ref(0x0008) = SWAPu32(0x08000403);
	psxMu32ref(0x000C) = SWAPu32(0x00000000);
	
	psxMu32ref(0x0064) = SWAPu32(0x00000000);
	psxMu32ref(0x0068) = SWAPu32(0xFF000000);
}

void psxBiosShutdown() {
}


#define psxBios_PADpoll(pad) { \
	PAD##pad##_startPoll(); \
	pad_buf##pad[0] = 0; \
	pad_buf##pad[1] = PAD##pad##_poll(0x42); \
	if (!(pad_buf##pad[1] & 0x0f)) { \
		bufcount = 32; \
	} else { \
		bufcount = (pad_buf##pad[1] & 0x0f) * 2; \
	} \
	PAD##pad##_poll(0); \
	i = 2; \
	while (bufcount--) { \
		pad_buf##pad[i++] = PAD##pad##_poll(0); \
	} \
}

void biosInterrupt(void) {
	int i,bufcount;

		if (pad_buf != NULL) {
			u32 *buf = (u32*)pad_buf;

				PAD1_startPoll();
				if (PAD1_poll(0x42) == 0x23) {
					PAD1_poll(0);
					*buf = PAD1_poll(0) << 8;
					*buf|= PAD1_poll(0);
					PAD1_poll(0);
					*buf&= ~((PAD1_poll(0)>0x20)?1<<6:0);
					*buf&= ~((PAD1_poll(0)>0x20)?1<<7:0);
				} else {
					PAD1_poll(0);
					*buf = PAD1_poll(0) << 8;
					*buf|= PAD1_poll(0);
				}

				PAD2_startPoll();
				if (PAD2_poll(0x42) == 0x23) {
					PAD2_poll(0);
					*buf|= PAD2_poll(0) << 24;
					*buf|= PAD2_poll(0) << 16;
					PAD2_poll(0);
					*buf&= ~((PAD2_poll(0)>0x20)?1<<22:0);
					*buf&= ~((PAD2_poll(0)>0x20)?1<<23:0);
				} else {
					PAD2_poll(0);
					*buf|= PAD2_poll(0) << 24;
					*buf|= PAD2_poll(0) << 16;
				}

		}
		if (!pad_stopped) {
			if (pad_buf1) {
				psxBios_PADpoll(1);
			}

			if (pad_buf2) {
				psxBios_PADpoll(2);
			}
		}

	if (psxHu32(0x1070) & 0x1) { // Vsync
		if (RcEV[3][1].status == EvStACTIVE) {
			softCall(RcEV[3][1].fhandler);
		}
	}

	if (psxHu32(0x1070) & 0x70) { // Rcnt 0,1,2
		for (i=0; i<3; i++) {
			if (psxHu32(0x1070) & (1 << (i+4))) {
				if (RcEV[i][1].status == EvStACTIVE) {
					softCall(RcEV[i][1].fhandler);
				}
				psxHwWrite32(0x1f801070, ~(1 << (i+4)));
				ResetIoCycle();
			}
		}
	}
}

void psxBiosException(void) {
	int i;

	switch (psxRegs.CP0.n.Cause & 0x3c) {
		case 0x00: // Interrupt
			interrupt_r26=psxRegs.CP0.n.EPC;
			SaveRegs();

			sp = psxMu32(0x6c80); // create new stack for interrupt handlers

			biosInterrupt();

			for (i=0; i<8; i++) {
				if (SysIntRP[i]) {
					u32 *queue = (u32*)PSXM(SysIntRP[i]);

					s0 = queue[2];
					softCall(queue[1]);
				}
			}

			if (jmp_int != NULL) {

				psxHwWrite32(0x1f801070, 0xffffffff);
				ResetIoCycle();

				ra = jmp_int[0];
				sp = jmp_int[1];
				fp = jmp_int[2];
				for (i=0; i<8; i++) // s0-s7
					 psxRegs.GPR.r[16+i] = jmp_int[3+i];
				gp = jmp_int[11];

				v0 = 1;
				pc0 = ra;
				return;
			}
			psxHwWrite16(0x1f801070, 0);
			ResetIoCycle();
			break;

		case 0x20: // Syscall
			switch (a0) {
				case 1: // EnterCritical - disable irq's
					/* Fixes Medievil 2 not loading up new game, Digimon World not booting up and possibly others */
					v0 = (psxRegs.CP0.n.Status & 0x404) == 0x404;
					psxRegs.CP0.n.Status &= ~0x404;
					//v0=1;	// HDHOSHY experimental patch: Spongebob, Coldblood, fearEffect, Medievil2, Martian Gothic
					break;

				case 2: // ExitCritical - enable irq's
					psxRegs.CP0.n.Status|= 0x404; 
					break;
				/* Normally this should cover SYS(00h, SYS(04h but they don't do anything relevant so... */
				default:
					break;
			}
			ResetIoCycle();
			pc0 = psxRegs.CP0.n.EPC + 4;
			k0 = pc0;

			psxRegs.CP0.n.Status = (psxRegs.CP0.n.Status & 0xfffffff0) |
								  ((psxRegs.CP0.n.Status & 0x3c) >> 2);
			ResetIoCycle();
			return;

		default:
			break;
	}

	pc0 = psxRegs.CP0.n.EPC;
	if (psxRegs.CP0.n.Cause & 0x80000000) pc0+=4;

	psxRegs.CP0.n.Status = (psxRegs.CP0.n.Status & 0xfffffff0) |
						  ((psxRegs.CP0.n.Status & 0x3c) >> 2);
	ResetIoCycle();
}

#define bfreeze(ptr, size) { \
	if (Mode == 1) memcpy(&psxR[base], ptr, size); \
	if (Mode == 0) memcpy(ptr, &psxR[base], size); \
	base += size; \
}

#define bfreezes(ptr) bfreeze(ptr, sizeof(ptr))
#define bfreezel(ptr) bfreeze(ptr, sizeof(*ptr))

#define bfreezepsxMptr(ptr, type) { \
	if (Mode == 1) { \
		if (ptr) psxRu32ref(base) = SWAPu32((s8 *)(ptr) - psxM); \
		else psxRu32ref(base) = 0; \
	} else { \
		if (psxRu32(base) != 0) ptr = (type *)(psxM + psxRu32(base)); \
		else (ptr) = NULL; \
	} \
	base += sizeof(u32); \
}

void psxBiosFreeze(int Mode) {
	u32 base = 0x40000;

	pad_stopped = 0;
	bfreezepsxMptr(jmp_int, u32);
	bfreezepsxMptr(pad_buf, int);
	bfreezepsxMptr(pad_buf1, char);
	bfreezepsxMptr(pad_buf2, char);
	bfreezepsxMptr(heap_addr, u32);
	bfreezel(&pad_buf1len);
	bfreezel(&pad_buf2len);
	bfreezes(regs);
	bfreezes(SysIntRP);
	bfreezel(&CardState);
	bfreezes(Thread);
	bfreezel(&CurThread);
	bfreezes(FDesc);
	bfreezel(&card_active_chan);
}
