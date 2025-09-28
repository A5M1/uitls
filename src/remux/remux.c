#define UNICODE
#define _UNICODE
#include <windows.h>
#include <emmintrin.h>
//gcc -o program.exe "remux" -nostdlib -Wl,-e,mainCRTStartup -O3 -s -lkernel32 -mavx
typedef unsigned char  uint8_t;
typedef signed   char  int8_t;
typedef unsigned short uint16_t;
typedef signed   short int16_t;
typedef unsigned int   uint32_t;
typedef signed   int   int32_t;
typedef unsigned long long uint64_t;
typedef signed   long long int64_t;
static int slen(const char* s) {
	const char* p=s;
	__m128i zero=_mm_setzero_si128();
	while(1) {
		__m128i chunk=_mm_loadu_si128((__m128i*)p);
		int mask=_mm_movemask_epi8(_mm_cmpeq_epi8(chunk, zero));
		if(mask!=0) {
			return (int)(p-s)+__builtin_ctz(mask);
		}
		p+=16;
	}
}
static void scpy(char* dst, const char* src, size_t max) {
	size_t i=0;
	while(i+16<=max-1) {
		__m128i chunk=_mm_loadu_si128((__m128i*)(src+i));
		int mask=_mm_movemask_epi8(_mm_cmpeq_epi8(chunk, _mm_setzero_si128()));
		if(mask!=0) {
			int len=__builtin_ctz(mask);
			for(int j=0; j<len; j++) dst[i+j]=src[i+j];
			dst[i+len]=0;
			return;
		}
		_mm_storeu_si128((__m128i*)(dst+i), chunk);
		i+=16;
	}
	while(i<max-1&&src[i]) {
		dst[i]=src[i];
		i++;
	}
	dst[i]=0;
}
static void msct(char* dst, const char* src, size_t max) {
	char* p=dst;
	__m128i zero=_mm_setzero_si128();
	while(1) {
		if((size_t)(p-dst)>=max) return;
		__m128i chunk=_mm_loadu_si128((__m128i*)p);
		int mask=_mm_movemask_epi8(_mm_cmpeq_epi8(chunk, zero));
		if(mask!=0) {
			int len=__builtin_ctz(mask);
			if((size_t)(p-dst+len)>=max-1) len=(int)(max-1-(p-dst));
			p+=len;
			break;
		}
		p+=16;
	}
	size_t remaining=max-(p-dst)-1;
	size_t i=0;
	while(i+16<=remaining) {
		__m128i chunk=_mm_loadu_si128((__m128i*)(src+i));
		int mask=_mm_movemask_epi8(_mm_cmpeq_epi8(chunk, zero));
		if(mask!=0) {
			int len=__builtin_ctz(mask);
			_mm_storeu_si128((__m128i*)(p+i), _mm_loadu_si128((__m128i*)(src+i)));
			i+=len;
			p[i]=0;
			return;
		}
		_mm_storeu_si128((__m128i*)(p+i), chunk);
		i+=16;
	}
	while(i<remaining&&src[i]) {
		p[i]=src[i];
		i++;
	}
	p[i]=0;
}
static char* itoa10(int64_t v, char* buf) {
	char tmp[20];
	char* p=buf;
	uint64_t u;
	if(v<0) {
		*p++='-';
		u=(uint64_t)(-(v+1))+1;
	}
	else {
		u=(uint64_t)v;
	}
	if(u<10) {
		*p++='0'+(char)u;
		*p=0;
		return buf;
	}
	int i=0;
	__asm__ volatile (
		"mov %[u], %%rax\n"
		"xor %%rdx, %%rdx\n"
		"1:\n"
		"mov $10, %%rcx\n"
		"xor %%rdx, %%rdx\n"
		"div %%rcx\n"
		"add $'0', %%dl\n"
		"mov %%dl, %[tmp_i]\n"
		"inc %[i]\n"
		"test %%rax, %%rax\n"
		"jnz 1b\n"
		: [tmp_i] "=m" (tmp[i]), [i] "+r" (i)
		: [u] "r" (u)
		: "rax", "rdx", "rcx", "memory"
		);
	while(i-->0) {
		*p++=tmp[i];
	}
	*p=0;
	return buf;
}
static void logStr(const char* s) {
	DWORD written;
	WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), s, slen(s), &written, NULL);
}
static void logLine(const char* s) {
	logStr(s);
	logStr("\r\n");
}
static void logInt(int v) {
	char buf[32];
	itoa10(v, buf);
	logStr(buf);
}
static int fileExists(const char* filename) {
	DWORD attr=GetFileAttributesA(filename);
	return (attr!=INVALID_FILE_ATTRIBUTES&&!(attr&FILE_ATTRIBUTE_DIRECTORY));
}
static void getUniqueName(char* out, size_t outSize, const char* base) {
	int n=0;
	char numbuf[32];
	size_t baseLen=0;
	while(base[baseLen]&&baseLen<outSize-10) baseLen++;
	for(size_t i=0; i<baseLen; i++) out[i]=base[i];
	out[baseLen]=0;
	while(fileExists(out)) {
		n++;
		for(size_t i=0; i<baseLen; i++) out[i]=base[i];
		out[baseLen]=0;
		msct(out, "-", outSize);
		msct(out, itoa10(n, numbuf), outSize);
		msct(out, ".mp4", outSize);
	}
	if(n==0) msct(out, ".mp4", outSize);
}
static void runFFmpeg(const char* infile, const char* outfile) {
	char cmd[1024];
	scpy(cmd, "ffmpeg -y -hide_banner -loglevel warning -i \"", sizeof(cmd));
	msct(cmd, infile, sizeof(cmd));
	msct(cmd, "\" -c:v h264_nvenc -preset p1 -tune ll -cq 23 -c:a aac -b:a 128k \"", sizeof(cmd));
	msct(cmd, outfile, sizeof(cmd));
	msct(cmd, "\"", sizeof(cmd));
	logStr("Running: ");
	logLine(cmd);
	STARTUPINFOA si; PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb=sizeof(si);
	if(CreateProcessA(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	else {
		logLine("FFmpeg launch failed!");
	}
}
void* memcpy(void* dst, const void* src, size_t n) {
	char* d=dst;
	const char* s=src;
	while(n--) {
		*d++=*s++;
	}
	return dst;
}
static void moveFile(const char* file) {
	char dest[MAX_PATH];
	scpy(dest, "webm\\", sizeof(dest));
	size_t dest_len=slen(dest);
	size_t file_len=slen(file);
	if(dest_len+file_len<MAX_PATH) {
		for(size_t i=0; i<file_len; i++) {
			dest[dest_len+i]=file[i];
		}
		dest[dest_len+file_len]=0;
		if(MoveFileA(file, dest)) {
			logStr("Moved ");
			logStr(file);
			logStr(" -> ");
			logLine(dest);
		}
		else {
			logStr("Move failed for ");
			logStr(file);
			logStr(" to ");
			logStr(dest);
			logLine("");
		}
	}
	else {
		logStr("Move failed: Path too long for ");
		logLine(file);
	}
}
static void createNoGallery(void) {
	HANDLE h=CreateFileA(
		"webm\\.nogallery",
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if(h!=INVALID_HANDLE_VALUE) {
		CloseHandle(h);
		logLine("Created webm\\.nogallery");
	}
}
void* memset(void* dst, int c, size_t n) {
	unsigned char* p=dst;
	while(n--) *p++=(unsigned char)c;
	return dst;
}
void mainCRTStartup(void) {
	WIN32_FIND_DATAA fd;
	HANDLE h=FindFirstFileA("*.webm", &fd);
	if(h==INVALID_HANDLE_VALUE) {
		logLine("No .webm files found.");
		ExitProcess(0);
	}
	CreateDirectoryA("webm", NULL);
	createNoGallery();
	int count=0;
	do {
		if(!fd.cFileName||!fd.cFileName[0]) continue;
		char base[512], out[512];
		scpy(base, fd.cFileName, sizeof(base));
		char* dot=base;
		while(*dot) dot++;
		while(dot>base&&*dot!='.') dot--;
		if(*dot=='.') *dot=0;
		getUniqueName(out, sizeof(out), base);
		logStr("Converting #");
		logInt(++count);
		logStr(": ");
		logStr(fd.cFileName);
		logStr(" -> ");
		logLine(out);
		runFFmpeg(fd.cFileName, out);
		moveFile(fd.cFileName);
	}
	while(FindNextFileA(h, &fd));
	FindClose(h);
	logStr("Done. Converted ");
	logInt(count);
	logLine(" file(s).");
	ExitProcess(0);
}
