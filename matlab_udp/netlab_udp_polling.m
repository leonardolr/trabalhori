localPort  = 9999;

% Create UDP object with detination IP and destination port
obj = udp('','LocalPort',localPort);

% The input buffer must be greater than the maximum size of incoming
% datagrams
obj.inputBufferSize = 3000;

% The output buffer must be greater than the maximum size of outcoming
% mensages from application
obj.OutputBufferSize = 3000;

obj.OutputDatagramPacketSize = 1000;

% make the UDP object ready to send and receive data
fopen(obj);
fprintf('Esperando pacotes na porta: %d\n',localPort);

while(1)
    if(obj.BytesAvailable>0)
        fprintf('A new datagram arrived from IP:%s Port:%d :\n',...
        obj.DatagramAddress,obj.DatagramPort);
        
        % get information of the remoteHost and remotePort from the
        % received UDP segment
        obj.RemoteHost = obj.DatagramAddress;
        obj.RemotePort = obj.DatagramPort;
        
        % Read float from receved data 
        % Each float has 4 bytes, thus the number of float in the message
        % is obj.BytesAvailable/4
        msg = fread(obj,obj.BytesAvailable/4,'float');
        
        disp(msg(:).');
        
        % Proccess the msg and transform the multiplication reult in floats
        X = single(5*msg); 
        
        % Send the processed data back to remote host
        fwrite(obj,X,'float');
    end
end

% Remember to call fclose(obj) before attempting to open a new socket
