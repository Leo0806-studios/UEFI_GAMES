// LINKER.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/*
ussage
first arg: mode
        -D driver. kernel level. produces .SL (sharedLibrary)
        -K kernel. kernel level produces .KRN (kernel)
        -KS kernel level shared lib. produces .SL
        -KEXEC kernel level executable. producces .KOBJ (kernelObject)
        -S Shared lib. Userspace Shared library. produces .SL
        -EXEC executable. userspace executable. produces .OBJ (Object)

second arg : input PE file.

third arg: output file name. will be created in the same dir as the input file.


about .KOBJ.
it will only run in ring0 if started by the kernel itself and with special parameters.
starting such an appliction from userspace will result in process termination.

about .KRN. 
this is the format my 3rd stage of the kenel will be in.
it is the first stage not stored in a pe executable.
it is the main kernel and loads all of the drivers (except the most basic ones)




*/
import <string>;
import PE;
#ifdef __INTELLISENSE__
#include "MXF/MXF.h"
#else
import MXF;

#endif
int main()
{
   std::string path = "C:\\Users\\leo08\\source\\repos\\UEFI_GAMES\\x64\\Debug\\KERNEL.exe";//for now ill hardcode a path to a test file.
   // std::string path = "C:\\Users\\leo08\\source\\repos\\UEFI_GAMES\\x64\\Debug\\LINKER.exe";//for now ill hardcode a path to a test file.
	MXF_LINKER::PE pe(path);
    pe.Parse();
    MXF_LINKER::MXF mxf(pe);
    mxf.Build();
    mxf.Write("outPath.MXF");


}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
