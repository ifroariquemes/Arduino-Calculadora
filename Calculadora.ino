/*
 * Calculadora
 * 
 * IFRO - Instituto Federal de Rondonia
 * Campus Ariquemes
 * 
 * Natanael Simões <natanael.simoes@ifro.edu.br>
 * Professor de Informatica
 */
#include <LiquidCrystal.h>  
  
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);  
String num;
char newNum, oldNum, newOp, oldOp, op, wipe[16];
float num1, num2;
bool recentCalc;

/* Inicializa LCD */
void setup() {  
  lcd.begin(16, 2);  
  clearScr();
}  

/* A cada 50ms checa botoes pressionados */
void loop() {  
  checkOperation();
  checkNumber(); 
  delay(50); 
}

/* Limpa variaveis e LCD */
void clearScr() {
  num = String();  
  op = '\0';
  recentCalc = false;
  lcd.clear();
  lcd.setCursor(15,0);
  lcd.print('0');  
  recentCalc = true;
}

/* Escreve na tela
 * @param line Numero da linha
 * @param val Texto a ser impresso
 */
void printAt(int line, String val) {  
  lcd.setCursor(16-val.length(),line); //Escreve da direita para a esquerda
  lcd.print(val);
}

/* Atribui operacao */
void setOperation() {
  switch(op) {
     // Caso haja operacao vai para a proxima
     case '+': newOp = '-'; break;
     case '-': newOp = '*'; break;
     case '*': newOp = '/'; break;
     case '/': newOp = 'R'; break;
     case 'R': newOp = '^'; break;
     case '^': newOp = '+'; break;
     default :
     //Primeira operacao sera soma
      if(num.length() > 0) {
       newOp = '+';       
       num1 = num.toFloat();       
       num = String();
       recentCalc = false;
      }
      break;
   }  
   //Imprime o simbolo da operacao no canto esquerdo inferior
   if(newOp != '\0') { 
    op = newOp;       
    lcd.setCursor(0,1);
    lcd.print(op);
   }
}

/* Executa o calculo */
void calculate() {
  num2 = num.toFloat();
  switch(op) {
    //Verifica a operacao e aplica nas variaveis
    case '+': num1 += num2; break;
    case '-': num1 -= num2; break;
    case '*': num1 *= num2; break;
    case '/': num1 /= num2; break;
    case 'R': num1 = pow(num1, 1/num2); break;
    case '^': num1 = pow(num1, num2); break;
  }   
  //Limpa a tela e imprime resultado na primeira linha
  clearScr();
  num = String(num1);      
  printAt(0, num); 
  newOp = '1'; 
  recentCalc = true;
}

/* Checa se botao de numero foi pressionado */
void checkNumber() {
  //A0 = Botoes do LCD (LEFT para ponto decimal)
  int dotBtn = analogRead(0);
  //A1 = Botoes numericos
  int numBtn = analogRead(1);
  if(numBtn >= 90 && numBtn <= 100)
    newNum = '1';
  else if(numBtn >= 105 && numBtn <= 115)
    newNum = '2';
  else if(numBtn >= 120 && numBtn <= 130)
    newNum = '3';
  else if(numBtn >= 135 && numBtn <= 145)
    newNum = '4';
  else if(numBtn >= 160 && numBtn <= 170)
    newNum = '5';
  else if(numBtn >= 190 && numBtn <= 210)
    newNum = '6'; 
  else if(numBtn >= 240 && numBtn <= 260)
    newNum = '7';
  else if(numBtn >= 330 && numBtn <= 350)
    newNum = '8'; 
  else if(numBtn >= 500 && numBtn <= 520)
    newNum = '9'; 
  else if(numBtn > 600)
    newNum = '0';
  else if(dotBtn >= 700 && dotBtn <= 750 && num.indexOf('.') < 0) // LCD LEFT, NO DOTS YET
    newNum = '.';
  else
    newNum = '\0'; 
 
  if(newNum != oldNum) { 
    oldNum = newNum;
    if(recentCalc)
      clearScr();
    if(newNum != '\0' && num.length() <= 14) {
      num.concat(newNum);   
      printAt((op!='\0'), num);    
    }
  }
}

/* Checa se botao de operacao foi pressionado */
void checkOperation() {
  //A5 = Botoes de operacao
  int opBtn = analogRead(5);
  // Se botao para troca operacao for pressionado
  if(opBtn >= 500 && opBtn <= 520) { 
    if(newOp == '\0') 
    setOperation();
  } 
  // Se botao de igual for pressionado
  else if(opBtn > 600) { 
    if(newOp == '\0')
    calculate();
  }
  else
    newOp = '\0';  
}
