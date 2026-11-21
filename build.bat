@echo off
setlocal

if not exist bin mkdir bin

gcc -std=c99 -Wall -Wextra -O2 fcfs.c -o bin\fcfs.exe || goto :error
gcc -std=c99 -Wall -Wextra -O2 sjf.c -o bin\sjf.exe || goto :error
gcc -std=c99 -Wall -Wextra -O2 srtf.c -o bin\srtf.exe || goto :error
gcc -std=c99 -Wall -Wextra -O2 round_robin.c -o bin\round_robin.exe || goto :error
gcc -std=c99 -Wall -Wextra -O2 priority.c -o bin\priority.exe || goto :error

echo Build complete. Executables are in the bin folder.
exit /b 0

:error
echo Build failed.
exit /b 1
