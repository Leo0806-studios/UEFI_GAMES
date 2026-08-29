# NEXT


## What
my personal OS dev project that will eventualy contain everything from bootloader, CRT/C++RT, kernel, drivers, and userland.
additionaly it contains a few proof of concept projects build puerly on uefi


## descriptions

### AP_STARTUP
small project containing only application bring up trampolines

### BOOTLOADER
self written bootloader that initializes a pre kernel enviroment for uefi applications to build on.
it also performs hardware discovery and prepares a standardiezed envorment and and data interface for the kernel to startup in.
### CPP_RUNTIME_STATIC_PART 
so far existing c++ runtime code that is old and in dire need of a rewite. for now supports global constructors, dynamic aallocation and exception throwing (but not unwinding or catching)

### CRT
msvc compatible implementation of the C runtime library and related functionality. mostly os independent. goal is to be msvc source compatible.
only supports x64 and the newest C standarts.

### efilib 
gnuefi git submodule

### FREESTANDING_STD_LIB_TESTS
old and in need of a rewite project containing tests for my c++ std port

### FREESTANDING_STD_LIB
old and incomplete partial port of the c++ standard library.

### KERNEL
my personal fully c++ hobby kernel project.

### LINKER
my originl idea for a custom binary format.
obsolete and got moved to my custom language project USL.

### LOW_LEVEL_LIB
contains asm stubs for variouse tasks that cant be performed in c++

### PONG
uefi application implementing pong ontop of my UEFI_ENGINE and the bootloader

### POST_BUILD_TOOL
obsolete. will be removed soon ish

### UEFI_ENGINE
UEFI based game eingine.

### USB_STACK
old project intended for a usb stack. wil be moved to the kernel once i reach that point. currently empty

### VM_CORE
hypervisor project for my kernel to eventualy run in for increased runtime stability and secureit. currently empty



## building

MSBUILD in visual studio is the only supported build system.

to build the CRT you must pass __CRT_BUILD as a preprocessor definition to the compiler