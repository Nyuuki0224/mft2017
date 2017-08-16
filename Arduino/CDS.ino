// ヘッダファイル指定　Including header files
#include <SakuraIO.h>
SakuraIO_I2C sakuraio;

// 照度センサの定義　Definition of illuminance sensor
//const int cdsPin = A0;
#define cdsPin A0

// 変数の定義　Definition of variables
int cdsAnalogVal;
uint32_t cdsSimpleVal = 0;
uint32_t cnt = 0;
uint32_t sendFlg = 0;
String msg = "";

// 起動時に1回だけ実行　Run once at startup
void setup() {
  Serial.begin(9600);

  // オンライン状態に遷移するまで待機　Wait until transition to online state
  Serial.print("Waiting to come online");
  for(;;){
    if( (sakuraio.getConnectionStatus() & 0x80) == 0x80 ) break;
    Serial.print(".");
    delay(1000);
  }
}

// 以下ループ実行　Loop execution
void loop() {

  // cnt値のカウントアップ　Count up cnt value
  cnt++;
  Serial.println(cnt);

  // 照度情報の取得と変換　Get and Convert illminance
  cdsAnalogVal = analogRead(cdsPin);
  cdsSimpleVal = map(cdsAnalogVal, 0, 350, 0, 100);

  // 閾値判定　Threshold determination
  if(cdsSimpleVal > 10){
    msg = "Lighted : ";
  }else{
    msg = "Darken : ";
  }
  Serial.print(msg);
  Serial.println(cdsSimpleVal);

  sakuraio.enqueueTx(0, cnt);
  sakuraio.enqueueTx(1, cdsSimpleVal);
  sakuraio.send();
  delay(100);

  // 利用可能な領域（Available）とデータが格納された領域（Queued）の取得　Get Available and Queued
  uint8_t available;
  uint8_t queued;
  if (sakuraio.getTxQueueLength(&available, &queued) != CMD_ERROR_NONE) {
    Serial.println("[ERR] get tx queue length error");
  }
  Serial.print("Available :");
  Serial.print(available);
  Serial.print(" Queued :");
  Serial.println(queued);
  Serial.println("");

  delay(900);
}