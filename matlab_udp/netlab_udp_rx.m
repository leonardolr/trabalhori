localPort  = 9999;

% Create UDP object with localPort
obj = udp('','LocalPort',localPort);

% Configure the socket 
% Callback function that is called when a new datagram arrives
obj.DatagramReceivedFcn = {@my_callback};

% The input buffer must be greater than the maximum size of incoming
% datagrams
obj.inputBufferSize = 3000;

% The output buffer must be greater than the maximum size of outcoming
% mensages from application
obj.OutputBufferSize = 3000;

obj.OutputDatagramPacketSize = 1000;

% make the UDP object ready to send and receive data
% Open the UDP socket
fopen(obj);

% You should close the object, after tests
%fclose(obj); 