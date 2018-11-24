# Compiling Windows:

* Download premake5, put the executable in the `midi` directory
* Run `BuildProjects.bat`


## Visual studio 2010:

* Open `midi/windows-vs2010/gmcl_midi.sln` in Visual Studio 2010
* Build the solution
* Note: You cannot build a 64 bits target using Visual Studio 2010!


## Visual studio 2017:
- An error might occur about some target not being installed. Right click the
  solution (in Solution Explorer) and click "Retarget solution". In the dialog
  just click `Ok`. Tested with retargeting to Windows SDK version `10.0.17763.0`

  Specific error:

  ```1>C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\VC\VCTargets\Microsoft.Cpp.WindowsSDK.targets(46,5): error MSB8036: The Windows SDK version 8.1 was not found. Install the required version  Windows SDK or change the SDK version in the project property pages or by right-clicking the solution and selecting "Retarget solution".
  1>Done building project "gmcl_midi.vcxproj" -- FAILED.
  ```
- Note that the x64 and Win32 will compile to the same dll in `lib`. Building
  one target will override the output from the other.


## In ANY version of Visual Studio:

In project properties (of both Win32 AND x64 targets):
    - Add the following to `C/C++ -> Preprocessor -> preprocessor definitions`: `__WINDOWS_MM__`
    - Add the following to `Linker -> Input -> Additional dependencies`: `winmm.lib`

Not doing this will cause the module to compile just fine, but it won't find any midi devices!

For Linux and OSX you need to do similar things. See https://www.music.mcgill.ca/~gary/rtmidi/
