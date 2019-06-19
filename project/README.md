```
$ ./project.exe test/for.c
sum = 0;
for(i=1;i<=10;i++)
{
    sum=sum+i;
}
========== lex ==============
token=sum
token==
token=0
token=;
token=for
token=(
token=i
token==
token=1
token=;
token=i
token=<
token==
token=10
token=;
token=i
token=+
token=+
token=)
token={
token=sum
token==
token=sum
token=+
token=i
token=;
token=}
========== dump ==============
0:sum
1:=
2:0
3:;
4:for
5:(
6:i
7:=
8:1
9:;
10:i
11:<
12:=
13:10
14:;
15:i
16:+
17:+
18:)
19:{
20:sum
21:=
22:sum
23:+
24:i
25:;
26:}
============ parse =============
0       @0      # t0 = 0
1       D=A
2       @t0
3       M=D
4       @t0     # sum = t0
5       D=M
6       @sum
7       M=D
8       @1      # t1 = 1
9       D=A
10      @t1
11      M=D
12      @t1     # i = t1
13      D=M
14      @i
15      M=D
(L0)
16      @i      # t2 = i
17      D=M
18      @t2
19      M=D
20      @10     # t3 = 10
21      D=A
22      @t3
23      M=D
24      @t2     # if not t2 <= t3 goto L3
25      D=M
26      @t3
27      D=D-M
28      @L3
29      D;JGT
30      @L2     # goto L2
31      0;JMP
(L1)
32      @i      # t4 = i
33      D=M
34      @t4
35      M=D
36      @1      # t5 = 1
37      D=A
38      @t5
39      M=D
40      @t4     # t6 = t4 + t5
41      D=M
42      @t5
43      D=D+M
44      @t6
45      M=D
46      @t6     # i = t6
47      D=M
48      @i
49      M=D
50      @L0     # goto L0
51      0;JMP
(L2)
52      @sum    # t7 = sum
53      D=M
54      @t7
55      M=D
56      @i      # t8 = i
57      D=M
58      @t8
59      M=D
60      @t7     # t9 = t7 + t8
61      D=M
62      @t8
63      D=D+M
64      @t9
65      M=D
66      @t9     # sum = t9
67      D=M
68      @sum
69      M=D
70      @L1     # goto L1
71      0;JMP
(L3)
```