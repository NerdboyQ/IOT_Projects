#include <SoftwareSerial.h>
#define RX 10 // Connect to HM10 TX for both AT and normal modes
#define TX 11  // Connect to HM10 RX for both AT and normal modes
SoftwareSerial HM10(RX,TX);


bool sendAtCmd(char* cmd) {
  Serial.write("========================\n\n");
  Serial.write("Cmd:");
  Serial.write(cmd);
  Serial.write("\n");
  HM10.write(cmd);
  
  if (String(cmd).indexOf("DISC") > 0) delay(3500); // default scan time is 3s
  else delay(500);
  while (HM10.available()) {
    Serial.write(HM10.read());
  }
  Serial.write("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  return 1;
}

void HM10xmtr_setup() {
  sendAtCmd("AT");  // Verifies solid UART communication if not actively connected, otherwise kills current connection
  sendAtCmd("AT+NAMELEDXmtr");  // Sets the BT Device Advertised name
  sendAtCmd("AT+NAME?");  // Gets the BT Device Advertised name
  sendAtCmd("AT+ADDR?");  // Gets the BT Device BT Address
  sendAtCmd("AT+BAUD?");  // Gets the Currently set baudrate 
  sendAtCmd("AT+ROLE?");  // Gets the Currently set role (0: slave/peripheral, 1: master/central) 
  sendAtCmd("AT+IMME?"); // Get Module Work type (only used for central/master mode) - 0: start work immediately, 1: only response to AT commands
  sendAtCmd("AT+IMME1"); // Set Module Work type (only used for central/master mode) - 0: start work immediately, 1: only response to AT commands
  sendAtCmd("AT+IMME?");
  sendAtCmd("AT+ALLO1");  // Set Whitelist enabled mode; Enabled (1), Disable (0)
  sendAtCmd("AT+ALLO?");  // Get Whitelist enabled mode; Enabled (1), Disable (0)
  sendAtCmd("AT+AD10035FF0D419B");  // Set Whitelisted device 1 to 0035FF0D419B
  sendAtCmd("AT+AD1?");  // Get Whitelisted device 1
  sendAtCmd("AT+SCAN?");  // Get scan delay time
  // sendAtCmd("AT+SCAN5");  // Set scan delay time to 5s (1-9s)
  // sendAtCmd("AT+AD2?");  // Get Whitelisted device 2
  // sendAtCmd("AT+AD3?");  // Get Whitelisted device 3
  // sendAtCmd("AT+SHOWB?");  // Show Whitelist device address
  // sendAtCmd("AT+PASS?");  // Get Device Pairing Pin
  // sendAtCmd("AT+DISI?"); // Discovery for iBeacons
  // sendAtCmd("AT+DISC?");  // Start a device discovery scan
  sendAtCmd("AT+CON0035FF0D419B"); // attempt to connect to device 0035FF0D419B
  delay(5000);
  sendAtCmd("AT");
  sendAtCmd("AT+DISC");
  sendAtCmd("AT+RESET");  // Reset BT Device
}

void HM10rcvr_setup() {
  sendAtCmd("AT");  // Verifies solid UART communication if not actively connected, otherwise kills current connection
  sendAtCmd("AT+NAMEBT-LED-00");  // Sets the BT Device Advertised name
  sendAtCmd("AT+NAME?");  // Gets the BT Device Advertised name
  sendAtCmd("AT+BAUD?");  // Gets the Currently set baudrate 
  sendAtCmd("AT+ADDR?");  // Gets the BT Device BT Address
  sendAtCmd("AT+ROLE?");  // Gets the Currently set role (0: slave/peripheral, 1: master/central) 
  sendAtCmd("AT+ALLO1");  // Set Whitelist enabled mode; Enabled (1), Disable (0)
  sendAtCmd("AT+ALLO?");  // Get Whitelist enabled mode; Enabled (1), Disable (0)
  sendAtCmd("AT+AD1B0D2786E300A");  // Set Whitelisted device 1 to B0D2786E300A
  sendAtCmd("AT+AD2B0D2786E300A");  // Set Whitelisted device 2 to B0D2786E300A
  sendAtCmd("AT+AD3B0D2786E300A");  // Set Whitelisted device 3 to B0D2786E300A
  sendAtCmd("AT+AD1?");  // Get Whitelisted device 1
  sendAtCmd("AT+AD2?");  // Get Whitelisted device 2
  sendAtCmd("AT+AD3?");  // Get Whitelisted device 3
  // sendAtCmd("AT+SCAN5");  // Set scan delay time to 5s (1-9s)
  // sendAtCmd("AT+AD2?");  // Get Whitelisted device 2
  // sendAtCmd("AT+AD3?");  // Get Whitelisted device 3
  // sendAtCmd("AT+SHOWB?");  // Show Whitelist device address
  sendAtCmd("AT+PASS?");  // Get Device Pairing Pin
  sendAtCmd("AT+RESET");  // Reset BT Device
}

void setup() {
  HM10.begin(9600);
  Serial.begin(9600);
  while (!Serial) {
    HM10.println("Connect Serial Commnication");
  }
  // HM10xmtr_setup();
  HM10rcvr_setup();
}



bool msgRcv = false;
void loop() {
  while (HM10.available()) {
    msgRcv = true;
    delay(100);
    Serial.write(HM10.read());
  }
  if (msgRcv) {
    Serial.write("\n~\n");
    msgRcv = false;
  }
}