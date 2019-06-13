# homework3
```
$ ./homework3.exe test/if.c
score = 61;
if (score < 60) {
    pass = 1;
}
else if (score < 80) {
    pass = 2;
}
else {
    pass = 3;
}
========== lex ==============
token=score
token==
token=61
token=;
token=if
token=(
token=score
token=<
token=60
token=)
token={
token=pass
token==
token=1
token=;
token=}
token=else
token=if
token=(
token=score
token=<
token=80
token=)
token={
token=pass
token==
token=2
token=;
token=}
token=else
token={
token=pass
token==
token=3
token=;
token=}
========== dump ==============
0:score
1:=
2:61
3:;
4:if
5:(
6:score
7:<
8:60
9:)
10:{
11:pass
12:=
13:1
14:;
15:}
16:else
17:if
18:(
19:score
20:<
21:80
22:)
23:{
24:pass
25:=
26:2
27:;
28:}
29:else
30:{
31:pass
32:=
33:3
34:;
35:}
============ parse =============
@61     t0 = 61
D=A
@t0
M=D
@t0     score = t0
D=M
@score
M=D
@score  t1 = score
D=M
@t1
M=D
@60     t2 = 60
D=A
@t2
M=D
@t1     if not t1 < t2 goto L0
D=M
@t2
D=D-M
@L0
D;JGE
@1      t3 = 1
D=A
@t3
M=D
@t3     pass = t3
D=M
@pass
M=D
@L1     goto L1
0;JMP
(L0)
@score  t4 = score
D=M
@t4
M=D
@80     t5 = 80
D=A
@t5
M=D
@t4     if not t4 < t5 goto L2
D=M
@t5
D=D-M
@L2
D;JGE
@2      t6 = 2
D=A
@t6
M=D
@t6     pass = t6
D=M
@pass
M=D
@L1     goto L1
0;JMP
(L2)
@3      t7 = 3
D=A
@t7
M=D
@t7     pass = t7
D=M
@pass
M=D
(L1)
```