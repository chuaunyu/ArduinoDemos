#define LED_PIN 13
#define DHT11_PIN 2

#define SUC 1
#define ERROR_READ -1
#define ERROR_SUM -2

long data = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
}

long read() {
  unsigned char checksum = 0;
  
  int sum = 0;
  int i=0;
  data = 0;
  pinMode(DHT11_PIN, OUTPUT);
  
  // Set LOW to gpio 18ms
  digitalWrite(DHT11_PIN, LOW);
  delay(25);

  // Set HIGH to gpio 40us
  digitalWrite(DHT11_PIN, HIGH);
  pinMode(DHT11_PIN, INPUT);
  delayMicroseconds(40);

  // Waitting for DHT11 response------------------

  unsigned int loopCnt = 10000;
  
  // Receive a LOW signal
  if(digitalRead(DHT11_PIN) == LOW){
      // Receive a HIGH signal
      while(digitalRead(DHT11_PIN) != HIGH){
        if (loopCnt-- == 0){
            Serial.println("ERR.1");
            return ERROR_READ;     
        }
     }
        
    
      // Read 4*8bit=32bit data -->  8bit temeprature int + 8bit temperature decimal + 8bit humidity int + 8bit humidity decimal
      for(i=0;i<32;i++){
        loopCnt = 10000;
        // Receive a HIGH signal --> Receive a LOW signal --> Wait for 28 us
        while(digitalRead(DHT11_PIN) != LOW){
            if (loopCnt-- == 0){
                return ERROR_READ;
            }
        }
           

        loopCnt = 10000;
        while(digitalRead(DHT11_PIN) != HIGH){
            if (loopCnt-- == 0){
                return ERROR_READ;
            }
        }
          
        delayMicroseconds(32);
      
        data*=2;
        if(digitalRead(DHT11_PIN) == 1){
          data++;
        } 
      }
  
      // Read 8bit check sum
      for(i=0;i<8;i++){
        // Receive a HIGH signal --> Receive a LOW signal --> Wait for 28 us
        loopCnt = 10000;
        // Receive a HIGH signal --> Receive a LOW signal --> Wait for 28 us
        while(digitalRead(DHT11_PIN) != LOW){
            if (loopCnt-- == 0){
                return ERROR_READ;
            }
        }
           

        loopCnt = 10000;
        while(digitalRead(DHT11_PIN) != HIGH){
            if (loopCnt-- == 0){
                return ERROR_READ;
            }
        }
        delayMicroseconds(32);
      
        checksum*=2;
        if(digitalRead(DHT11_PIN) == 1){
          checksum++;
        }
      }

      // Valid checksum---------------------
      sum += (data>>24)&0xff;
      sum += (data>>16)&0xff;
      sum += (data>>8)&0xff;
      sum += data&0xff;
      sum = sum&0xff;   
  
      if(sum != checksum){
        return ERROR_SUM;
      }
      return SUC;
    }else{
      return ERROR_READ;
    }
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  long val = read();
  if(val != SUC){
    Serial.println("{\"success\":false}");
  }else{
    Serial.print("{\"success\":true, \"data\":{\"Humidity\":");
    Serial.print((data>>24)&0xff);
    Serial.print(".");
    Serial.print((data>>16)&0xff);
    Serial.print(", \"Temperature\":");
    Serial.print((data>>8)&0xff);
    Serial.print(".");
    Serial.print(data&0xff);
    Serial.print("}}\n");
  }
  digitalWrite(LED_PIN, LOW);
  delay(2000); // sleep 2s
}
