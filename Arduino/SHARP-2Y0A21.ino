// ヘッダファイル指定　Including header files
#include <SakuraIO.h>
SakuraIO_I2C sakuraio;

// 変数の定義　Definition of variables
uint32_t cnt = 0;

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

  // https://blogs.yahoo.co.jp/nobita_rx7/27459807.html
  float len = 19750.0 * pow(analogRead(0), -1.25);
  Serial.print(len);
  Serial.println("cm");

  sakuraio.enqueueTx(0, cnt);
  sakuraio.enqueueTx(1, len);
  sakuraio.send();
  delay(1000);

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

 delay(4000);
}