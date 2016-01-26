

int PIN_RS;
int PIN_D0;
int PIN_EB;

void lcdCmdWrite(int val){
  int i=0;
  for(i=PIN_D0;i<=PIN_RS;i++){
    // val是   RS-RW-D7...D0的数据
    // 从D0倒着写到RS PIN口一位一位的写数据
    digitalWrite(i, val & 01);
    val >>= 1;
  }

  digitalWrite(PIN_EB,LOW);
  delayMicroseconds(1);
  digitalWrite(PIN_EB,HIGH);
  delayMicroseconds(1); 
  digitalWrite(PIN_EB,LOW);
  delayMicroseconds(1);
}

void lcdDataWrite(int val){
  // 定义所有引脚
  int i = 0;
  digitalWrite(PIN_RS, HIGH);
  digitalWrite(PIN_RS-1, LOW);
  for (i=PIN_D0; i < PIN_D0+8; i++) {
    digitalWrite(i,val & 01);
    val >>= 1;
  }
  digitalWrite(PIN_EB,LOW);
  delayMicroseconds(1);
  digitalWrite(PIN_EB,HIGH);
  delayMicroseconds(1);
  digitalWrite(PIN_EB,LOW);
  delayMicroseconds(1);
}

void setupLCD(int eb, int d0){
  PIN_EB = eb;
  PIN_D0 = d0;
  PIN_RS = d0+9;
  
  int i = 0;
  for (i=PIN_EB; i <= PIN_RS; i++) {
    pinMode(i,OUTPUT);
  }
  delay(100);
  delayMicroseconds(15);
  lcdCmdWrite(0x38);
  delayMicroseconds(5);
  lcdCmdWrite(0x38);
  delayMicroseconds(5);
  lcdCmdWrite(0x38);
  delayMicroseconds(5);


  lcdCmdWrite(0x06);  // 设置光标自动右移动，文字不移   
  delay(10);                     
  lcdCmdWrite(0x01);  // 清屏               
  delay(10); 
}

void setup() {
  setupLCD(2, 3);
}

void loop() {
  lcdCmdWrite(0x01);  // 清屏
  delay(10);                
  lcdCmdWrite(0x80+5);  // 光标移动5个格子
     
  // 写入欢迎信息 
  lcdDataWrite('H');
  lcdDataWrite('I');
  lcdDataWrite('!');
  delay(3000);

  lcdCmdWrite(0xc0+5);  // 第二行,光标移动5个格子
  delay(10);                
  lcdDataWrite('1');
  lcdDataWrite('6');
  lcdDataWrite('0');
  lcdDataWrite('2'); 
  lcdDataWrite('!'); 
  delay(2000);
  lcdCmdWrite(0x01);  // 清屏
  delay(1000);
}
