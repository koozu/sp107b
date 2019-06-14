#include <assert.h>
#include "compiler.h"

int E(int);
void STMT();
void IF();
void BLOCK();
void emitts(int, char *);
void emitop(int, int, char *, int, int);
void emitst(char *, int);
void emitgoto(int);

int tempIdx = 0, labelIdx = 0, pcIdx = 0;
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

void emitts(int f,char *item){
  emit("%d\t@%s\t# t%d = %s\n",pcIdx++,item,f,item);
  if(isAlpha(item[0]))
  {
    emit("%d\tD=M\n",pcIdx++);
  }
  else
  {
    emit("%d\tD=A\n",pcIdx++);
  }
  emit("%d\t@t%d\n",pcIdx++,f);
  emit("%d\tM=D\n",pcIdx++);
}

void emitst(char *id, int i){
  emit("%d\t@t%d\t# %s = t%d\n", pcIdx++, i, id, i);
  emit("%d\tD=M\n", pcIdx++);
  emit("%d\t@%s\n", pcIdx++, id);
  emit("%d\tM=D\n", pcIdx++);
}

void emitop(int i, int i1, char *op, int i2, int jump){
  if(strcmp(op,"+")==0||strcmp(op,"-")==0||strcmp(op,"*")==0||strcmp(op,"/")==0) {
    emit("%d\t@t%d\t# t%d = t%d %s t%d\n", pcIdx++, i1, i, i1, op, i2);
    emit("%d\tD=M\n", pcIdx++);
    emit("%d\t@t%d\n", pcIdx++, i2);
    emit("%d\tD=D%sM\n", pcIdx++, op);
    emit("%d\t@t%d\n", pcIdx++, i);
    emit("%d\tM=D\n", pcIdx++);
  }
  else {
    emit("%d\t@t%d\t# if not t%d %s t%d goto L%d\n", pcIdx++, i1, i1, op, i2, jump);
    emit("%d\tD=M\n", pcIdx++);
    emit("%d\t@t%d\n", pcIdx++,i2);
    emit("%d\tD=D-M\n", pcIdx++);
    emit("%d\t@L%d\n", pcIdx++,jump);
    if(strcmp(op,"<")==0)  emit("%d\tD;JGE\n", pcIdx++); 
    else if(strcmp(op,">")==0)  emit("%d\tD;JLE\n", pcIdx++);       
    else if(strcmp(op,"=")==0)  emit("%d\tD;JNE\n", pcIdx++);       
    else if(strcmp(op,"!=")==0)  emit("%d\tD;JEQ\n", pcIdx++);
    else if(strcmp(op,">=")==0)  emit("%d\tD;JLT\n", pcIdx++);
    else if(strcmp(op,"<=")==0)  emit("%d\tD;JGT\n", pcIdx++);
  }
}

void emitgoto(int jump){
  emit("%d\t@L%D\t# goto L%d\n", pcIdx++, jump, jump);
  emit("%d\t0;JMP\n", pcIdx++);
}

// F = (E) | Number | Id
int F(int jump) {
  int f;
  if (isNext("(")) { // '(' E ')'
    next(); // (
    f = E(jump);
    next(); // )
  } else { // Number | Id
    f = nextTemp();
    char *item = next();
    //emit("t%d = %s\n", f, item);
    emitts(f,item);
  }
  return f;
}

// E = F (op E)*
int E(int jump) {
  int i1 = F(jump);
  while (isNext("+ - * / & | ! < > =")) {
    char *op = next();
    if(isNext("& | =")) strcat(op,next());
    int i2 = E(jump);
    int i;
    if(strcmp(op,"+")==0||strcmp(op,"-")==0||strcmp(op,"*")==0||strcmp(op,"/")==0) {
      i = nextTemp();
    }
    else {
      i = i2;
    }
    //emit("t%d = t%d %s t%d\n", i, i1, op, i2);
    emitop(i, i1, op, i2, jump);
    i1 = i;
  }
  return i1;
}

// ASSIGN = id '=' E;
void ASSIGN() {
    char *id = next();
    skip("=");
    int e = E(0);
    skip(";");
    //emit("%s = t%d\n", id, e);
    emitst(id, e);
}

// while (E) STMT
void WHILE() {
  int whileBegin = nextLabel();
  int whileEnd = nextLabel();
  emit("(L%d)\n", whileBegin);
  skip("while");
  skip("(");
  int e = E(whileEnd);
  //emit("if not t%d goto L%d\n", e, whileEnd);
  skip(")");
  STMT();
  //emit("goto L%d\n", whileBegin);
  emitgoto(whileBegin);
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
  int e = E(elseBegin);
  //emit("if not t%d goto L%d\n", e, elseBegin);
  skip(")");
  STMT();
  if(!isNext("else"))
  {
    emit("(L%d)\n", elseBegin);
  }
  else{
    int ifEnd = nextLabel();
    //emit("goto L%d\n",ifEnd);
    emitgoto(ifEnd);
    emit("(L%d)\n", elseBegin);
    while(isNext("else"))
    {
      skip("else");
      if(isNext("if"))
      {
        int nextelse = nextLabel();
        skip("if");
        skip("(");
        e = E(nextelse);
        //emit("if not t%d goto L%d\n", e, nextelse);
        skip(")");      
        STMT();
        //emit("goto L%d\n",ifEnd);
        emitgoto(ifEnd);
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