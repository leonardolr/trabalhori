/*
 * This code is based on the example \WiFiEsp\examples\UdpSendReceive
 * This sketch sends an UDP packet on localPort using the ESP. 
 * When a packet is received an 'ACK' packet is sent to the client on port remotePort.
 * 
 * To use this code you should download and install WiFiEsp.
 * 
*/


#include <WiFiEsp.h>
#include <WiFiEspUdp.h>

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

// Max. number of bytes of the input buffer. You can change this value
#define MAX_SIZE_BUFFER_RX_UDP 200

char ssid[] = "temp-ap";            // your network SSID (name)
char pass[] = "trabalhoderedes";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

unsigned int localPort = 10002;  // local port to listen on

char packetBuffer[255];          // buffer to hold incoming packet
char ReplyBuffer[] = "ACK";      // a string to send back

WiFiEspUDP Udp;

// Address and port of remote node
// In this example, the remote IP will be a broadcast IP , which will 
// be configured in the setup, after gateway and subnet mask dicovery
IPAddress broadcast_ip; 
uint16_t remote_port = 9999;

// buffer for incoming packets
char incomingPacket[MAX_SIZE_BUFFER_RX_UDP];

// This pin is used to reset the ESP, during arduino setup()
const int resetpin_esp=12;

// These floats are transmitted over the implemented network
float f1 = 0;
float f2 = 0;

union float_bytes{
    float f;
    char f_bytes[4];
};

/*
 * This function gets a float f and returns 4 bytes in the array output_bytes
*/
void fxn_float_to_bytes(float f, char output_bytes[4]){
    union float_bytes tmp;
    tmp.f = f;
    // Invert the order because the endaness of the computer
    output_bytes[3] = tmp.f_bytes[0];
    output_bytes[2] = tmp.f_bytes[1];
    output_bytes[1] = tmp.f_bytes[2];
    output_bytes[0] = tmp.f_bytes[3];
}

void fxn_udp_send(float f1, float f2){
  char bytes_f1[4];
  char bytes_f2[4];
  
  // We should send 8 bytes (4 bytes from f1 and 4 bytes from f2)
  char bytes_to_send[8];
  int i;
  int status_begin;

  // get bytes from f1
  fxn_float_to_bytes(f1,bytes_f1);
  
  // get bytes from f2
  fxn_float_to_bytes(f2,bytes_f2);

  // copy byte to a single array
  for(i=0;i<4;++i){
    bytes_to_send[i]=bytes_f1[i];
  }
  for(i=0;i<4;++i){
    bytes_to_send[i+4]=bytes_f2[i];
  }
  
  status_begin = Udp.beginPacket(broadcast_ip, remote_port);
  Udp.write(bytes_to_send,8);
  Udp.endPacket();
}

IPAddress getBroadcastIP(IPAddress subNetMask, IPAddress gateway){
  IPAddress broadcastIP;
  // see https://en.wikipedia.org/wiki/Broadcast_address for more details
  // The subnet mask gives the information of how many bits are used to
  // specify the IP address space
  // The broadcast IP address is the IP address space with the last bits equal to 1
  // Since you know the gateway address and the subnetmask, you can find the broadcast adress
  broadcastIP[0] = (subNetMask[0] & gateway[0]) | ~subNetMask[0];
  broadcastIP[1] = (subNetMask[1] & gateway[1]) | ~subNetMask[1];
  broadcastIP[2] = (subNetMask[2] & gateway[2]) | ~subNetMask[2];
  broadcastIP[3] = (subNetMask[3] & gateway[3]) | ~subNetMask[3];
  return broadcastIP;
}

void checkUdpSocket(){
  int i;
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    Serial.print("\nRecebido uma MSG de ");
    Serial.print(packetSize);
    Serial.println("bytes");
    Serial.print("de um processo/host remoto com  IP: ");
    Serial.print(Udp.remoteIP());
    Serial.print(" e porta: ");
    Serial.println(Udp.remotePort(),DEC);
    Serial.println("Bytes da mensagem: ");
    int len = Udp.read(incomingPacket, MAX_SIZE_BUFFER_RX_UDP-1);
    if (len > 0)
    {
      for(i=0;i<len;++i){
        // print bytes as decimal
        Serial.print(incomingPacket[i],DEC);
        Serial.print(" ");
      }
      Serial.print("\r\n");
    }
    
    // We could send back a reply to the IP address and port we got the packet from
    // Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    // Udp.write("ACK");
    // Udp.endPacket();
  }
}

void setup() {

  // reset ESP
  pinMode(resetpin_esp, OUTPUT);
  digitalWrite(resetpin_esp, LOW);
  delay(100);
  digitalWrite(resetpin_esp, HIGH);
  delay(1000); // wait some time to ESP initiate
  
  // initialize serial for debugging
  // This serial is used to communicate with the PC, for debugging
  Serial.begin(115200);
  
  // initialize serial for ESP module
  // Before communicating with esp, you should change the baudrate of the ESP to 9600
  // A Higher baudrate with SoftwareSerial will probably fail. So when using a software serial
  // we should use a low baudrate
  Serial1.begin(9600);
  
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }
  
  Serial.println("Connected to wifi");
  printWifiStatus();

  //get the broadcast IP of this network
  broadcast_ip = getBroadcastIP(WiFi.subnetMask(),WiFi.gatewayIP());
  Serial.print("Broadcast IP: ");
  Serial.println(broadcast_ip);

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  Udp.begin(localPort);
  
  Serial.print("Listening on port ");
  Serial.println(localPort);
}

void loop() {
  // Send a UDP datagram with two floats over the UDP that uses WiFi link, in this case
  fxn_udp_send(f1,f2);
  // Increment both floats for the next round . Only for demostration
  f1 += 0.1;
  f2 += 0.2;
  // Pause the arduino. You don't want to overload the network :)
  delay(1000);
  // Check for incoming UDP datagrams
  checkUdpSocket();
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi); // there is a bug in the RSSI value
  Serial.println(" dBm");
}


