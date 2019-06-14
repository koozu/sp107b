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
0       @61     # t0 = 61
1       D=A
2       @t0
3       M=D
4       @t0     # score = t0
5       D=M
6       @score
7       M=D
8       @score  # t1 = score
9       D=M
10      @t1
11      M=D
12      @60     # t2 = 60
13      D=A
14      @t2
15      M=D
16      @t1     # if not t1 < t2 goto L0
17      D=M
18      @t2
19      D=D-M
20      @L0
21      D;JGE
22      @1      # t3 = 1
23      D=A
24      @t3
25      M=D
26      @t3     # pass = t3
27      D=M
28      @pass
29      M=D
30      @L1     # goto L1
31      0;JMP
(L0)
32      @score  # t4 = score
33      D=M
34      @t4
35      M=D
36      @80     # t5 = 80
37      D=A
38      @t5
39      M=D
40      @t4     # if not t4 < t5 goto L2
41      D=M
42      @t5
43      D=D-M
44      @L2
45      D;JGE
46      @2      # t6 = 2
47      D=A
48      @t6
49      M=D
50      @t6     # pass = t6
51      D=M
52      @pass
53      M=D
54      @L1     # goto L1
55      0;JMP
(L2)
56      @3      # t7 = 3
57      D=A
58      @t7
59      M=D
60      @t7     # pass = t7
61      D=M
62      @pass
63      M=D
(L1)
```