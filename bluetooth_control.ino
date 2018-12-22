
#include <SoftwareSerial.h>
#include <Servo.h>

Servo s[4] ; //定义舵机号
#define servo1 4    //右臂
#define servo2 5    //底座
#define servo3 6    //夹具
#define servo4 7    //前臂
int servostep=2;     //灵敏度，操控每一步移动角度
/*以下设置每个舵机参数，可以根据需要修改{a,b,c,d,e}
 * a 舵机复位初始角度 
 * b 舵机运行最小角度 
 * c 舵机运行最大角度  
 */
int servo_set[4][3]={
    {90,50,150},//右臂
    {90,22,168},//底座
    {90,36,105},//夹具
    {90,32,110},//前臂  
};
unsigned long servo_millis[4]={0,0,0,0};       //初始化舵机运行时间
int cur_position[4]={servo_set[0][0],servo_set[1][0],servo_set[2][0],servo_set[3][0]}; 
int tag_position[4]={servo_set[0][0],servo_set[1][0],servo_set[2][0],servo_set[3][0]}; 
SoftwareSerial mySerial(11,10);
bool loop_stu=true;
int snmm;
void setup() {                                 //设置程序（运行一次）
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(servo1,OUTPUT);pinMode(servo2,OUTPUT);pinMode(servo3,OUTPUT);pinMode(servo4,OUTPUT);
  s[0].attach(servo1);s[1].attach(servo2);s[2].attach(servo3);s[3].attach(servo4);
  s[0].write(servo_set[0][0]); //舵机复位
  s[1].write(servo_set[1][0]); //舵机复位
  s[2].write(servo_set[2][0]); //舵机复位
  s[3].write(servo_set[3][0]); //舵机复位
  while(!Serial);
  while(Serial.read() >= 0);
}

void loop(){
char send_char;
while(mySerial.available()) send_char= mySerial.read();
moveservo(send_char); 
delay(20);
}

void moveservo(char ttt){                      //移动舵机函数(sn舵机号，tag_position目标角度)
  int sn;
  switch (ttt){
    case '1':  sn=1;tag_position[sn]=servo_set[sn][2];    break; //按下“<”按钮
    case '2':  sn=0;tag_position[sn]=servo_set[sn][2];    break; //按下“∧”按钮
    case '3':  sn=1;tag_position[sn]=servo_set[sn][1];    break; //按下“>”按钮
    case '4':  sn=0;tag_position[sn]=servo_set[sn][1];    break; //按下“∨”按钮
    case '5':  sn=3;tag_position[sn]=servo_set[sn][2];    break; //按下“□”按钮
    case '6':  s[2].write(servo_set[2][2]);    break; //按下“△”按钮
    case '7':  sn=3;tag_position[sn]=servo_set[sn][1];    break; //按下“×”按钮
    case '8':  s[2].write(servo_set[2][1]);    break; //按下“○”按钮
    case 'c':  break;             //按下“SELECT”按钮切换连续模式
    case 'd':  break;              //按下“START”暂停舵机运动
    default:   sn=5;break;
  }

if(sn<4){  
  snmm=sn;
  if (loop_stu) {
     for(int i=0;i<4;i++){
        tag_position[i]=cur_position[i];
     }
    }
     loop_stu=!loop_stu;
}      
if (abs( tag_position[snmm]-cur_position[snmm])>=servostep &&  loop_stu){
     cur_position[snmm]=tag_position[snmm]>cur_position[snmm]?cur_position[snmm]+=servostep:cur_position[snmm]-=servostep;
     cur_position[snmm]=cur_position[snmm]<servo_set[snmm][1]?servo_set[snmm][1]:cur_position[snmm];
     cur_position[snmm]=cur_position[snmm]>servo_set[snmm][2]?servo_set[snmm][2]:cur_position[snmm];      //移动不超出舵机设定范围
     s[snmm].write(cur_position[snmm]);
delay(10);
  } 
}    



