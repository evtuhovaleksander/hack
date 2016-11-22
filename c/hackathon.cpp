#include "SX1272.h"
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h> 
#include  <signal.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

// Define section

#define BAND868 //900, 433
#define MAX_NB_CHANNEL 9
#define STARTING_CHANNEL 10
#define ENDING_CHANNEL 18
uint8_t loraChannelIndex=0;
uint32_t loraChannelArray[MAX_NB_CHANNEL]={CH_10_868,CH_11_868,CH_12_868,CH_13_868,CH_14_868,CH_15_868,CH_16_868,CH_17_868,CH_18_868};
#define LORAMODE  1 //Mode
#define LORA_ADDR 255 //Self address
#define DEFAULT_DEST_ADDR 6 //Gateway address

  #define PRINTLN                   printf("\n")
  #define PRINT_CSTSTR(fmt,param)   printf(fmt,param)
  #define PRINT_STR(fmt,param)      PRINT_CSTSTR(fmt,param)
  #define PRINT_VALUE(fmt,param)    PRINT_CSTSTR(fmt,param)
  #define FLUSHOUTPUT               fflush(stdout);



//Variales
int dest_addr=DEFAULT_DEST_ADDR;
char cmd[260]="****************";
char sprintf_buf[100];
int msg_sn=0;
bool radioON=false;
uint8_t loraMode=LORAMODE;
uint32_t loraChannel=loraChannelArray[loraChannelIndex];
char loraPower='x'; //innitial poser level, M (maximum), H (high), L (low)
uint8_t loraAddr=LORA_ADDR;
unsigned int inter_pkt_time=10000; //Time between sending
unsigned int random_inter_pkt_time=0;
long last_periodic_sendtime=0;
// packet size for periodic sending
uint8_t MSS=40;






void SelectAndSend(int iden,stat)
{
    int rc;
    char *error;


    sqlite3 *db;
    rc = sqlite3_open("places.db", &db);


    char *sqlSelect = "SELECT status FROM Place where identifier=6;";
    char **results = NULL;
    int rows, columns;
    sqlite3_get_table(db, sqlSelect, &results, &rows, &columns, &error);
    if(columns==1&&rows==1)
    {
        int cellPosition =0;
        char *status=results[cellPosition];
        send_data(6,status);
    }
    sqlite3_free_table(results);
    sqlite3_close(db);

}


void UpdateDB(int identifier, int status)
{
    int rc;
    char *error;


    sqlite3 *db;
    rc = sqlite3_open("MyDb.db", &db);

    // Execute SQL

    char *sqlUpdateTable = "Update Place Set status = 1 where identifier=6";
    rc = sqlite3_exec(db, sqlCreateTable, NULL, NULL, &error);

    sqlite3_close(db);


}

void send_data(int iden,char *status)
{
    int i=0, e;
    int cmdValue;
    if (radioON) {



        if (inter_pkt_time) {

            if (millis() - last_periodic_sendtime > (random_inter_pkt_time ? random_inter_pkt_time : inter_pkt_time)) {
                PRINT_CSTSTR("%s", "Start to send data: ");
                PRINT_VALUE("%ld", millis());
                PRINTLN;

                sx1272.CarrierSense();
                long startSend = millis();
                e = sx1272.sendPacketTimeout(iden, (uint8_t *) status, strlen(status), 10000);
                PRINT_CSTSTR("%s", "LoRa Sent in ");
                PRINT_VALUE("%ld", millis() - startSend);
                PRINTLN;
                PRINT_CSTSTR("%s", "Packet sent, state ");
                PRINT_VALUE("%d", e);
                PRINTLN;
                if (random_inter_pkt_time) {
                    random_inter_pkt_time = random() % inter_pkt_time + 1000;
                    PRINT_CSTSTR("%s", "next in ");
                    PRINT_VALUE("%ld", random_inter_pkt_time);
                    PRINTLN;
                }
                last_periodic_sendtime = millis();
            }
        }




    }

}






























//Configure LoRa tranciever
void startConfig() {

  int e;
    
  // Set transmission mode and print the result
  e = sx1272.setMode(loraMode);
  // Select frequency channel
  if (loraMode==11) {
    e = sx1272.setChannel(CH_18_868);
  }
  else {
    e = sx1272.setChannel(loraChannel);
  }  
  // Select output power (Max, High or Low)
  e = sx1272.setPower(loraPower);
  // get preamble length
  e = sx1272.getPreambleLength();
  // Set the node address and print the result
  //e = sx1272.setNodeAddress(loraAddr);
  sx1272._nodeAddress=loraAddr;
  e=0;
}

void setup() {
  int e;
  
  //Add our code here
  Serial.begin(38400);
  // Power ON the module
  e = sx1272.ON();
  
  PRINT_CSTSTR("%s","^$**********Power ON: state ");
  PRINT_VALUE("%d", e);
  PRINTLN;

  e = sx1272.getSyncWord();

  if (!e) {
    PRINT_CSTSTR("%s","^$Default sync word: 0x");
    PRINT_VALUE("%X", sx1272._syncWord);
    PRINTLN;

  }    
  
  if (!e) {
    radioON=true;
    startConfig();
  }
  
  FLUSHOUTPUT;
  delay(1000);

}

void loop() {




  int i=0, e;
  int cmdValue;
    SelectAndSend(6);

  if (radioON) {



      

        
      // the end-device should also open a receiving window to receive 
      // INIT & UPDT messages
      e=1;
      // open a receive window
      uint16_t w_timer=1000;
      if (loraMode==1)
        w_timer=5000;
      e = sx1272.receivePacketTimeout(w_timer);
      // IF WE RECEIVE A RADIO PACKET
      if (!e) {
         int a=0, b=0;
         uint8_t tmp_length;

         sx1272.getSNR();
         sx1272.getRSSIpacket();

         tmp_length=sx1272._payloadlength;
         
         sprintf(sprintf_buf,"--- rxlora. dst=%d type=0x%.2X src=%d seq=%d len=%d SNR=%d RSSIpkt=%d BW=%d CR=4/%d SF=%d\n", 
                   sx1272.packet_received.dst,
                   sx1272.packet_received.type, 
                   sx1272.packet_received.src,
                   sx1272.packet_received.packnum,
                   tmp_length, 
                   sx1272._SNR,
                   sx1272._RSSIpacket,
                   (sx1272._bandwidth==BW_125)?125:((sx1272._bandwidth==BW_250)?250:500),
                   sx1272._codingRate+4,
                   sx1272._spreadingFactor);
                   
         PRINT_STR("%s",sprintf_buf);
         // provide a short output for external program to have information about the received packet
         // ^psrc_id,seq,len,SNR,RSSI
         sprintf(sprintf_buf,"^p%d,%d,%d,%d,%d,%d,%d\n",
                   sx1272.packet_received.dst,
                   sx1272.packet_received.type,                   
                   sx1272.packet_received.src,
                   sx1272.packet_received.packnum, 
                   tmp_length,
                   sx1272._SNR,
                   sx1272._RSSIpacket);
                   
         PRINT_STR("%s",sprintf_buf);          

         // ^rbw,cr,sf
         sprintf(sprintf_buf,"^r%d,%d,%d\n", 
                   (sx1272._bandwidth==BW_125)?125:((sx1272._bandwidth==BW_250)?250:500),
                   sx1272._codingRate+4,
                   sx1272._spreadingFactor);
                   
         PRINT_STR("%s",sprintf_buf);


          int iden=sx1272.packet_received.src;
          int st=0;
         if((char)sx1272.packet_received.data[0]=='1')
         {
             st=1;
         }
          else
         {
             st=0;
         }
          UpdateDB(iden, st)

      }

  }
}


int main (int argc, char *argv[]){

  setup();
  
  while(1){
    loop();
  }
  
  return (0);
}








