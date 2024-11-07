#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "AnalisadorLexico.h"

#define TAM_LEXEMA 50
#define TAM_NUM 20

void error(char msg[]) {
    printf("%s\n", msg);
    exit(1);
}

TOKEN AnalisadorLexico(FILE *fd) {

    TOKEN tk;
    int estado = 0;
    char c;
    int tamL = 0;
    int tamD = 0;
    char lexema[TAM_LEXEMA] = "";
    char digitos[TAM_NUM] = "";
    


    while (1) {
        c = fgetc(fd);

        switch (estado) {
            case 0:  // Estado inicial
                if (c == ' '  ||c == '\t' || c == '\n') {
                    estado = 0;  // Ignora espaços e tabulações
                    contLinha++;
                }
                else if (c == '_' ){
                 estado = 1;
                 lexema [tamL]= c; 
                 lexema [++tamL]='\0';
                }
                else if (isalpha(c)){
                    estado = 2;
                    lexema [tamL]= c;
                    lexema[++tamL]= '\0';
                }
                else if (isdigit(c)){
                    estado = 4;
                    digitos[tamD] = c;
                    digitos[++tamD]= '\0';
                }
            
                else if (c == '\''){
                    estado = 8;
                }
                else if (c == '"'){
                    estado = 12;
                }
                else if (c == '/'){
                   estado = 15;
                }
                else if (c == '+'){
                   estado = 19;
                   tk.cat = SN;
                   tk.codigo = ADICAO;
                   return tk;
                }
                else if (c == '-'){
                   estado = 20;
                   tk.cat = SN;
                   tk.codigo = SUBTRACAO;
                   return tk;
                }
                else if (c == '*'){
                   estado = 22;
                   tk.cat = SN;
                   tk.codigo = MULTIPLICACAO;
                   return tk;
                }
                else if (c == '>'){
                   estado = 23;
        
                }
                else if (c == '<'){
                   estado = 24;
                }
                else if (c == '='){
                   estado = 26;
                }
                else if (c == '&'){
                   estado = 27;
        
                }
                else if (c == ','){
                   estado = 29;
                   tk.cat = SN;
                   tk.codigo = VIRGULA;
                   return tk;
                }
                 else if (c == '!'){
                   estado = 30;
        
                }
                else if (c == '('){
                   estado = 32;
                   tk.cat = SN;
                   tk.codigo = PAR_ESQUERDO;
                   return tk;
                }
                else if (c == ')'){
                   estado = 33;
                   tk.cat = SN;
                   tk.codigo = PAR_DIREITO;
                   return tk;
                }
                else if (c == '['){
                   estado = 34;
                   tk.cat = SN;
                   tk.codigo = COL_ESQUERDO;
                   return tk;
                }
                else if (c == ']'){
                   estado = 35;
                   tk.cat = SN;
                   tk.codigo = COL_DIREITO;
                   return tk;
                }
                else if (c == '|'){
                   estado = 36;
                }
                else if (c =='\n'){
                    estado = 0;
                    tk.cat = FIM_EXPR;
                    contLinha++;
                    return tk;
                }
                else if(c == EOF){
                    tk.cat = FIM_ARQ;
                    return tk;
                }
                else{
                    error("Caracter invalido");

                }
                break;

                case 1:
                if(isalpha(c)){
                    estado = 2; //vai para o estado 2
                    lexema[tamL] = c;
                    lexema[++tamL]='\0';
                }
                else if (c == '_'){
                    estado = 1; //fica no estado 1
                    lexema[tamL]= c;
                    lexema[++tamL] = '\0';
                }
                break;

                case 2:
                if(isalpha(c) || isdigit(c) || c =='_'){
                    estado = 2; //fica no no proprio estado
                    lexema[tamL]=c;
                    lexema[++tamL]= '\0';
                }
                else {
                    ungetc(c, fd);//reconhecendo o token
                    estado = 3; //indo pro estado 3
                    tk.cat = ID;
                    strcpy(tk.lexema,lexema);
                    return tk;
                }
                break;

                case 4:
                if(isdigit(c)){
                    estado = 4;
                    digitos[tamD]=c;
                    digitos[++tamD]='\0';
                }
                else if(c == '.'){
                    estado = 5;
                    digitos[tamD]=c;
                    digitos[++tamD]='\0';
                }
                else{
                    ungetc(c,fd);
                    estado = 40;
                    tk.cat = CT_I;
                    tk.valorInt = atoi(digitos);
                    tamD = 0;
                    return tk;
                }
                break;

                case 5:
                if(isdigit(c)){
                    estado = 6;
                    digitos[tamD]=c;
                    digitos[++tamD]='\0';
                }
                break;

                case 6:
                if(isdigit(c)){
                    estado = 6;
                    digitos[tamD]=c;
                    digitos[++tamD]='\0';
                }
                else{
                    ungetc(c,fd);
                    estado = 7;
                    tk.cat = CT_R;
                    tk.valorReal =atof(digitos);
                    tamD = 0;
                    return tk;
                }
                break;

                case 8:
                if(isprint(c)){
                    estado = 10;
                    lexema[tamL]=c;
                    lexema[++tamL]='\0';
                }
                else if (c == '\\') {
                    estado = 9;
                }
                else if(c == '\''){
                    estado = 11;
                    tk.cat = CT_C;
                    tk.c = lexema[0];
                    return tk;
                }
                break;

                case 9:
                if (c == 'n'){
                    estado = 44;
                    tk.cat = CT_C;
                    tk.c = '\n';
                }
                else if (c == '0'){
                    estado = 41;
                    tk.cat = CT_C;
                    tk.c = '\0';
                }
                break;
                case 44:
                if(c == '\''){
                    estado = 45;
                    return tk;
                }
                break;
                case 41:
                if(c == '\''){
                    estado = 42;
                    return tk;
                }
                break;

                case 10:
                if(c == '\''){
                    estado = 11;
                    tk.cat = CT_C;
                    tk.c = lexema[0];
                    return tk;
                }
                break;

                case 12:
                tamL = 0;
                if(c == '"'){
                    estado = 14;
                    tk.cat = LT;
                    strcpy(tk.lexema , "");
                    return tk;
                }
                else if(isprint(c)){
                    estado = 13;
                    lexema[tamL]= c;
                    lexema[++tamL]='\0';
                }
                break;

                case 13:
                tamL = 0;
                while (c != '"' && c != EOF) {
                    lexema[tamL++] = c;  // Armazena o caractere no lexema
                    c = fgetc(fd);
                }
                 if(c == '"'){
                    estado = 14;
                    tk.cat = LT;
                    strcpy(tk.lexema , lexema);
                    return tk;
                }
                break;
                
                case 15:
                if(c == '/'){
                    estado = 16;
                    lexema[tamL]=c;
                    lexema[++tamL]='\0';
                }
                else{
                    ungetc(c,fd);
                    estado = 21;
                    tk.cat = SN;
                    tk.codigo = DIVISAO;
                    return tk;
                }
                break;

                case 16:
                tk.cat = COMENTARIO;
                while ((c = fgetc(fd)) != '\n' && c != EOF) {
                // Ignora todos os caracteres até o final da linha ou EOF
                }
                ungetc('\n', fd);  // Coloca a nova linha de volta no fluxo para sinalizar fim da expressão, se necessário
                return tk;
                
                case 23:
                if (c == '='){
                    estado = 25;
                    tk.cat = SN;
                    tk.codigo = MAIOR_IGUAL;
                    return tk;
                }
                else{
                    ungetc(c,fd);
                    estado = 39;
                    tk.cat = SN;
                    tk.codigo = MAIOR_QUE;
                    return tk;
                }
                break;

                case 24:
                if(c == '='){
                    estado = 25;
                    tk.cat = SN;
                    tk.codigo = MENOR_IGUAL;
                    return tk;
                }
                else{
                    ungetc(c,fd);
                    estado = 39;
                    tk.cat = SN;
                    tk.codigo = MENOR_QUE;
                    return tk;
                }
                break;

                case 26:
                if(c == '='){
                    estado = 25;
                    tk.cat = SN;
                    tk.codigo = IGUALDADE;
                    return tk;
                }
                else{
                    ungetc(c,fd);
                    estado = 39;
                    tk.cat = SN;
                    tk.codigo = ATRIBUICAO;
                    return tk;
                }
                break;

                case 27:
                if(c == '&'){
                    estado = 28;
                    tk.cat = SN;
                    tk.codigo = ENDERECO;
                    return tk;
                }
                else{
                    ungetc(c,fd);
                    estado = 39;
                    tk.cat = SN;
                    tk.codigo = AND;
                    return tk;
                }
                break;
                
                case 30:
                if(c == '='){
                    estado = 31;
                    tk.cat = SN;
                    tk.codigo = DIFERENTE;
                    return tk;
                }
                else{
                    ungetc(c,fd);
                    estado = 38;
                    tk.cat = SN;
                    tk.codigo = NOT;
                    return tk;
                }
                break;
             }
         }
    }
    int main(){
    FILE *fd;
    TOKEN token;
    if ((fd=fopen("teste.txt", "r")) == NULL) {
        error("Arquivo de entrada da expressao nao encontrado!");
    }
    while (true){
        token = AnalisadorLexico(fd);
        printf("<Linha %d", contLinha);
        switch (token.cat) {

            case SN: switch (token.codigo) {

                case PAR_ESQUERDO:
                    printf("<SN, PARENTESE ESQUERDO ");
                    break;
                
                case PAR_DIREITO:
                    printf("<SN, PARENTESE DIREITO ");
                    break;
                    
                case COL_ESQUERDO: 
                    printf("<SN, COLCHETE ESQUERDO> ");
                    break;

                case COL_DIREITO: 
                    printf("<SN, COLCHETE DIREITO> ");
                    break;

                case VIRGULA: 
                    printf("<SN, VIRGULA> ");
                    break;

                case ENDERECO: 
                    printf("<SN, ENDERECO> ");
                    break;

                case ATRIBUICAO: 
                    printf("<SN, ATRIBUICAO> ");
                    break;

                case ADICAO: 
                    printf("<SN, ADICAO> ");
                    break;

                case SUBTRACAO: 
                    printf("<SN, SUBTRACAO> ");
                    break;

                case MULTIPLICACAO: 
                    printf("<SN, MULTIPLICACAO> ");
                    break;

                case DIVISAO: 
                    printf("<SN, DIVISAO> ");
                    break;

                case IGUALDADE: 
                    printf("<SN, IGUALDADE> ");
                    break;

                case DIFERENTE:
                    printf("<SN, DIFERENTE> ");
                    break;

                case MENOR_IGUAL: 
                    printf("<SN, MENOR ou IGUAL> ");
                    break;

                case MAIOR_IGUAL: 
                    printf("<SN, MAIOR ou IGUAL> ");
                    break;

                case MENOR_QUE: 
                    printf("<SN, MENOR QUE> ");
                    break;

                case MAIOR_QUE: 
                    printf("<SN, MAIOR QUE> ");
                    break;

                case AND: 
                    printf("<SN, AND> ");
                    break;

                case OR: 
                    printf("<SN, OR> ");
                    break;

                case NOT:
                    printf("<SN, NOT> ");
                    break;
            }
            break;
            case PV_R:
                switch (token.codigo){
                    case CONST:
                        printf("<PR, CONST>\n");
                        break;
                    
                    case PR:
                        printf("<PR, PR>\n");
                        break;

                    case INIT:
                        printf("<PR, INIT>\n");
                        break;

                    case ENDP:
                        printf("<PR, ENDP>\n");
                        break;

                    case CHAR:
                        printf("<PR, CHAR>\n");
                        break;

                    case INT:
                        printf("<PR, INT>\n");
                        break;

                    case REAL:
                        printf("<PR, REAL>\n");
                        break;

                    case BOOL:
                        printf("<PR, BOOL>\n");
                        break;

                    case DO:
                        printf("<PR, DO>\n");
                        break;

                    case WHILE:
                        printf("<PR, WHILE>\n");
                        break;

                    case ENDW:
                        printf("<PR, ENDW>\n");
                        break;

                    case VAR:
                        printf("<PR, VAR>\n");
                        break;

                    case FROM:
                        printf("<PR, FROM>\n");
                        break;

                    case TO:
                        printf("<PR, TO>\n");
                        break;

                    case DT:
                        printf("<PR, DT>\n");
                        break;

                    case BY:
                        printf("<PR, BY>\n");
                        break;

                    case IF:
                        printf("<PR, IF>\n");
                        break;

                    case ENDV:
                        printf("<PR, ENDV>\n");
                        break;

                    case ELIF:
                        printf("<PR, ELIF>\n");
                        break;

                    case ELSE:
                        printf("<PR, ELSE>\n");
                        break;

                    case ENDI:
                        printf("<PR, ENDI>\n");
                        break;

                    case GETOUT:
                        printf("<PR, GETOUT\n");
                        break;

                    case GETINT:
                        printf("<PR, GETINT>\n");
                        break;

                    case GETCHAR:
                        printf("<PR, GETCHAR>\n");
                        break;

                    case GETREAL:
                        printf("<PR, GETREAL>\n");
                        break;

                    case PUTINT:
                        printf("<PR, PUTINT>\n");
                        break;

                    case PUTCHAR:
                        printf("<PR, PUTCHAR>\n");
                        break;

                    case PUTREAL:
                        printf("<PR, PUTREAL>\n");
                        break;
                }
                break; 

            case ID: 
                printf("<ID, %s> ", token.lexema);
                break;
            case CT_I:
                printf("<CT_I, %d>\n", token.valorInt);
                break;
            case CT_R:
                printf("<CT_R, %f>\n", token.valorReal);
                break;
            case CT_C:
                if (token.c == '\n') {
                    printf("<CT_C, \\n> \n");
                } 
                else if (token.c == '\0') {
                    printf("<CT_C, \\0> \n");
                } 
                else {
                    printf("<CT_C, %c> \n", token.c);
                }
                break;
            case LT:
                printf("<LT, %s> \n", token.lexema);
                break;
            case COMENTARIO:
                printf("<CMT, COMENTARIO>\n");
                break;
            case FIM_ARQ: 
                printf(" <Fim do arquivo encontrado>\n");
                break;
        }
        if (token.cat == FIM_ARQ) break;
    }
    
    fclose(fd);
    return 0;

}
