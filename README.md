Cali - Compiler Assisted Library Isolation
==========================================

Without further precautions, software libraries can freely access the program's entire address space, and also inherit its system-level privileges.
This lack of separation regularly leads to security-critical incidents once libraries contain vulnerabilities or turn rogue.
We present *Cali*, a compiler-assisted library isolation system that *fully automatically* shields a program from a given library.
Cali is fully compatible with any mainline Linux kernel and does not require supervisor privileges to execute.
We compartmentalize libraries into their own process and kernel namespace context with well-defined security policies.
To preserve the functionality of the interactions between program and library, Cali uses a Program Dependency Graph to track data flow between the program and the library during link time.


Build
-----
- Install Clang/LLVM 7, including `lld`
- `mkdir build ; cd build` 
- `cmake -DCMAKE_BUILD_TYPE=Release ..`
- `make -j4`
- To install, add `build/cali-linker` to your `$PATH`

Tested on Ubuntu 18.04. Other distributions might need adjustments.


How to use
----------

1. Configure your application build using LLVM/Clang 7 with LTO enabled, and verify that it compiles:
   - Typical settings: `CC=clang`, `CXX=clang++`, `CFLAGS="-flto"`, `CXXFLAGS="-flto"`, `LDFLAGS="-flto"`
   - Read the manual of your build system to set these parameters. 
     For automake you can use `export A=B` before calling `./configure`. 
2. Write a yaml policy that configures your protection. See [cali-linker/sample_configs/config.all.yaml](cali-linker/sample_configs/config.all.yaml) 
   for possible options.
   The policy should specify all object files from your application, and possibly created static libraries.
   [cali-linker/sample_configs](cali-linker/sample_configs) contains examples.
3. Add Cali to your build: `LDFLAGS=-fuse-ld=cali --cali-config=your_policy.yaml`
4. Compile and test the final binary



License
-------
- [glibc-2.23](glibc-2.23), [libipc](libipc) and [precompiled-libraries](precompiled-libraries) are licensed with GNU LGPL v2.1 (code from the GNU project and the Cali authors)
- [libnsjail](libnsjail) is licensed with Apache License 2.0 (code by Google, modified by the Cali authors)
- [cali-linker](cali-linker) is licensed with GNU GPL v3



Credits
-------
- Cali contains code from the GNU libc project (malloc implementation in libipc). 
- Cali contains a library version of Google's [nsjail](https://github.com/google/nsjail) (in libnsjail).
- Our graph library is closely related to boost::graph 
