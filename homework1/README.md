$ ./exp0hack.exe '5-3*2+4/2'
=== EBNF Grammar =====
E=F ([+-] F)*
F=Number | '(' E ')'
==== parse:5-3*2+4/2 ========
# t0=5
@5
D=A
@t0
M=D
# t1=3
@3
D=A
@t1
M=D
# t2=2
@2
D=A
@t2
M=D
# t3=t1*t2
@t1
D=M
@t2
D=D*M
@t3
M=D
# t4=t0-t3
@t0
D=M
@t3
D=D-M
@t4
M=D
# t5=4
@4
D=A
@t5
M=D
# t6=2
@2
D=A
@t6
M=D
# t7=t5/t6
@t5
D=M
@t6
D=D/M
@t7
M=D
# t8=t4+t7
@t4
D=M
@t7
D=D+M
@t8
M=D
