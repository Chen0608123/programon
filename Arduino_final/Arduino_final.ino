// ==========================================
// 專案：Unity 與 Arduino 體感硬體整合
// 硬體：晃動馬達 x1, 風扇 x1, 噴霧 x1, 致冷晶片 x2, 震動馬達 x1
// ==========================================

// --- 硬體腳位定義 ---
// 1. 晃動馬達 (需使用 L298N 等馬達驅動板)
const int motorSpeedPin = 10; // PWM 速度控制腳位
const int motorDirPin = 12;   // 轉向控制腳位

// 2. 風扇 (需使用 L298N 等馬達驅動板)
const int fanSpeedPin = 11;   // PWM 速度控制腳位
const int fanDirPin = 13;     // 轉向控制腳位

// 3. 噴霧模組
const int sprayRelayPin = 8;  // 繼電器控制腳位

// 4. 致冷晶片 (分為 1 號與 2 號，獨立控制)
const int peltierRelayPin1 = 7; // 致冷晶片 1 號的繼電器
// const int peltierRelayPin2 = 6; // 致冷晶片 2 號的繼電器

// 5. 震動馬達 (新增)
const int vibPin = 5;        

// ==========================================
// 系統初始化 (Setup)
// ==========================================
void setup() {
  // 啟動序列埠通訊，Baud Rate 設為 9600 (Unity 端連線時也必須設定為 9600)
  Serial.begin(9600);

  // 設定所有使用到的腳位為輸出模式 (OUTPUT)
  pinMode(motorSpeedPin, OUTPUT);
  pinMode(motorDirPin, OUTPUT);
  pinMode(fanSpeedPin, OUTPUT);
  pinMode(fanDirPin, OUTPUT);
  pinMode(sprayRelayPin, OUTPUT);
  pinMode(peltierRelayPin1, OUTPUT);
  //pinMode(peltierRelayPin2, OUTPUT);
  pinMode(vibPin, OUTPUT); // 新增震動馬達腳位設定

  // 初始安全狀態：確保程式剛啟動時，所有設備都是關閉的
  digitalWrite(sprayRelayPin, LOW);
  digitalWrite(peltierRelayPin1, LOW);
  //digitalWrite(peltierRelayPin2, LOW);
  digitalWrite(vibPin, LOW); // 確保震動馬達初始為關閉
  analogWrite(motorSpeedPin, 0);
  analogWrite(fanSpeedPin, 0);
  
  Serial.println("=== 系統初始化完成，等待 Unity 指令 ===");
}

// ==========================================
// 主程式迴圈 (Loop)
// ==========================================
void loop() {
  // 檢查序列埠是否有收到來自 Unity 的指令
  if (Serial.available() > 0) {
    // 讀取一個字元作為指令
    char command = Serial.read(); 

    // 根據收到的字元執行對應的硬體動作
    switch (command) {
      
      // -----------------------------
      // 晃動馬達控制指令
      // -----------------------------
      case 'Q': // [Q] 馬達正轉 (PWM 速度 150)
        digitalWrite(motorDirPin, HIGH);
        analogWrite(motorSpeedPin, 150);
        delay(1000);
        analogWrite(motorSpeedPin, 0);
        break;
      
      case 'W': // [W] 馬達反轉 (PWM 速度 220)
        digitalWrite(motorDirPin, LOW);
        analogWrite(motorSpeedPin, 220);
        delay(1000);
        analogWrite(motorSpeedPin, 0);
        break;

      // -----------------------------
      // 風扇控制指令
      // -----------------------------
      case 'A': // [A] 風扇開啟 (PWM 全速 255)
        digitalWrite(fanDirPin, HIGH);
        analogWrite(fanSpeedPin, 255);
        break;
      case 'S': // [S] 風扇停止
        analogWrite(fanSpeedPin, 0);
        break;

      // -----------------------------
      // 噴霧控制指令
      // -----------------------------
      case 'Z': // [Z] 噴霧開啟
        digitalWrite(sprayRelayPin, HIGH);
        break;
      case 'X': // [X] 噴霧關閉
        digitalWrite(sprayRelayPin, LOW);
        break;

      // -----------------------------
      // 致冷晶片 1 號控制指令
      // -----------------------------
      case 'C': // [C] 開啟 1 號致冷晶片
        digitalWrite(peltierRelayPin1, HIGH);
        break;
      case 'V': // [V] 關閉 1 號致冷晶片
        digitalWrite(peltierRelayPin1, LOW);
        break;

      // -----------------------------
      // 震動馬達控制指令 (新增)
      // -----------------------------
      case 'B': // [B] 開啟震動馬達
        digitalWrite(vibPin, HIGH);
        Serial.println("震動中...");
        delay(1000);
        digitalWrite(vibPin, LOW);
        Serial.println("停止震動");
        break;

      // case 'N': // [N] 關閉震動馬達
      //   digitalWrite(vibPin, LOW);
      //   Serial.println("停止震動");
      //   break;
    }
  }
}