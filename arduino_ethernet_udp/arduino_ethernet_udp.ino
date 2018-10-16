/*
 * This example shows how to send and receive data using an UDP socket,
 * which is transmitted over an Ethernet link.
 * More specifically, the arduino initiate the Ethernet interface and UDP socket.
 * Then, in the loop() function, the arduino board sends two floats over the UDP socket,
 * increment the floats for the next round and check if there is data in the UDP socket to read.
*/


#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

// Max. number of bytes of the input buffer. You can change this value
#define MAX_SIZE_BUFFER_RX_UDP 200

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

// local port to transmit and listen UDP segments
unsigned int localPort = 9876;

// Address and port of remote node
// In this example, the remote IP will be a broadcast IP , which will 
// be configured in the setup, after gateway and subnet mask dicovery
IPAddress broadcast_ip; 
unsigned int remote_port = 9999;

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

// Buffer used to edit messages before sending via UDP
char buffer_str[50];

// buffer for incoming packets
char incomingPacket[MAX_SIZE_BUFFER_RX_UDP];

// These floats are transmitted over the implemented network
float f1 = 0;
float f2 = 0;

// This union is used to convert float in bytes, before sending float vover Ethernet
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

  // To send an UDP segment, we have to use the three lines below
  // After calling Udp.endPacket(), the UDP segment is sent over the network
  Udp.beginPacket(broadcast_ip, remote_port);
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

void setup() {
 
  // start the serial library:
  Serial.begin(115200);
  Serial.println("Inicializacao Serial");
  Serial.println("Tentando obter IP...");
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
  Serial.print("IP obtido: ");
  // print your local IP address:
  Serial.println(Ethernet.localIP());

  //get the broadcast IP of this network
  broadcast_ip = getBroadcastIP(Ethernet.subnetMask(),Ethernet.gatewayIP());
  Serial.print("Broadcast IP: ");
  Serial.println(broadcast_ip);

  // Open UDP socket
  Udp.begin(localPort);
  Serial.println("Socket Aberto");
  Serial.print("Escutando no IP: ");
  Serial.print(Ethernet.localIP());
  Serial.print("  Porta: ");
  Serial.println(localPort);
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
    Serial.println(Udp.remotePort());
    Serial.println("Bytes da mensagem: ");
    int len = Udp.read(incomingPacket, MAX_SIZE_BUFFER_RX_UDP-1);
    if (len > 0)
    {
      for(i=0;i<len;++i){
        // print bytes as decimal
        Serial.print(incomingPacket[i],DEC);
        Serial.print(" ");
      }
      Serial.print("\n");
    }
    
    // We could send back a reply to the IP address and port we got the packet from
    // Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    // Udp.write("ACK");
    // Udp.endPacket();
  }
}

void loop() {
  // Send a UDP datagram with two floats over the Ethernet link
  fxn_udp_send(f1,f2);
  // Increment both floats for the net round . Only for demostration
  f1 += 0.1;
  f2 += 0.2;
  // Pause the arduino you dont want to overload the network :)
  delay(1000);
  // Check for incoming UDP datagrams
  checkUdpSocket();
}
