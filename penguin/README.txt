To sucessfully build this project, you must download some external sources.

[Step 1]:
  You must have a full checkout of the /penguin/ directory and the /libpurple/ directory.
  These are the only sources needed within pidgin that are required.


[Step 2]:
  Compile and run the Visual Studio solution located at:
    ..\libpurple\wrapper\generator\Scripts.sln

  This will generate the PurpleWrapper classes needed to make libpurple calls.


[Step 3]:
  Follow instructions in GLib/README.txt


[Step 4]:
  Copy the following folders and files into this directory from GLib/build:
    /builddeps/
    /scripts/
    /tools/
    /vsprops/
    msvc-pragmas.h
    OABuild.rules


[Step 5]:
  Obtain some win32 build of libxml2. This package is tested using the
  win32 port at:
    http://www.zlatkovic.com/pub/libxml/

  This package should contain, at least:
    /bin/libxml2.dll
    /lib/libxml2.lib
    and various .h files in /include/

  Place these directories and files inside the libxml2 directory.


[Step 6]:
  Obtain some win32 build of libiconv. This package is tested with the
  win32 port at:
    http://gnuwin32.sourceforge.net/packages/libiconv.htm

  The developer files package should contain, at least:
    /lib/libiconv.lib
   and various .h files in /include/

  Place these directories and files inside the libxml2 directory.


[Step 7]:
  Open Penguin.sln inside Visual Studio and build (but don't run) the solution.


[Step 8]:
  Once Visual Studio builds the project, you need to add two DLLs to the build
  directory. Namely:
    libxml2.dll
    iconv.dll

  By default, it is built in:
    Win32/Debug/bin/


[Step 9]:
  Run the project!






