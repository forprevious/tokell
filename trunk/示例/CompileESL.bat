armcc ell.c
armlink -ro-base 0 ell.o
ESlCompiler ell.o __image.axf
pause