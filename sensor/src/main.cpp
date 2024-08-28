#include "HT_SSD1306Wire.h"
#include "LoRaWan_APP.h"
#include "Wire.h"
#include <Arduino.h>

// Parser for GPS strings.
// TinyGPSPlus gps;
// UART connection to the GPS module.
// HardwareSerial gpsSerial(1);

// Pin for receiving data from GPS.
// const int GPS_TX_PIN = 45;

// How long to sleep after a packet has been transmitted before
// sending the next packet.
const int SLEEP_TIME_BETWEEN_EVENTS_MS = 10000;

// Use Over the Air Activation for joining the LoRaWAN network.
const bool LORA_OTAA = true;

// For OTAA.
uint8_t devEui[8] = {0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x06, 0x6F, 0xCD};
uint8_t joinEui[] =  {0x00, 0x50, 0x8E, 0x04, 0xE0, 0xBB, 0x0E, 0x30}; // you should set whatever your TTN generates. TTN calls this the joinEUI, they are the same thing. 
uint8_t appKey[] =   {0x57, 0x12, 0xEA, 0xED, 0xC9, 0xD6, 0xBC, 0x44, 0x6B, 0x4E, 0x4C, 0x0F, 0xAC, 0xF7, 0xC4, 0x22};// you should set whatever your TTN generates 
// These are only used for ABP.
uint8_t nwkSKey[16] = {0};
uint8_t appSKey[16] = {0};
uint32_t devAddr    = 0;

// LoRaWAN channel mask
uint16_t userChannelsMask[6] = {0xFF00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};

// ADR enable
bool loraWanAdr = true;

// Application port
uint8_t appPort = 1;

/*!
 * Number of trials to transmit the frame, if the LoRaMAC layer did not
 * receive an acknowledgment. The MAC performs a datarate adaptation,
 * according to the LoRaWAN Specification V1.0.2, chapter 18.4, according
 * to the following table:
 *
 * Transmission nb | Data Rate
 * ----------------|-----------
 * 1 (first)       | DR
 * 2               | DR
 * 3               | max(DR-1,0)
 * 4               | max(DR-1,0)
 * 5               | max(DR-2,0)
 * 6               | max(DR-2,0)
 * 7               | max(DR-3,0)
 * 8               | max(DR-3,0)
 *
 * Note, that if NbTrials is set to 1 or 2, the MAC will not decrease
 * the datarate, in case the LoRaMAC layer did not receive an acknowledgment
 */
uint8_t confirmedNbTrials = 8;

const int trigPin = 45;
const int echoPin = 46;

// Keep a running counter of how many packets have been sent since reset.
RTC_DATA_ATTR uint16_t count = 0;
// Keep a count of how many packets have been acknowledged since reset.
RTC_DATA_ATTR uint16_t acked_count = 0;

// Whether the GPS has a lock or not.
// RTC_DATA_ATTR bool gps_locked = false;

// Number of unacked packets in a row.
RTC_DATA_ATTR uint16_t failed_tx_consecutive = 0;

// Driver for the OLED display.
//
// THIS MUST BE DEFINED LAST. (I don't know why.)
SSD1306Wire display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED);


///////////////////////////////////////////////////////////////////////////////
// Display Functions
///////////////////////////////////////////////////////////////////////////////


// Display on the OLED that a packet was sent and if it was acked.
void display_tx_done(uint8_t tries, bool acked) {
  char cnt_str[25];
  char ack_str[25];

  snprintf(cnt_str, 25, "TX Count: %i", tries);
  snprintf(ack_str, 25, "Ack: %s", acked?"Yes":"No");

  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(4, 5, "Packet Sent");
  display.setFont(ArialMT_Plain_10);
  display.drawString(4, 20, cnt_str);
  display.drawString(4, 30, ack_str);
  display.display();
  delay(4000);
}

// Display LoRaWAN Join and GPS status.
void display_status(bool lora_joined) {
  char msg[32];
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  snprintf(msg, 32, "LoRa: %s", (lora_joined?"Joined":"Joining..."));
  display.drawString(4, 25, msg);
  display.drawString(4, 35, msg);
  snprintf(msg, 32, "Failed Cnt: %i", failed_tx_consecutive);
  display.drawString(4, 45, msg);
  display.display();
  delay(2000);
}

///////////////////////////////////////////////////////////////////////////////
// TX Packet and GPS Functions
///////////////////////////////////////////////////////////////////////////////
float measureDistanceCM() {
	digitalWrite(trigPin,LOW);
	delayMicroseconds(2);
	digitalWrite(trigPin,HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin,LOW);
	float duration = pulseIn(echoPin, HIGH);
	return duration * .0343/2;
}

// Read the GPS and create an outgoing packet structure.
void prepareTxFrame(void) {
  // We are going to send 20 bytes.
  float distance1 = measureDistanceCM();
  // wait 10s
  delay(10000);
  float distance2 = measureDistanceCM();
  
	Serial.printf("Dist: %f cm\n",distance);
	uint32_t dist_bits = *(uint32_t*)(&distance);
  appDataSize = 5;
  appData[0] = 0x0D; // garbage bin ID (1 byte)
	appData[1] = (dist_bits >> 24) & 0xFF;
  appData[2] = (dist_bits >> 16) & 0xFF;
  appData[3] = (dist_bits >> 8) & 0xFF;
  appData[4] = dist_bits & 0xFF;

  for (int i = 0; i < appDataSize; i++) {
    Serial.print(appData[i], HEX);
    Serial.print(" ");
  }
    Serial.println("");
}

static void send_packet(void) {
  prepareTxFrame();

  // Since we are sending a packet, increment our counter.
  count += 1;

  LoRaWAN.send(true, confirmedNbTrials, appPort);
}

///////////////////////////////////////////////////////////////////////////////
// Callback Functions
///////////////////////////////////////////////////////////////////////////////

// Called when we have joined a LoRaWAN network.
static void joined(void) {
  display_status(true);

  send_packet();
}

// Called after a packet has been sent.
static void sent(uint8_t tries, bool acked) {
  printf("Packet Sent. TX count: %i, Acked: %i\r\n", tries, acked);

  // Show status on OLED.
  display_tx_done(tries, acked);

  if (!acked) {
    // Increment our failed counter.
    failed_tx_consecutive += 1;

    // If we have relatively few failures, just try to send again.
    if (failed_tx_consecutive < 5) {
      send_packet();
    } else {
      // If we have many failed packets, try to re-join the network.
      failed_tx_consecutive = 0;
      LoRaWAN.join(LORA_OTAA, true);
    }
  } else {
    // Reset failed counter if needed.
    failed_tx_consecutive = 0;

    // Increment number of packets acked.
    acked_count += 1;

    // We want to wait for a period of time and then prepare and send another
    // packet. To do so in a low power mode, we use the LoRaWAN `cycle()` function
    // which will reboot the ESP32 chip after the desired amount of time.
    //
    // Note, all state is NOT lost in this operation. State marked `RTC_DATA_ATTR`
    // will be preserved, and the LoRaWAN stack uses this extensively. So, when
    // the chip restarts we will still be joined to the LoRaWAN network.
    LoRaWAN.cycle(SLEEP_TIME_BETWEEN_EVENTS_MS);
  
    // send_packet();
  }
}

static void acked(void) {
}

void received(McpsIndication_t *mcpsIndication) {
  printf("+REV DATA:%s", mcpsIndication->RxSlot?"RXWIN2":"RXWIN1");
  printf(", RXSIZE %d", mcpsIndication->BufferSize);
  printf(", PORT %d\r\n", mcpsIndication->Port);
//   printf("+REV DATA:");
//   for (uint8_t i = 0; i < mcpsIndication->BufferSize; i++) {
//     printf("%02X", mcpsIndication->Buffer[i]);
//   }
//   printf("\r\n");
}

///////////////////////////////////////////////////////////////////////////////
// SETUP
///////////////////////////////////////////////////////////////////////////////

void setup() {
  // Configure our normal `printf()` UART.
  Serial.begin(115200);

  // Configure the serial port for the GPS.
  // delay(10); // give the serial port time to initialize

  // Turn off LED.
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 0);

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  // Initialize MCU.
  Mcu.begin();
  display.init();

  // Configure the DEVEUI to be what is hardcoded in this chip.
  // LoRaWAN.generateDeveuiByChipID();

  // Show a title screen.
  if (count == 0) {
    char eui_str[100];
    int eui_str_len = snprintf(eui_str, 100, "%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x",
      devEui[0], devEui[1], devEui[2], devEui[3], devEui[4], devEui[5], devEui[6], devEui[7]);
    eui_str[eui_str_len] = '\0';

    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 5, "Starting...");
    display.drawString(64, 40, "Device EUI:");
    display.drawString(64, 50, eui_str);
    display.setFont(ArialMT_Plain_24);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    // display.drawString(64, 16, "GPSLoRa");
    display.display();
    delay(2000);
  }

  // Initiate the LoRa process. This will check if the connection
  // is already established, or if we are connecting for the first
  // time (after a full power cycle for example).
  LoRaWAN.init(CLASS_A, LORAWAN_ACTIVE_REGION, loraWanAdr, joined, sent, acked, received);
  LoRaWAN.join(LORA_OTAA, false);
}

///////////////////////////////////////////////////////////////////////////////
// LOOP
///////////////////////////////////////////////////////////////////////////////

void loop() {
  // All operations are event based, so all we do here is
  // let the LoRaWAN stack handle events or go to sleep.
  LoRaWAN.sleep();
}
