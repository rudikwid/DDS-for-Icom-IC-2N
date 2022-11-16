//#include <LiquidCrystal_SR.h>

#include <LiquidCrystal_I2C.h>
#include <Encoder.h>
#include <EEPROM.h>
// EEPROM mapping-> 0->lastsetting,1-2->last RX,3-4->last TX/Freq duplex
// 5->memory count, 6-...->memory save, each channel contain 4byte RX&TX channel
//LiquidCrystal_SR lcd(18,19,TWO_WIRE);


LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display


Encoder myEnc(14, 15);

#define  S8    13
#define  S4    12
#define  S2    11
#define  S1    10
#define  B8    9
#define  B4    8
#define  B2    7
#define  B1    6
#define  C8    5
#define  C4    4
#define  C2    3
#define  C1    2
#define  enc_a A0
#define  enc_b A1
#define  key   A2
#define  smtr  A3

int ratusan = 0;
int puluhan = 0;
int satuan = 0;
word rxch_num =0;
word txch_num =0;
int mem_num = 0;
int num = 1;
int addr = 0;
int datal =0;
int datah = 0;
byte lastset = 0;      //bit0->sim/dup, bit1->vfo/mem, bit2->last channel
byte ant[8] = {0b10001,0b01010,0b00100,0b10001,0b01010,0b00100,0b00100,0b00100};
byte signal1[8] = {0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b11111,0b11111};
byte signal2[8] = {0b00000,0b00000,0b00000,0b00000,0b00000,0b11111,0b11111,0b11111};
byte signal3[8] = {0b00000,0b00000,0b00000,0b00000,0b11111,0b11111,0b11111,0b11111};
byte signal4[8] = {0b00000,0b00000,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111};
byte signal5[8] = {0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111};
boolean duplex = false;
boolean memory = false;
boolean menu = false;
boolean func = false;
boolean dupMnu = false;
boolean encDup = false;
boolean encMem = false;
boolean memMnu = false;
long oldPosition  = -999;

void setup(){
  //lcd.begin(16,2);
lcd.init();
lcd.backlight();
  lcd.createChar (0, ant);
  lcd.createChar (1, signal1);
  lcd.createChar (2, signal2);
  lcd.createChar (3, signal3);
  lcd.createChar (4, signal4);
  lcd.createChar (5, signal5);
  pinMode(S8,OUTPUT);
  pinMode(S4,OUTPUT);
  pinMode(S2,OUTPUT);
  pinMode(S1,OUTPUT);
  pinMode(B8,OUTPUT);
  pinMode(B4,OUTPUT);
  pinMode(B2,OUTPUT);
  pinMode(B1,OUTPUT);
  pinMode(C8,OUTPUT);
  pinMode(C4,OUTPUT);
  pinMode(C2,OUTPUT);
  pinMode(C1,OUTPUT);
  pinMode(enc_a,INPUT);
  digitalWrite(enc_a,HIGH);
  pinMode(enc_b,INPUT);
  digitalWrite(enc_b,HIGH);
  myEnc.write(10);
  Serial.begin(9600);
  lastset = EEPROM.read(0);      // read last setting dup/simp, vfo/mem
  duplex = bitRead(lastset,0);
  memory = bitRead(lastset,1);
  datal = EEPROM.read(1);
  datah = EEPROM.read(2);
  rxch_num = word(datah, datal);
  datal = EEPROM.read(3);
  datah = EEPROM.read(4);
  txch_num = word(datah, datal);
  mem_num = EEPROM.read(5); 
  delay(1000);
  Serial.println("FP2N Ready...");
  Serial.print("lastset :");
  Serial.print(rxch_num,HEX);
  Serial.print(" ");
  Serial.print(datal,HEX);
  Serial.print(" ");
  Serial.print(datah,HEX);
  Serial.print(" ");
  Serial.println(lastset,BIN);
  lcd.print("FP2N controller");
  delay(1000);
  lcd.clear();
  lcd.setCursor ( 0, 0 );
  if(memory == false){
    lcd.print("VFO RX:14");
  }
  else{
    lcd.print("MEM RX:14");
  }
  lcd.setCursor ( 12, 0 );
  lcd.print(" Mhz");
  lcd.setCursor ( 0, 1 );
  if(duplex == false){
    lcd.print("[SIM]");
  }
  else{
    lcd.print("[DUP]");
  }
  lcd.setCursor ( 6, 1 );
  lcd.print(char(0));
  lcd.print(char(1));
  lcd.print(char(2));
  lcd.print(char(3));
  lcd.print(char(4));
  lcd.print(char(5));
  set_ch(rxch_num);
}

void loop(){
  
  
  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    //Serial.println(newPosition);
    if(newPosition > 13){
      myEnc.write(10);
      if(encDup == false){
        rxch_num = rxch_num + num;
        if (rxch_num > 999){
          rxch_num = 999;
        }
        Serial.println(rxch_num);
        set_ch(rxch_num);
      }
      else{
        txch_num++;
        if (txch_num > 999){
          txch_num = 999;
        }
        Serial.println(txch_num);
        lcd.setCursor ( 5, 1 );
        if(txch_num < 10){
          lcd.print("DUP-00");
          lcd.setCursor ( 11, 1 );
          lcd.print(txch_num);
        }
        if(txch_num > 10 & txch_num < 100){
          lcd.print("DUP-0");
          lcd.setCursor ( 10, 1 );
          lcd.print(txch_num);
        }
        if(txch_num > 100){
          lcd.print("DUP-");
          lcd.setCursor ( 9, 1 );
          lcd.print(txch_num);
        }
      }
    }
    if(newPosition < 7){
      myEnc.write(10);
       if(encDup == false){
        rxch_num = rxch_num - num;
        if (rxch_num > 999){
          rxch_num = 999;
        }
        Serial.println(rxch_num);
        set_ch(rxch_num);
      }
      else{
        txch_num--;
        if (txch_num > 999){
          txch_num = 999;
        }
        Serial.println(txch_num);
        lcd.setCursor ( 5, 1 );
        if(txch_num < 10){
          lcd.print("DUP-00");
          lcd.setCursor ( 11, 1 );
          lcd.print(txch_num);
        }
        if(txch_num > 10 & txch_num < 100){
          lcd.print("DUP-0");
          lcd.setCursor ( 10, 1 );
          lcd.print(txch_num);
        }
        if(txch_num > 100){
          lcd.print("DUP-");
          lcd.setCursor ( 9, 1 );
          lcd.print(txch_num);
        }
      }
    }
  }
  
  // Inisiasi key button
  // button TX = 124 - 115  menjadi 150-140
  // button DUP = 55 - 50  menjadi 335-325
  // button Memory = 103 - 99 menjadi 512-497
  // button Function = 150 - 140 - menjadi 746-736
  
  int buttonValue = analogRead(key);
  if (buttonValue < 1000) {
    delay(100);
    if (buttonValue < 150 && buttonValue > 140 ){
      lcd.setCursor ( 4, 0 );
      lcd.print("TX");
      if(duplex == true){
        set_ch(txch_num);
      }
      while (buttonValue < 1000){
        buttonValue = analogRead(key);
      }
      if(duplex == true){
        
        set_ch(rxch_num);
      }
      lcd.setCursor ( 4, 0 );
      lcd.print("RX");
    }
    if (buttonValue < 335 && buttonValue > 325 ){
      if(dupMnu == false){
        if(duplex == false){
          Serial.println("duplex");
          duplex = true;
          lcd.setCursor ( 0, 1 );
          lcd.print("[DUP]");
          bitSet(lastset,0);
          EEPROM.write(0, lastset);
        }
        else{
          Serial.println("simplex");
          duplex = false;
          lcd.setCursor ( 0, 1 );
          lcd.print("[SIM]");
          bitClear(lastset,0);
          EEPROM.write(0, lastset);
        }
      }
      else{
        Serial.println("duplex menu");
        encDup = true;
        lcd.setCursor ( 5, 1 );
        if(txch_num < 10){
          lcd.print("DUP-00");
          lcd.setCursor ( 11, 1 );
          lcd.print(txch_num);
        }
        if(txch_num > 10 & txch_num < 100){
          lcd.print("DUP-0");
          lcd.setCursor ( 10, 1 );
          lcd.print(txch_num);
        }
        if(txch_num > 100){
          lcd.print("DUP-");
          lcd.setCursor ( 9, 1 );
          lcd.print(txch_num);
        }
      }
      delay(100);
      while (buttonValue < 1000){
        buttonValue = analogRead(key);
      }
    }
    if (buttonValue < 512 && buttonValue > 497 ){
      if(memMnu == false){
        if(memory == false){
          Serial.println("Memory");
          memory = true;
          lcd.setCursor ( 0, 0 );
          lcd.print("MEM");
          bitSet(lastset,1);
          EEPROM.write(0, lastset);
        }
        else{
          Serial.println("VFO");
          memory = false;
          lcd.setCursor ( 0, 0 );
          lcd.print("VFO");
          bitClear(lastset,1);
          EEPROM.write(0, lastset);
        }
      }
      else{
        lcd.setCursor ( 5, 1 );
        lcd.print("MEMSAVE:");
      }  
      delay(100);
      while (buttonValue < 1000){
        buttonValue = analogRead(key);
      }
    }
    if (buttonValue < 748 && buttonValue > 738){
      if(func == false){
        Serial.println("function");
        func = true;
        dupMnu = true;
        memMnu = true;
        num = 100;
        lcd.setCursor ( 0, 1 );
        lcd.print("[FUN]");
      }
      else{
        Serial.println("back");
        func = false;
        dupMnu = false;
        encDup = false;
        memMnu = false;
        num = 1;
        lcd.setCursor ( 0, 1 );
        if(duplex == false){
          lcd.print("[SIM]");
        }
        else{
          lcd.print("[DUP]");
        }
        signalBar();
        datal = lowByte(txch_num);
        datah = highByte(txch_num);
        EEPROM.write(3, datal);
        delay(10);
        EEPROM.write(4, datah);
      }  
      while (buttonValue < 1000){
        buttonValue = analogRead(key);
      }
    }
  }
}

void signalBar(){
  lcd.setCursor ( 5, 1 );
  lcd.print(" ");
  lcd.print(char(0));
  lcd.print(char(1));
  lcd.print(char(2));
  lcd.print(char(3));
  lcd.print(char(4));
  lcd.print(char(5));
}

void set_ch(int channel){
  if(channel < 0){
    channel = 0;
  }
  if (channel > 999){
    channel = 999;
  }
  int temp = channel % 100;
  ratusan = (channel - temp)/100;
  int temp2 = temp % 10;
  puluhan = (temp - temp2)/10;  
  satuan = temp2;
  /*Serial.print(ratusan);
  Serial.print(" ");
  Serial.print(puluhan);
  Serial.print(" ");
  Serial.println(satuan);*/
  lcd.setCursor ( 9, 0 );
  lcd.print(ratusan);
  lcd.print(",");
  lcd.print(puluhan);
  lcd.print(satuan);
  digitalWrite(S8,bitRead(satuan, 3));
  digitalWrite(S4,bitRead(satuan, 2));
  digitalWrite(S2,bitRead(satuan, 1));
  digitalWrite(S1,bitRead(satuan, 0));
  digitalWrite(B8,bitRead(puluhan, 3));
  digitalWrite(B4,bitRead(puluhan, 2));
  digitalWrite(B2,bitRead(puluhan, 1));
  digitalWrite(B1,bitRead(puluhan, 0));
  digitalWrite(C8,bitRead(ratusan, 3));
  digitalWrite(C4,bitRead(ratusan, 2));
  digitalWrite(C2,bitRead(ratusan, 1));
  digitalWrite(C1,bitRead(ratusan, 0));
  datal = lowByte(channel);
  datah = highByte(channel); 
  Serial.print(channel,HEX);
  Serial.print(" ");
  Serial.print(datal,HEX);
  Serial.print(" ");
  Serial.println(datah,HEX);
  EEPROM.write(1, datal);
  delay(10);
  EEPROM.write(2, datah);
}
