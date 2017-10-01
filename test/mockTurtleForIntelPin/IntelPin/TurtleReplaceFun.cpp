/*BEGIN_LEGAL 
Intel Open Source License 

Copyright (c) 2002-2016 Intel Corporation. All rights reserved.
 
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.  Redistributions
in binary form must reproduce the above copyright notice, this list of
conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.  Neither the name of
the Intel Corporation nor the names of its contributors may be used to
endorse or promote products derived from this software without
specific prior written permission.
 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE INTEL OR
ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
END_LEGAL */
//  Replace an original function with a custom function defined in the tool using
//  probes.  The replacement function has a different signature from that of the 
//  original replaced function.

#include "pin.H"
#include <iostream>

// Pin calls this function every time a new img is loaded.
// It is best to do probe replacement when the image is loaded,
// because only one thread knows about the image at this time.
//
VOID ImageLoad( IMG img, VOID *v )
{
    std::cout << "Loading " << IMG_Name(img) << ", Image id = " << IMG_Id(img) << std::endl;

    // Print all functions in the img
    /*
    for( SEC sec= IMG_SecHead(img); SEC_Valid(sec); sec = SEC_Next(sec) ) {
        for( RTN rtn= SEC_RtnHead(sec); RTN_Valid(rtn); rtn = RTN_Next(rtn) ) {
            std::cout << RTN_Name(rtn) << std::endl;
        }
    }*/

    // Replace Turtle::PenDown() with proxy::PenDown(Turtle*)
    RTN rtn = RTN_FindByName(img, "_ZN6Turtle7PenDownEv");
    if (RTN_Valid(rtn)) {
        std::cout << "Valid RTN for Turtle::PenDown()" << std::endl;
        RTN_ReplaceProbed(rtn, RTN_Funptr(RTN_FindByName(img,
            "_ZN5proxy7PenDownEP6Turtle")));
    }
}

/* ===================================================================== */
/* Main: Initialize and start Pin in Probe mode.                         */
/* ===================================================================== */
int main( INT32 argc, CHAR *argv[] )
{
    // Initialize symbol processing
    PIN_InitSymbols();
    
    // Initialize pin
    if (PIN_Init(argc, argv)) return -1;

    // Register ImageLoad to be called when an image is loaded
    IMG_AddInstrumentFunction( ImageLoad, 0 );
    
    // Start the program in probe mode, never returns
    PIN_StartProgramProbed();
    
    return 0;
}
