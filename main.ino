#define LED 11
#define OUT 12
#define BUTTON 3
#define IN A0
//定义一些方便查找的宏
typedef long long LL;

int reading = analogRead(IN);
LL lastDebounceTime = 0;  // 上次按键触发时间
LL debounceDelay = 3500; // 去抖时间
int lastButtonState = LOW;   // 上次读取的按键状态
int buttonState; 
int freequencyData [256];
int rhythmData [256];
int dataLenth=0;
int freequencyDataNow;

void saveData(int freequencyData);
void play();

void setup() {
  Serial.begin(9600);
  pinMode(A0,INPUT);
  pinMode(3,INPUT);
  pinMode(12,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  digitalWrite(4,HIGH);
  digitalWrite(5,LOW);
  //引脚设置
  digitalWrite(LED,HIGH);
}

void loop() {
  // 读取按键状态赋值到一个变量

  // 如果按键状态变化，不过是由于抖动还是按键按下造成的
  // 下面的程序都需要进行这一步处理
  if((millis()-lastDebounceTime)>=5000){
    saveData(freequencyDataNow);
    digitalWrite(LED,LOW);
    delay(250);
    digitalWrite(LED,HIGH);
    play(); 
    lastDebounceTime = millis();
    Serial.println(0);
  }
  
  
  noTone(OUT);
  if(analogRead(IN) <= 650){
    freequencyDataNow = 0;
  }
  else{
    freequencyDataNow = map(analogRead(IN),650,1000,1000,1500);
    tone(OUT,freequencyDataNow,10);
  }
  delay(12);
  // 保存当前值，一边下一个循环检测的时候使用

}

void saveData(int saveFreequencyData){
  if(dataLenth>=250){
    for(int i;i<256;i++){
      freequencyData[i] = 0;
      rhythmData[i] = 0;
      dataLenth = 0;
    }
  }

  if(dataLenth==0){
    freequencyData[0] = saveFreequencyData;
    rhythmData[0] = 500;
    dataLenth++;
    return;
  }

  if(abs(saveFreequencyData-freequencyData[dataLenth-1])<10){
      rhythmData[dataLenth-1] += 500;
      dataLenth++;
      return;
  }

  freequencyData[dataLenth] = saveFreequencyData;
  rhythmData[dataLenth] = 500;
  dataLenth++;
}

void play(){
  int lenth = dataLenth-1;
  int i=0;
  while(lenth--){ 
      if(freequencyData[i]==0){
        delay(rhythmData[i]);
        i++;
        continue;
      }
      tone(OUT,freequencyData[i],rhythmData[i]);
      delay(rhythmData[i]);
      i++;
  }
}