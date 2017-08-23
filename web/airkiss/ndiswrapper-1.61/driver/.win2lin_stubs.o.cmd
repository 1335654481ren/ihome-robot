cmd_/home/renxl/share/github/web/airkiss/ndiswrapper-1.61/driver/win2lin_stubs.o := gcc -Wp,-MD,/home/renxl/share/github/web/airkiss/ndiswrapper-1.61/driver/.win2lin_stubs.o.d  -nostdinc -isystem /usr/lib/gcc/x86_64-linux-gnu/5/include  -I./arch/x86/include -Iarch/x86/include/generated/uapi -Iarch/x86/include/generated  -Iinclude -I./arch/x86/include/uapi -Iarch/x86/include/generated/uapi -I./include/uapi -Iinclude/generated/uapi -include ./include/linux/kconfig.h -Iubuntu/include  -D__KERNEL__  -fno-pie   -D__ASSEMBLY__ -fno-PIE  -fno-pie -m64 -DCONFIG_X86_X32_ABI -DCONFIG_AS_CFI=1 -DCONFIG_AS_CFI_SIGNAL_FRAME=1 -DCONFIG_AS_CFI_SECTIONS=1 -DCONFIG_AS_FXSAVEQ=1 -DCONFIG_AS_SSSE3=1 -DCONFIG_AS_CRC32=1 -DCONFIG_AS_AVX=1 -DCONFIG_AS_AVX2=1 -DCONFIG_AS_SHA1_NI=1 -DCONFIG_AS_SHA256_NI=1   -mfentry -DCC_USING_FENTRY -DCC_HAVE_ASM_GOTO          -DMODULE  -c -o /home/renxl/share/github/web/airkiss/ndiswrapper-1.61/driver/win2lin_stubs.o /home/renxl/share/github/web/airkiss/ndiswrapper-1.61/driver/win2lin_stubs.S

source_/home/renxl/share/github/web/airkiss/ndiswrapper-1.61/driver/win2lin_stubs.o := /home/renxl/share/github/web/airkiss/ndiswrapper-1.61/driver/win2lin_stubs.S

deps_/home/renxl/share/github/web/airkiss/ndiswrapper-1.61/driver/win2lin_stubs.o := \
    $(wildcard include/config/x86/64.h) \
  include/linux/linkage.h \
  include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/kasan.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
    $(wildcard include/config/kprobes.h) \
  include/linux/stringify.h \
  include/linux/export.h \
    $(wildcard include/config/have/underscore/symbol/prefix.h) \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
  arch/x86/include/asm/linkage.h \
    $(wildcard include/config/x86/32.h) \
    $(wildcard include/config/x86/alignment/16.h) \
  /home/renxl/share/github/web/airkiss/ndiswrapper-1.61/driver/win2lin_stubs.h \

/home/renxl/share/github/web/airkiss/ndiswrapper-1.61/driver/win2lin_stubs.o: $(deps_/home/renxl/share/github/web/airkiss/ndiswrapper-1.61/driver/win2lin_stubs.o)

$(deps_/home/renxl/share/github/web/airkiss/ndiswrapper-1.61/driver/win2lin_stubs.o):
