#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  int mmx;
  int sse;
  int sse2;
  int sse3;
  int ssse3;
  int sse41;
  int sse42;
  int avx;
  int avx2;
  int aes;
  int htt;
  int mmxx;
  int amd3D;
  int amd3x;
} X64ST;
typedef struct {
  int cores;
  int logical_processors;
  int l2_cache_kb;
  int l3_cache_kb;
} CPUInfo;

void cpuid(int code, unsigned int* a, unsigned int* b, unsigned int* c, unsigned int* d) {
    __asm__ __volatile__(
        "cpuid"
        : "=a" (*a), "=b" (*b), "=c" (*c), "=d" (*d)
        : "a" (code), "c" (0)
    );
}

void get_vendor_string(char *buffer) {
  unsigned int a, b, c, d;
  cpuid(0, &a, &b, &c, &d);
  memcpy(buffer, &b, 4);
  memcpy(buffer + 4, &d, 4);
  memcpy(buffer + 8, &c, 4);
  buffer[12] = '\0';
}

void get_cpu_brand(char *buffer) {
  unsigned int regs[12];
  cpuid(0x80000002, &regs[0], &regs[1], &regs[2], &regs[3]);
  cpuid(0x80000003, &regs[4], &regs[5], &regs[6], &regs[7]);
  cpuid(0x80000004, &regs[8], &regs[9], &regs[10], &regs[11]);
  memcpy(buffer, regs, sizeof(regs));
  buffer[48] = '\0';
}

uint64_t xgetbv() {
  unsigned int eax, edx;
  __asm__ __volatile__("xgetbv" : "=a"(eax), "=d"(edx) : "c"(0));
  return ((uint64_t)edx << 32) | eax;
}

void check_instruction_sets(X64ST *x64) {
  unsigned int a, b, c, d;
  cpuid(1, &a, &b, &c, &d);
  x64->mmx = (d >> 23) & 1;
  x64->sse = (d >> 25) & 1;
  x64->sse2 = (d >> 26) & 1;
  x64->sse3 = (c >> 0) & 1;
  x64->ssse3 = (c >> 9) & 1;
  x64->sse41 = (c >> 19) & 1;
  x64->sse42 = (c >> 20) & 1;
  x64->aes = (c >> 25) & 1;
  int osxsave = (c >> 27) & 1;
  int avx_support = (c >> 28) & 1;
  if (avx_support && osxsave) {
    uint64_t xcr0 = xgetbv();
    if ((xcr0 & 0x6) == 0x6)
      x64->avx = 1;
  }
  cpuid(7, &a, &b, &c, &d);
  x64->avx2 = (b >> 5) & 1;
  x64->htt = (d >> 28) & 1;
  cpuid(0x80000001, &a, &b, &c, &d);
  x64->mmxx = (d >> 22) & 1;
  x64->amd3D = (d >> 31) & 1;
  x64->amd3x = (d >> 30) & 1;
}

void display_instruction_sets(const X64ST *x64, char *buffer) {
  if (x64->mmx)
    strcat(buffer, "mmx, ");
  if (x64->sse)
    strcat(buffer, "sse, ");
  if (x64->sse2)
    strcat(buffer, "sse2, ");
  if (x64->sse3)
    strcat(buffer, "sse3, ");
  if (x64->ssse3)
    strcat(buffer, "ssse3, ");
  if (x64->sse41)
    strcat(buffer, "sse4.1, ");
  if (x64->sse42)
    strcat(buffer, "sse4.2, ");
  if (x64->avx)
    strcat(buffer, "avx, ");
  if (x64->avx2)
    strcat(buffer, "avx2, ");
  if (x64->aes)
    strcat(buffer, "aes, ");
  if (x64->htt)
    strcat(buffer, "htt, ");
  if (x64->mmxx)
    strcat(buffer, "mmx Extended, ");
  if (x64->amd3D)
    strcat(buffer, "3Dnow, ");
  if (x64->amd3x)
    strcat(buffer, "3Dnow Extended, ");
}
void get_cpu_codename(char *codename) {
  unsigned int a, b, c, d;
  cpuid(1, &a, &b, &c, &d);
  int family = ((a >> 8) & 0xF) + ((a >> 20) & 0xFF);
  int model = ((a >> 4) & 0xF) + ((a >> 12) & 0xF0);
  int stepping = a & 0xF;
  sprintf(codename, "Family: %d, Model: %d, Stepping: %d", family, model,
          stepping);
}
void get_cpu_info(CPUInfo *info) {
  unsigned int a, b, c, d;
  cpuid(1, &a, &b, &c, &d);
  int logical_processors = (b >> 16) & 0xFF;
  info->logical_processors = logical_processors;
  int cores_per_package = 0;
  for (int i = 0;; i++) {
    cpuid(4, &a, &b, &c, &d);
    int cache_type = a & 0x1F;
    if (cache_type == 0)
      break;
    if (i == 0)
      cores_per_package = ((a >> 26) & 0x3F) + 1;
  }
  info->cores = cores_per_package;
  cpuid(4, &a, &b, &c, &d);
  if ((a & 0x1F) == 2) {
    int cache_size = ((b >> 22) + 1) * ((b & 0xFFF) + 1) *
                     (((b >> 12) & 0x3FF) + 1) * (d + 1);
    info->l2_cache_kb = cache_size / 1024;
  }
  cpuid(4, &a, &b, &c, &d);
  if ((a & 0x1F) == 3) {
    int cache_size = ((b >> 22) + 1) * ((b & 0xFFF) + 1) *
                     (((b >> 12) & 0x3FF) + 1) * (d + 1);
    info->l3_cache_kb = cache_size / 1024;
  }
}
int main() {
  char vendor[13];
  char brand[49];
  char display_buffer[256] = {0};
  X64ST x64 = {0};
  get_vendor_string(vendor);
  printf("CPU: %s\n", vendor);
  get_cpu_brand(brand);
  printf("BRAND: %s\n", brand);
  check_instruction_sets(&x64);
  printf("Available Instruction Sets:\n");
  printf("1 = supported, 0 = not supported\n");
  display_instruction_sets(&x64, display_buffer);
  printf("Instructions: %s\n", display_buffer);
  printf("  %d mmx\n", x64.mmx);
  printf("  %d sse\n", x64.sse);
  printf("  %d sse2\n", x64.sse2);
  printf("  %d sse3\n", x64.sse3);
  printf("  %d ssse3\n", x64.ssse3);
  printf("  %d sse4.1\n", x64.sse41);
  printf("  %d sse4.2\n", x64.sse42);
  printf("  %d avx\n", x64.avx);
  printf("  %d avx2\n", x64.avx2);
  printf("  %d aes\n", x64.aes);
  printf("  %d htt\n", x64.htt);
  printf("AMD:\n");
  printf("  %d mmx Extended\n", x64.mmxx);
  printf("  %d 3Dnow\n", x64.amd3D);
  printf("  %d 3Dnow Extended\n\n", x64.amd3x);
  CPUInfo info = {0};
  char codename[128] = {0};

  get_cpu_info(&info);
  get_cpu_codename(codename);

  printf("CPU Info:\n");
  printf("Cores: %d\n", info.cores);
  printf("Logical Processors: %d\n", info.logical_processors);
  printf("L2 Cache: %d KB\n", info.l2_cache_kb);
  printf("L3 Cache: %d KB\n", info.l3_cache_kb);
  printf("Codename: %s\n", codename);
  return 0;
}
