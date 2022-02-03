# How to Build
List the steps to build the binary and requirements from build environment

- [Software Requirements](#software-requirements)
- [Make targets](#make-targets)
- [Environment dependencies](#environment-dependencies)
- [Binary file date](#binary-file-date)


## Software Requirements
-   Visual Studio 2015 build tools at C:\Program Files (x86)\Microsoft Visual Studio 14.0
-	Python 3


## Make targets
The binary generation contains the date of the created binary. Therefore, the build date must be provided to the make target in order to get the same binary as the production one.

1) Build P-SEAM-Loader as described [here](../p-seam-loader/BUILD.md) with the production SEAM-Loader build date. The P-SEAM-Loader build instructions describe how to extract its build date.
2) Copy P-SEAM-Loader binaries - pseamldr.so and pseamldr.so.consts in the project's folder: Projects\Server\Spr\Seamldr
3) Open a command prompt in the above project's folder and the Build the project, using the build date in YYYYMMDD format: 

```
BLD REL_NT O1 [build-date]
```
	
The ACM binary will be built in Projects\Server\Spr\Seamldr\BUILD\PRJ\REL_NT\O1\_SEAMLDR_REL_NT_O1.PW.bin

4) Clean everything:
 
```
BLD REL_NT CLEANALL
```

## Environment dependencies



## Binary file date
The binary generation contains the date of the created binary.
 
In order to reproduce the exact binary, it is required to include the production SEAM-Loader build date as 
in the make target :  

```bash
BLD REL_NT O1 YYYYMMDD

