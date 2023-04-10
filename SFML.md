# SFML

## Setting up SFML

1. Goto https://www.sfml-dev.org/download/sfml/2.5.1/
2. Download the Visual C++ 15 (2017) - 32-bit option
3. Unzip it to wherever is convenient (I chose d:\\source\\libraries\\SFML-2.5.1)
4. In the VS project, open Project>SDEV230_Project Properties
5. Set the Configuration to "All Configurations"
	+ ***NOTE*** When performing the following steps, do not remove any directories that have been added by others who may have the library stored in a different directory
6. Under Configuration Properties>C/C++, add the include directory to Additional Include Directories (e.g. d:\\source\\libraries\\SFML-2.5.1\\include)
7. Under Linker>General, add the library directory to Additional Library Directories (e.g. d:\\source\\libraries\\SFML-2.5.1\\lib)
8. Locate the .dll files in the library (e.g. d:\\source\\libraries\\SFML-2.5.1\\bin)
9. Copy the following .dll files to the Release folder in the project (e.g. D:\\source\\repos\\SDEV230_Project\\Release)
	+ If the release folder does not exist yet, create it
	+ sfml-graphics-2.dll
	+ sfml-window-2.dll
	+ sfml-system-2.dll
9. Copy the following .dll files to the Debug folder in the project (e.g. D:\\source\\repos\\SDEV230_Project\\Debug)
	+ If the debug folder does not exist yet, create it
	+ sfml-graphics-2-d.dll
	+ sfml-window-2-d.dll
	+ sfml-system-2-d.dll
10. *One more thing:* When you compile, ensure your solution platform is set to x86, not x64.
	+ If you specifically want x64 compatibility, that can be arranged, it will just be a lot of extra steps.

That should be everything! If you have issues getting it to compile and run, contact UnusualParticle.