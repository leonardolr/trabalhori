% Please, see [1] to learn more about event and callbacks in MATLAB
% [1] https://www.mathworks.com/help/instrument/events-and-callbacks_f13-51459.html#f13-43866
% 
function my_callback(udpobj,event)
    fprintf('A new datagram arrived from IP:%s Port:%d\n',...
        udpobj.DatagramAddress,udpobj.DatagramPort);

    % Get information about the incoming datagram:
    % The IP Address and UDP port
    % Then, configure the remote host with information get from the
    % datagram
    udpobj.RemoteHost = udpobj.DatagramAddress;
    udpobj.RemotePort = udpobj.DatagramPort;
    
    % read data as float because you know that the sender is transmitting
    % float:
    msg = fread(udpobj,udpobj.BytesAvailable,'float');
    % You could read the msg as bytes, if necessary
    % msg = fread(udpobj,udpobj.BytesAvailable)
    disp('Received msg: ');
    disp(msg);
    
    % X = single(5*msg); % transform the multiplication reult in floats
    
    % Send the data back
    %fwrite(udpobj,X,'float');
end