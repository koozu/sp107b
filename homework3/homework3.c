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

void emitts(int f,char *item){
  emit("@%s\tt%d = %s\n",item,f,item);
  if(isAlpha(item[0]))
  {
    emit("D=M\n");
  }
  else
  {
    emit("D=A\n");
  }
  emit("@t%d\n",f);
  emit("M=D\n");
}

void emitst(char *id, int i){
  emit("@t%d\t%s = t%d\n", i, id, i);
  emit("D=M\n");
  emit("@%s\n",id);
  emit("M=D\n");
}

void emitop(int i, int i1, char *op, int i2, int jump){
  if(strcmp(op,"+")==0||strcmp(op,"-")==0||strcmp(op,"*")==0||strcmp(op,"/")==0) {
    emit("@t%d\tt%d = t%d %s t%d\n", i1, i, i1, op, i2);
    emit("D=M\n");
    emit("@t%d\n",i2);
    emit("D=D%sM\n",op);
    emit("@t%d\n",i);
    emit("M=D\n");
  }
  else {
    emit("@t%d\tif not t%d %s t%d goto L%d\n", i1, i1, op, i2, jump);
    emit("D=M\n");
    emit("@t%d\n",i2);
    emit("D=D-M\n");
    emit("@L%d\n",jump);
    if(strcmp(op,"<")==0)  emit("D;JGE\n"); 
    else if(strcmp(op,">")==0)  emit("D;JLE\n");       
    else if(strcmp(op,"=")==0)  emit("D;JNE\n");       
    else if(strcmp(op,"!=")==0)  emit("D;JEQ\n");
    else if(strcmp(op,">=")==0)  emit("D;JLT\n");
    else if(strcmp(op,"<=")==0)  emit("D;JGT\n");
  }
}

void emitgoto(int jump){
  emit("@L%D\tgoto L%d\n",jump,jump);
  emit("0;JMP\n");
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