#include <assert.h>
#include "compiler.h"

int E();
void STMT();
void IF();
void BLOCK();

int tempIdx = 0, labelIdx = 0;
/*char mm = "case";
strstr(token,mm);*/

#define nextTemp() (tempIdx++)
#define nextLabel() (labelIdx++)
#define emit printf

int isNext(char *set) {
  char eset[SMAX], etoken[SMAX];
  sprintf(eset, " %s ", set);
  sprintf(etoken, " %s ", tokens[tokenIdx]);
  return (tokenIdx < tokenTop && strstr(eset, etoken) != NULL);
}

int isEnd() {
  return tokenIdx >= tokenTop;
}

char *next() {
  // printf("token[%d]=%s\n", tokenIdx, tokens[tokenIdx]);
  return tokens[tokenIdx++];
}

char *skip(char *set) {
  if (isNext(set)) {
    return next();
  } else {
    printf("skip(%s) got %s fail!\n", set, next());
    assert(0);
  }
}

// F = (E) | Number | Id
int F() {
  int f;
  if (isNext("(")) { // '(' E ')'
    next(); // (
    f = E();
    next(); // )
  } else { // Number | Id
    f = nextTemp();
    char *item = next();
    emit("t%d = %s\n", f, item);
  }
  return f;
}

// E = F (op E)*
int E() {
  int i1 = F();
  while (isNext("+ - * / & | ! < > =")) {
    char *op = next();
    if(isNext("& | =")) strcat(op,next());
    int i2 = E();
    int i = nextTemp();
    emit("t%d = t%d %s t%d\n", i, i1, op, i2);
    i1 = i;
  }
  return i1;
}

// ASSIGN = id '=' E;
void ASSIGN() {
    char *id = next();
    skip("=");
    int e = E();
    skip(";");
    emit("%s = t%d\n", id, e);
}

// while (E) STMT
void WHILE() {
  int whileBegin = nextLabel();
  int whileEnd = nextLabel();
  emit("(L%d)\n", whileBegin);
  skip("while");
  skip("(");
  int e = E();
  emit("if not t%d goto L%d\n", e, whileEnd);
  skip(")");
  STMT();
  emit("goto L%d\n", whileBegin);
  emit("(L%d)\n", whileEnd);
}

void STMT() {
  if (isNext("while"))
    return WHILE();
  else if (isNext("if"))
    IF();
  else if (isNext("{"))
    BLOCK();
  else
    ASSIGN();
}

// IF = if(E) STMT 擴充:可以有else if與只有if沒有else
void IF() {
  int elseBegin = nextLabel();
  skip("if");
  skip("(");
  int e = E();
  emit("if not t%d goto L%d\n", e, elseBegin);
  skip(")");
  STMT();
  if(!isNext("else"))
  {
    emit("(L%d)\n", elseBegin);
  }
  else{
    int ifEnd = nextLabel();
    emit("goto L%d\n",ifEnd);
    emit("(L%d)\n", elseBegin);
    while(isNext("else"))
    {
      skip("else");
      if(isNext("if"))
      {
        int nextelse = nextLabel();
        skip("if");
        skip("(");
        e = E();
        emit("if not t%d goto L%d\n", e, nextelse);
        skip(")");      
        STMT();
        emit("goto L%d\n",ifEnd);
        emit("(L%d)\n", nextelse);
      }
      else 
      {
        STMT();
        break;
      }
    }
    emit("(L%d)\n", ifEnd);
  }
}

void STMTS() {
  while (!isEnd() && !isNext("}")) {
    STMT();
  }
}

// { STMT* }
void BLOCK() {
  skip("{");
  STMTS();
  skip("}");
}

void PROG() {
  STMTS();
}

void parse() {
  printf("============ parse =============\n");
  tokenIdx = 0;
  PROG();
}