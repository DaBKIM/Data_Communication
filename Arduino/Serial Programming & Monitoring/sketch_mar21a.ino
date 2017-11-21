const int ledPin=12;
int blinkRate=0;

void setup(){
  
  Serial.begin(9600);//init serial port
  pinMode(ledPin,OUTPUT);//set ledPin by OUTPUT
}

int ck=0;

void loop(){

  int tmp=0;
 
  while(Serial.available()){
   
   delay(3);
   char ch=Serial.read();
   
   if(isDigit(ch)){
     
     tmp=tmp*10+(ch-'0');
      //blinkRate=blinkRate*100;
      ck=tmp;
   }
  }
  blink();
}

//blink the led according to the blinkRate
void blink(){
  
  digitalWrite(ledPin,HIGH);//led on
  delay(ck);  //wait for blinkRate time
  digitalWrite(ledPin,LOW);//led off
  delay(ck);  
}
