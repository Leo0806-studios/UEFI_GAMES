// LINKER.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/*
usage
first arg: mode
        -D driver. kernel level. produces .SL (sharedLibrary)
        -K kernel. kernel level produces .KRN (kernel)
        -KS kernel level shared lib. produces .SL
        -KEXEC kernel level executable. produces .KOBJ (kernelObject)
        -S Shared lib. Userspace Shared library. produces .SL
        -EXEC executable. userspace executable. produces .OBJ (Object)

second arg : input PE file.
optional third arg: -BA
    instead of outputting a binary it will create a .txt file that contains a C-Style unsigned char array which contains the binary data as hex.
    additionally the file contains a pointer called <filename>_ptr and is of the type const unsigned char*. a size_t <filename>_size contains the size.
    both the pointer and the size are only valid after the included function <filename>_SETUP() is called
    the inclusion of the size_t and the pointer is so you can define a extern var to put in a header
    this file can then be just #include ed inside any c or c++ file to use. the char array signature is unsigned char <filename>[count]; 

third arg: output file name. will be created in the same dir as the input file.


about .KOBJ.
it will only run in ring0 if started by the kernel itself and with special parameters.
starting such an application from userspace will result in process termination.

about .KRN. 
this is the format my 3rd stage of the Kernel will be in.
it is the first stage not stored in a PE executable.
it is the main kernel and loads all of the drivers (except the most basic ones)




*/
import <string>;
import <iostream>;
import PE;
#ifdef __INTELLISENSE__
#include "MXF/MXF.h"
#else
import MXF;

#endif
int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;
    std::ios::sync_with_stdio(false);
  // std::string path = "C:\\Users\\leo08\\source\\repos\\UEFI_GAMES\\x64\\Debug\\KERNEL.exe";//for now ill hardcode a path to a test file.
    std::string path = "C:\\Users\\leo08\\source\\repos\\UEFI_GAMES\\x64\\Debug\\LINKER.exe";//for now ill hardcode a path to a test file.
    if (argc < 3 && path == "") {
        std::cout << "[MAIN] to few args. restart with the correct args\n";
        
        return -1;
    }
	MXF_LINKER::PE pe(path);
    pe.Parse();
    MXF_LINKER::MXF mxf(pe);
    mxf.Build();
    std::filesystem::path outpath = path;
    outpath = outpath.parent_path();
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
