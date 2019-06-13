# homework2
```
$ ./homework2.exe test/if.c
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
t0 = 61
score = t0
t1 = score
t2 = 60
t3 = t1 < t2
if not t3 goto L0
t4 = 1
pass = t4
goto L1
(L0)
t5 = score
t6 = 80
t7 = t5 < t6
if not t7 goto L2
t8 = 2
pass = t8
goto L1
(L2)
t9 = 3
pass = t9
(L1)
```
