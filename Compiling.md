#Compiling Windows:

Add this to preprocessor definitions:
__WINDOWS_MM__

In linker, add this to input -> Additional dependencies:
winmm.lib
