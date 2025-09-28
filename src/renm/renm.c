/* Abnormal General Software License v2 (ABGSLV2)
This Abnormal General Software License Version 2 ("License") is a legal agreement between the original author ("Licensor")
and the individual or entity using the software, source code, or executable binary licensed under this agreement ("Licensee").
By using, modifying, or distributing the software, Licensee agrees to abide by the terms set forth herein.
https://xcn.abby0666.xyz/abgslv2.htm
https://github.com/AM51
$ gcc -o ../renm.exe renm.c -Os -s -nostdlib -lkernel32 -lshell32 -Wl,-e,wWinMainCRTStartup -Wl,--subsystem,console -fno-stack-protector -fno-builtin -fno-stack-check -fno-asynchronous-unwind-tables -fno-unwind-tables -fno-exceptions -fmerge-constants -ffunction-sections -fdata-sections -Wl,--gc-sections -m64 -Wl,--build-id=none -fno-align-labels
*/

typedef void* HANDLE;
typedef unsigned long DWORD;
typedef int BOOL;
typedef unsigned short WORD;
typedef unsigned int UINT;
typedef unsigned long long ULONGLONG;
typedef HANDLE HGLOBAL;
typedef unsigned short wchar_t;
#define NULL ((void*)0)
#define INVALID_HANDLE_VALUE ((HANDLE)(intptr_t)-1)
typedef unsigned long long size_t;
typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef long long intptr_t;
typedef unsigned long long uintptr_t;
typedef struct _FILETIME { DWORD dwLowDateTime; DWORD dwHighDateTime; } FILETIME;
typedef struct _WIN32_FIND_DATAW {
	DWORD dwFileAttributes;
	FILETIME ftCreationTime;
	FILETIME ftLastAccessTime;
	FILETIME ftLastWriteTime;
	DWORD nFileSizeHigh;
	DWORD nFileSizeLow;
	DWORD dwReserved0;
	DWORD dwReserved1;
	wchar_t cFileName[260];
	wchar_t cAlternateFileName[14];
} WIN32_FIND_DATAW;
typedef HANDLE(__stdcall* tGetStdHandle)(DWORD);
typedef BOOL(__stdcall* tGetConsoleMode)(HANDLE, DWORD*);
typedef BOOL(__stdcall* tSetConsoleMode)(HANDLE, DWORD);
typedef BOOL(__stdcall* tWriteFile)(HANDLE, const void*, DWORD, DWORD*, void*);
typedef BOOL(__stdcall* tMoveFileExW)(const wchar_t*, const wchar_t*, DWORD);
typedef HANDLE(__stdcall* tFindFirstFileW)(const wchar_t*, WIN32_FIND_DATAW*);
typedef BOOL(__stdcall* tFindNextFileW)(HANDLE, WIN32_FIND_DATAW*);
typedef BOOL(__stdcall* tFindClose)(HANDLE);
typedef HANDLE(__stdcall* tGetProcessHeap)(void);
typedef void* (__stdcall* tHeapAlloc)(HANDLE, DWORD, size_t);
typedef void* (__stdcall* tHeapReAlloc)(HANDLE, DWORD, void*, size_t);
typedef BOOL(__stdcall* tHeapFree)(HANDLE, DWORD, void*);
typedef wchar_t* (__stdcall* tGetCommandLineW)(void);
typedef HGLOBAL(__stdcall* tLocalFree)(HGLOBAL);
typedef ULONGLONG(__stdcall* tGetTickCount64)(void);
typedef void(__stdcall* tExitProcess)(UINT);
typedef wchar_t** (__stdcall* tCommandLineToArgvW)(const wchar_t*, int*);
typedef void* (__stdcall* tLoadLibraryA)(const char*);
typedef void* (__stdcall* tGetProcAddress)(void*, const char*);
typedef void* (__stdcall* tGetModuleHandleA)(const char*);
typedef void* (__stdcall* tGetModuleHandleW)(const wchar_t*);
static tGetStdHandle pGetStdHandle;
static tGetConsoleMode pGetConsoleMode;
static tSetConsoleMode pSetConsoleMode;
static tWriteFile pWriteFile;
static tMoveFileExW pMoveFileExW;
static tFindFirstFileW pFindFirstFileW;
static tFindNextFileW pFindNextFileW;
static tFindClose pFindClose;
static tGetProcessHeap pGetProcessHeap;
static tHeapAlloc pHeapAlloc;
static tHeapReAlloc pHeapReAlloc;
static tHeapFree pHeapFree;
static tGetCommandLineW pGetCommandLineW;
static tLocalFree pLocalFree;
static tGetTickCount64 pGetTickCount64;
static tExitProcess pExitProcess;
static tCommandLineToArgvW pCommandLineToArgvW;
static tLoadLibraryA pLoadLibraryA;
static tGetProcAddress pGetProcAddress;
static tGetModuleHandleA pGetModuleHandleA;
static void resolveAll() {
	char k[]={ 'k','e','r','n','e','l','3','2','.','d','l','l',0 };
	char s[]={ 's','h','e','l','l','3','2','.','d','l','l',0 };
	char gm[]={ 'G','e','t','M','o','d','u','l','e','H','a','n','d','l','e','A',0 };
	char gpa[]={ 'G','e','t','P','r','o','c','A','d','d','r','e','s','s',0 };
	char clf[]={ 'C','o','m','m','a','n','d','L','i','n','e','T','o','A','r','g','v','W',0 };
	char glc[]={ 'G','e','t','C','o','m','m','a','n','d','L','i','n','e','W',0 };
	char gh[]={ 'G','e','t','H','e','a','p','P','r','o','c','e','s','s','I','d',0 };
	extern void* __stdcall GetModuleHandleA(const char*);
	extern void* __stdcall GetProcAddress(void*, const char*);
	void* hK=GetModuleHandleA(k);
	pGetProcAddress=(tGetProcAddress)GetProcAddress;
	pGetModuleHandleA=(tGetModuleHandleA)GetModuleHandleA;
	pGetStdHandle=(tGetStdHandle)pGetProcAddress(hK, "GetStdHandle");
	pGetConsoleMode=(tGetConsoleMode)pGetProcAddress(hK, "GetConsoleMode");
	pSetConsoleMode=(tSetConsoleMode)pGetProcAddress(hK, "SetConsoleMode");
	pWriteFile=(tWriteFile)pGetProcAddress(hK, "WriteFile");
	pMoveFileExW=(tMoveFileExW)pGetProcAddress(hK, "MoveFileExW");
	pFindFirstFileW=(tFindFirstFileW)pGetProcAddress(hK, "FindFirstFileW");
	pFindNextFileW=(tFindNextFileW)pGetProcAddress(hK, "FindNextFileW");
	pFindClose=(tFindClose)pGetProcAddress(hK, "FindClose");
	pGetProcessHeap=(tGetProcessHeap)pGetProcAddress(hK, "GetProcessHeap");
	pHeapAlloc=(tHeapAlloc)pGetProcAddress(hK, "HeapAlloc");
	pHeapReAlloc=(tHeapReAlloc)pGetProcAddress(hK, "HeapReAlloc");
	pHeapFree=(tHeapFree)pGetProcAddress(hK, "HeapFree");
	pGetCommandLineW=(tGetCommandLineW)pGetProcAddress(hK, "GetCommandLineW");
	pLocalFree=(tLocalFree)pGetProcAddress(hK, "LocalFree");
	pGetTickCount64=(tGetTickCount64)pGetProcAddress(hK, "GetTickCount64");
	pExitProcess=(tExitProcess)pGetProcAddress(hK, "ExitProcess");
	pLoadLibraryA=(tLoadLibraryA)pGetProcAddress(hK, "LoadLibraryA");
	void* hS=pLoadLibraryA(s);
	pCommandLineToArgvW=(tCommandLineToArgvW)pGetProcAddress(hS, "CommandLineToArgvW");
}
static int gRenamed=0, gDryRun=0, gFailed=0, gSkipped=0;
void* memset(void* d, int c, size_t n) {
	__asm__ volatile("rep stosb" : "=D"(d), "=c"(n) : "0"(d), "1"(n), "a"(c) : "memory");
	return d;
}

void* memcpy(void* dst, const void* src, size_t n) {
	if(n==0) return dst;
	__asm__ volatile("rep movsb" : "+D"(dst), "+S"(src), "+c"(n) : : "memory");
	return dst;
}

#ifdef _M_X64
__attribute__((naked)) void ___chkstk_ms(void) {
	__asm__ volatile(
		"mov %rcx,%r10\n"
		"cmp $0x1000,%r10\n"
		"jb 1f\n"
		"0: sub $0x1000,%rsp\n"
		"test %rsp,(%rsp)\n"
		"sub $0x1000,%r10\n"
		"cmp $0x1000,%r10\n"
		"jae 0b\n"
		"1: sub %r10,%rsp\n"
		"test %rsp,(%rsp)\n"
		"ret"
		);
}
#endif

static unsigned int rng_seed=0;

void srand32(unsigned int s) {
	if(s==0) s=0xdeadbeef;
	rng_seed=s;
}

int rand32() {
	unsigned int ret;
	__asm__ volatile(
		"mov %[seed], %%eax\n"
		"test %%eax, %%eax\n"
		"jnz 1f\n"
		"mov $0xDEADBEEF, %%eax\n"
		"1:\n"
		"mov %%eax, %%ebx\n"
		"shl $13, %%eax\n"
		"xor %%ebx, %%eax\n"
		"mov %%eax, %%ebx\n"
		"shr $17, %%eax\n"
		"xor %%ebx, %%eax\n"
		"mov %%eax, %%ebx\n"
		"shl $5, %%eax\n"
		"xor %%ebx, %%eax\n"
		"mov %%eax, %[seed]\n"
		"and $0x7FFFFFFF, %%eax\n"
		"mov %%eax, %[out]\n"
		: [seed] "+m"(rng_seed), [out] "=r"(ret)
		:
		: "eax", "ebx"
		);
	return (int)ret;
}

static size_t mwcslen(const wchar_t* s) {
	const wchar_t* p=s;
	while(*p) p++;
	return (size_t)(p-s);
}

size_t mstrlen(const char* s) {
	size_t n=0;
	while(s[n]) n++;
	return n;
}

int mstricmp(const char* a, const char* b) {
	while(*a&&*b) {
		char c1=(*a>='A'&&*a<='Z') ? *a+32 : *a;
		char c2=(*b>='A'&&*b<='Z') ? *b+32 : *b;
		if(c1!=c2) return c1-c2;
		a++; b++;
	}
	return *a-*b;
}

char* mstrcpyA(char* d, const char* s) {
	char* p=d;
	while(*s) *p++=*s++;
	*p=0;
	return d;
}

char* mstrcatA(char* d, const char* s) {
	char* p=d;
	while(*p) p++;
	while(*s) *p++=*s++;
	*p=0;
	return d;
}

wchar_t* wccopy(wchar_t* d, const wchar_t* s) {
	wchar_t* p=d;
	while(*s) *p++=*s++;
	*p=0;
	return d;
}

wchar_t* wcCat(wchar_t* d, const wchar_t* s) {
	wchar_t* p=d;
	while(*p) p++;
	while(*s) *p++=*s++;
	*p=0;
	return d;
}

int wcCmp(const wchar_t* a, const wchar_t* b) {
	while(*a&&*b) {
		if(*a!=*b) return *a-*b;
		a++; b++;
	}
	return *a-*b;
}

int wcicmp(const wchar_t* a, const wchar_t* b) {
	while(*a&&*b) {
		wchar_t c1=(*a>=L'A'&&*a<=L'Z') ? *a+32 : *a;
		wchar_t c2=(*b>=L'A'&&*b<=L'Z') ? *b+32 : *b;
		if(c1!=c2) return c1-c2;
		a++; b++;
	}
	return *a-*b;
}

int toWideChar(const char* src, wchar_t* dst, int dstSize) {
	if(!src||!dst||dstSize<=0) return 0;
	int di=0;
	const uint8_t* p=(const uint8_t*)src;
	while(*p) {
		uint32_t cp=0;
		int n=0;
		if(*p<0x80) cp=*p, n=1;
		else if((*p&0xE0)==0xC0) cp=*p&0x1F, n=2;
		else if((*p&0xF0)==0xE0) cp=*p&0x0F, n=3;
		else if((*p&0xF8)==0xF0) cp=*p&0x07, n=4;
		else { p++; continue; }
		for(int i=1; i<n; i++) {
			uint8_t nb=p[i];
			if((nb&0xC0)!=0x80) return 0;
			cp=(cp<<6)|(nb&0x3F);
		}
		p+=n;
		if(cp<=0xFFFF) {
			if(di+1>=dstSize) { dst[di]=0; return 0; }
			dst[di++]=(wchar_t)cp;
		}
		else if(cp>=0x10000&&cp<=0x10FFFF) {
			if(di+2>=dstSize) { dst[di]=0; return 0; }
			uint32_t d=cp-0x10000;
			dst[di++]=(wchar_t)(0xD800+(d>>10));
			dst[di++]=(wchar_t)(0xDC00+(d&0x3FF));
		}
		else {
			if(di+1>=dstSize) { dst[di]=0; return 0; }
			dst[di++]=L'?';
		}
	}
	if(di>=dstSize) return 0;
	dst[di]=0;
	return 1;
}

int wideToUtf8(const wchar_t* src, char* dst, int dstSize) {
	if(!src||!dst||dstSize<=0) return 0;
	int di=0;
	while(*src) {
		uint32_t wc=(uint32_t)*src++;
		if(wc<=0x7F) { if(di+1>=dstSize) break; dst[di++]=(char)wc; }
		else if(wc<=0x7FF) { if(di+2>=dstSize) break; dst[di++]=(char)(0xC0|(wc>>6)); dst[di++]=(char)(0x80|(wc&0x3F)); }
		else if(wc>=0xD800&&wc<=0xDBFF) {
			if(*src>=0xDC00&&*src<=0xDFFF) {
				uint32_t w2=*src++;
				uint32_t cp=0x10000+(((wc-0xD800)<<10)|(w2-0xDC00));
				if(di+4>=dstSize) break;
				dst[di++]=(char)(0xF0|(cp>>18));
				dst[di++]=(char)(0x80|((cp>>12)&0x3F));
				dst[di++]=(char)(0x80|((cp>>6)&0x3F));
				dst[di++]=(char)(0x80|(cp&0x3F));
			}
		}
		else { if(di+3>=dstSize) break; dst[di++]=(char)(0xE0|(wc>>12)); dst[di++]=(char)(0x80|((wc>>6)&0x3F)); dst[di++]=(char)(0x80|(wc&0x3F)); }
	}
	if(di<dstSize) dst[di]=0; else dst[dstSize-1]=0;
	return 1;
}

char* intToStr(int v, char* b) {
	char t[32]; int n=0, neg=0;
	if(v==0) { b[0]='0'; b[1]=0; return b; }
	if(v<0) { neg=1; v=-v; }
	while(v>0) { t[n++]='0'+(v%10); v/=10; }
	char* p=b;
	if(neg) *p++='-';
	while(n>0) *p++=t[--n];
	*p=0;
	return b;
}

static HANDLE g_stdout_handle;

static void writeAnsi(const char* s, int len) { DWORD w; pWriteFile(g_stdout_handle, s, (DWORD)len, &w, 0); }
static void ansiRed() { const char s[5]={ '\x1b','[','9','1','m' }; writeAnsi(s, 5); }
static void ansiGreen() { const char s[5]={ '\x1b','[','9','2','m' }; writeAnsi(s, 5); }
static void ansiYellow() { const char s[5]={ '\x1b','[','9','3','m' }; writeAnsi(s, 5); }
static void ansiBlue() { const char s[5]={ '\x1b','[','9','4','m' }; writeAnsi(s, 5); }
static void ansiCyan() { const char s[5]={ '\x1b','[','9','6','m' }; writeAnsi(s, 5); }
static void ansiWhite() { const char s[5]={ '\x1b','[','9','7','m' }; writeAnsi(s, 5); }
static void ansiReset() { const char s[4]={ '\x1b','[','0','m' }; writeAnsi(s, 4); }

static long write_(int fd, const char* buf, size_t count) {
	if(fd!=1) return -1;
	DWORD written=0;
	BOOL r=pWriteFile(g_stdout_handle, buf, (DWORD)count, &written, 0);
	return r ? (long)written : -1;
}

void print(const char* m) {
	write_(1, m, mstrlen(m));
}

void printLine(const char* c, const char* m) {
	print(c);
	print(m);
	ansiReset();
	print("\n");
}

void cleanNameW(const wchar_t* in, char* out, int sz) {
	size_t j=0;
	int dash=0;
	for(size_t i=0; in[i]!=0&&j<(size_t)sz-1; i++) {
		wchar_t wc=in[i];
		if(wc<=127&&((wc>='a'&&wc<='z')||(wc>='A'&&wc<='Z')||(wc>='0'&&wc<='9'))) {
			out[j++]=(char)((wc>='A'&&wc<='Z') ? wc+32 : wc);
			dash=0;
		}
		else if(wc==L' '||wc==L'-'||wc==L'_'||wc==L'.') {
			if(!dash&&j>0) { out[j++]='-'; dash=1; }
		}
	}
	if(j>0&&out[j-1]=='-') j--;
	out[j]=0;
}

int hasValidExtensionW(const wchar_t* fn) {
	const wchar_t* d=fn;
	while(*d) d++;
	while(d>fn&&*d!=L'.') d--;
	if(*d!=L'.') return 0;
	char ext[32];
	wideToUtf8(d, ext, 32);
	const char* exts[]={ ".jpg", ".jpeg", ".png", ".gif", ".bmp", ".tiff", ".webp", ".mp4",
							".mov", ".avi", ".mkv", ".wmv", ".flv", ".webm", ".mpeg", ".mpg" };
	for(int i=0; i<16; i++)
		if(mstricmp(ext, exts[i])==0) return 1;
	return 0;
}

int endsWithNoGallery(const wchar_t* p) {
	const wchar_t* q=p;
	const wchar_t* l=NULL;
	while(*q) {
		if(*q==L'\\'||*q==L'/') l=q+1;
		q++;
	}
	if(!l) l=p;
	return (wcicmp(l, L".nogallery")==0);
}

int isInitialRoot(const wchar_t* c, const wchar_t* r) {
	if(wcCmp(r, L".")==0) return (wcCmp(c, L".")==0);
	return (wcicmp(c, r)==0);
}

int renameExW(const wchar_t* o, const wchar_t* n) { return pMoveFileExW(o, n, 0x00000001); }

char* intToStr_s(int v, char* b) { return intToStr(v, b); }

void printRename_full(const char* pc, const char* pt, const char* o, const char* n,
	const char* s, const char* sc, const char* oc, const char* nc) {
	print(pc); print(pt); print(oc); print(o); print(sc);
	print(" -> "); print(nc); print(n); print(sc); print(s);
	ansiReset(); print("\n");
}

void cleanAndRenameW(const wchar_t* d, WIN32_FIND_DATAW* f, int dry) {
	wchar_t* n=f->cFileName;
	wchar_t* dot=n;
	while(*dot) dot++;
	while(dot>n&&*dot!=L'.') dot--;
	if(*dot!=L'.') dot=NULL;

	wchar_t base[260];
	if(dot&&!(f->dwFileAttributes&0x00000010)) {
		size_t len=(size_t)(dot-n);
		for(size_t i=0; i<len; i++) base[i]=n[i];
		base[len]=0;
	}
	else wccopy(base, n);
	char cleaned[260];
	cleanNameW(base, cleaned, sizeof(cleaned));
	int isdir=(f->dwFileAttributes&0x00000010);
	if(cleaned[0]==0) { gSkipped++; return; }
	wchar_t newNameW[260];
	if(dot&&!isdir) {
		wchar_t cW[260]; toWideChar(cleaned, cW, 260);
		wccopy(newNameW, cW); wcCat(newNameW, dot);
	}
	else toWideChar(cleaned, newNameW, 260);
	if(wcicmp(newNameW, n)!=0) {
		size_t dlen=mwcslen(d);
		size_t nlen=mwcslen(n);
		size_t nnwlen=mwcslen(newNameW);
		size_t maxlen=(dlen+1+(nlen>nnwlen ? nlen : nnwlen)+1);
		wchar_t* o=(wchar_t*)pHeapAlloc(pGetProcessHeap(), 0, maxlen*sizeof(wchar_t));
		wchar_t* nn=(wchar_t*)pHeapAlloc(pGetProcessHeap(), 0, maxlen*sizeof(wchar_t));

		if(!o||!nn) {
			ansiRed(); printLine("", "Fatal: Out of memory for path construction in cleanAndRenameW");
			if(o) pHeapFree(pGetProcessHeap(), 0, o);
			if(nn) pHeapFree(pGetProcessHeap(), 0, nn);
			gFailed++;
			return;
		}

		wccopy(o, d); wcCat(o, L"\\"); wcCat(o, n);
		wccopy(nn, d); wcCat(nn, L"\\"); wcCat(nn, newNameW);
		char oa[512], na[512];
		wideToUtf8(o, oa, 512); wideToUtf8(nn, na, 512);
		const char* of=oa; const char* nf=na; const char* ls=na;
		while(*ls) { if(*ls=='\\'||*ls=='/') nf=ls+1; ls++; }
		const char* type=isdir ? "[DIREC] " : "[FILES] ";
		if(dry) {
			gDryRun++; ansiYellow(); print(type); print(of);
			print(" -> "); print(nf); print(" [DRY]"); ansiReset(); print("\n");
			pHeapFree(pGetProcessHeap(), 0, o); pHeapFree(pGetProcessHeap(), 0, nn);
			return;
		}
		if(renameExW(o, nn)) {
			gRenamed++;
			if(isdir) ansiCyan(); else ansiGreen();
			print(type); print(of); print(" -> "); print(nf); print(" [RENAMED]");
			ansiReset(); print("\n");
			wccopy(n, newNameW);
		}
		else {
			gFailed++; ansiRed();
			print(type); print(of); print(" -> "); print(nf); print(" [FAILED]");
			ansiReset(); print("\n");
		}

		pHeapFree(pGetProcessHeap(), 0, o); pHeapFree(pGetProcessHeap(), 0, nn);
	}
	else gSkipped++;
}
void processDirsOnlyW(const wchar_t* d, int dry, const wchar_t* root) {
	if(endsWithNoGallery(d)) {
		return;
	}
	wchar_t s[260]; wccopy(s, d); wcCat(s, L"\\*");
	WIN32_FIND_DATAW f;
	HANDLE h=pFindFirstFileW(s, &f);
	if(h==INVALID_HANDLE_VALUE) {
		return;
	}
	int dir_cap=16, dir_cnt=0;
	WIN32_FIND_DATAW* dirs=(WIN32_FIND_DATAW*)pHeapAlloc(pGetProcessHeap(), 0, dir_cap*sizeof(WIN32_FIND_DATAW));
	if(!dirs) { pFindClose(h); return; }
	do {
		if(wcCmp(f.cFileName, L".")==0||wcCmp(f.cFileName, L"..")==0) continue;
		if(f.dwFileAttributes&0x00000010) {
			if(dir_cnt>=dir_cap) {
				int nc=dir_cap*2;
				WIN32_FIND_DATAW* tmp=(WIN32_FIND_DATAW*)pHeapReAlloc(pGetProcessHeap(), 0, dirs, nc*sizeof(WIN32_FIND_DATAW));
				if(!tmp) { pHeapFree(pGetProcessHeap(), 0, dirs); pFindClose(h); return; }
				dirs=tmp; dir_cap=nc;
			}
			memcpy(&dirs[dir_cnt++], &f, sizeof(WIN32_FIND_DATAW));
		}
	}
	while(pFindNextFileW(h, &f));
	pFindClose(h);
	for(int i=0; i<dir_cnt; i++) {
		size_t dlen=mwcslen(d);
		size_t nlen=mwcslen(dirs[i].cFileName);
		size_t path_size=(dlen+1+nlen+1)*sizeof(wchar_t);
		wchar_t* fullPath=(wchar_t*)pHeapAlloc(pGetProcessHeap(), 0, path_size);
		if(!fullPath) { continue; }
		wccopy(fullPath, d);
		wcCat(fullPath, L"\\");
		wcCat(fullPath, dirs[i].cFileName);
		processDirsOnlyW(fullPath, dry, root);
		pHeapFree(pGetProcessHeap(), 0, fullPath);
	}
	for(int i=0; i<dir_cnt; i++) {
		cleanAndRenameW(d, &dirs[i], dry);
	}

	pHeapFree(pGetProcessHeap(), 0, dirs);
}
void processDirW(const wchar_t* d, int dry, const wchar_t* root) {
	if(endsWithNoGallery(d)) {
		if(!isInitialRoot(d, root)) {
			char m[340], b[260];
			wideToUtf8(d, b, 260);
			mstrcpyA(m, "[DIR] Skipped ("); mstrcatA(m, b); mstrcatA(m, "/.nogallery) and its contents.");
			ansiBlue(); printLine("", m);
		}
		return;
	}
	wchar_t s[260]; wccopy(s, d); wcCat(s, L"\\*");
	WIN32_FIND_DATAW f;
	HANDLE h=pFindFirstFileW(s, &f);
	if(h==INVALID_HANDLE_VALUE) {
		if(isInitialRoot(d, root)) {
			char m[300], b[260];
			wideToUtf8(d, b, 260);
			mstrcpyA(m, "[DIR] Warning: Cannot open or empty directory "); mstrcatA(m, b);
			ansiBlue(); printLine("", m);
		}
		return;
	}
	int dir_cap=16, dir_cnt=0;
	WIN32_FIND_DATAW* dirs=(WIN32_FIND_DATAW*)pHeapAlloc(pGetProcessHeap(), 0, dir_cap*sizeof(WIN32_FIND_DATAW));
	if(!dirs) { ansiRed(); printLine("", "Fatal: Out of memory allocating dir list"); pFindClose(h); return; }
	int file_cap=16, file_cnt=0;
	WIN32_FIND_DATAW* files=(WIN32_FIND_DATAW*)pHeapAlloc(pGetProcessHeap(), 0, file_cap*sizeof(WIN32_FIND_DATAW));
	if(!files) { ansiRed(); printLine("", "Fatal: Out of memory allocating file list"); pHeapFree(pGetProcessHeap(), 0, dirs); pFindClose(h); return; }

	do {
		if(wcCmp(f.cFileName, L".")==0||wcCmp(f.cFileName, L"..")==0) continue;

		if(f.dwFileAttributes&0x00000010) {
			if(dir_cnt>=dir_cap) {
				int nc=dir_cap*2;
				WIN32_FIND_DATAW* tmp=(WIN32_FIND_DATAW*)pHeapReAlloc(pGetProcessHeap(), 0, dirs, nc*sizeof(WIN32_FIND_DATAW));
				if(!tmp) {
					ansiRed(); printLine("", "Fatal: Out of memory expanding dir list");
					pHeapFree(pGetProcessHeap(), 0, files); pHeapFree(pGetProcessHeap(), 0, dirs); pFindClose(h); return;
				}
				dirs=tmp; dir_cap=nc;
			}
			memcpy(&dirs[dir_cnt++], &f, sizeof(WIN32_FIND_DATAW));
		}
		else if(hasValidExtensionW(f.cFileName)) {
			if(file_cnt>=file_cap) {
				int nc=file_cap*2;
				WIN32_FIND_DATAW* tmp=(WIN32_FIND_DATAW*)pHeapReAlloc(pGetProcessHeap(), 0, files, nc*sizeof(WIN32_FIND_DATAW));
				if(!tmp) {
					ansiRed(); printLine("", "Fatal: Out of memory expanding file list");
					pHeapFree(pGetProcessHeap(), 0, files); pHeapFree(pGetProcessHeap(), 0, dirs); pFindClose(h); return;
				}
				files=tmp; file_cap=nc;
			}
			memcpy(&files[file_cnt++], &f, sizeof(WIN32_FIND_DATAW));
		}
	}
	while(pFindNextFileW(h, &f));
	pFindClose(h);
	for(int i=0; i<file_cnt; i++) {
		cleanAndRenameW(d, &files[i], dry);
	}
	pHeapFree(pGetProcessHeap(), 0, files);
	for(int i=0; i<dir_cnt; i++) {
		size_t dlen=mwcslen(d);
		size_t nlen=mwcslen(dirs[i].cFileName);
		size_t path_size=(dlen+1+nlen+1)*sizeof(wchar_t);
		wchar_t* fullPath=(wchar_t*)pHeapAlloc(pGetProcessHeap(), 0, path_size);
		if(!fullPath) {
			ansiRed(); printLine("", "Fatal: Out of memory for recursion path");
			continue;
		}

		wccopy(fullPath, d);
		wcCat(fullPath, L"\\");
		wcCat(fullPath, dirs[i].cFileName);
		processDirW(fullPath, dry, root);
		pHeapFree(pGetProcessHeap(), 0, fullPath);
	}
	pHeapFree(pGetProcessHeap(), 0, dirs);
}
void printSummary() {
	char b[512], t[64];
	mstrcpyA(b, "\n=== Summary ===\n");
	intToStr_s(gRenamed, t); mstrcatA(b, "Renamed: "); mstrcatA(b, t); mstrcatA(b, "\n");
	intToStr_s(gDryRun, t); mstrcatA(b, "Dry-run: "); mstrcatA(b, t); mstrcatA(b, "\n");
	intToStr_s(gSkipped, t); mstrcatA(b, "Skipped: "); mstrcatA(b, t); mstrcatA(b, "\n");
	intToStr_s(gFailed, t); mstrcatA(b, "Failed: "); mstrcatA(b, t); mstrcatA(b, "\n");
	printLine("", b);
}

void enable_ansi() {
	g_stdout_handle=pGetStdHandle(((DWORD)-11));
	if(g_stdout_handle==((DWORD)-11)) return;
	DWORD mode=0;
	if(!pGetConsoleMode(g_stdout_handle, &mode)) return;
	pSetConsoleMode(g_stdout_handle, mode|0x0004);
}

void wWinMainCRTStartup() {
	resolveAll();
	enable_ansi();
	g_stdout_handle=pGetStdHandle(((DWORD)-11));
	srand32((uint32_t)pGetTickCount64());
	wchar_t root[260]; wccopy(root, L".");
	int dry=0; int argc;
	wchar_t** argv=pCommandLineToArgvW(pGetCommandLineW(), &argc);
	if(argv) {
		for(int i=1;i<argc;i++) {
			if(wcicmp(argv[i], L"--dry-run")==0) dry=1;
			else wccopy(root, argv[i]);
		}
		pLocalFree((HGLOBAL)argv);
	}
	processDirW(root, dry, root);
	processDirsOnlyW(root, dry, root);

	printSummary();
	pExitProcess(0);
}
