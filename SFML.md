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

That's it! The dll files should already be in the correct directory, and all the rest of the links and settings should work fine.