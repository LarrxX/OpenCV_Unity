%module OpenCVPlugin

%{
#include "OpenCVDllInterface.h"
%}

//Use System.IntPtr from C# for void* pointers
%typemap( cstype ) void* "System.IntPtr"
%typemap( csin ) void* %{ $csinput %}
%typemap( imtype ) void* "System.IntPtr"

%include "OpenCVDllInterface.h"

