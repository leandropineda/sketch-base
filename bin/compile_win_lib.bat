set JAVA_HOME=C:\Program Files\Java\jdk1.8.0_121
REM Define and Set environment variable JAVA_HOME to JDK installed directory
REM I recommend that you set JAVA_HOME permanently, via "Control Panel" ⇒ "System" ⇒ "Environment Variables"
echo %JAVA_HOME%
REM In Windows, you can refer a environment variable by adding % prefix and suffix 
g++ -Wl,--add-stdcall-alias -std=c++11 -I"%JAVA_HOME%\include" -I"%JAVA_HOME%\include\win32" -shared -o sketch.dll ../src/sketch.h
REM Compile HellJNI.c into shared library hello.dll
nm sketch.dll 