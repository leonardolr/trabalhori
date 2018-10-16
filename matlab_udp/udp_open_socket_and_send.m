% Esta funcao abre um socket e envia os dados de acordo com os parametros
% recebido na funcao.
%
% O primeiro parametro 'destIP' define o endereco IP de destino
% O segundo paramentro 'destPort' define a porta de destino
% O terceiro parametro 'localPort' define a porta do socket na maquina que
% esta enviando os dados
% O quarto parametro 'msg' eh a mensagem que se deseja enviar. Essa
% mensagem Pode ser um array de dados
% O quinto parametro 'typeOfMsg' eh opcional e define se a msg sera bytes
% (valores entre 0 e 255), se sera float, int16 e etc.
% Alguns valores permitidos em typeOfMsg sao 'uint8' (byte), 'float',
% 'int16', e outros parametros aceitos na funcao fwrite. Note que as aspas
% sao necessarias nesse parametro. Cada elemento do vetor passado no
% parametro 'msg' eh tratado como sendo do tipo colocado no parametro
% typeOfMsg. Caso esse parametro nao seja inserido, assumi-se o tipo
% 'uint8' que representa bytes (cada elemento de msg eh considerado um byte
% com valor entre 0 e 255).
%
function udp_open_socket_and_send(destIP,destPort,localPort,msg,typeOfMsg)

% Create UDP object with detination IP and destination port
% obj = udp(destIP,destPort,'LocalPort',localPort);

PacketSize = 1000;
obj = udp(destIP,destPort,'LocalPort',localPort,...
    'OutputBufferSize',3000,'OutputDatagramPacketSize',PacketSize);

% make the UDP object ready to send and receive data
fopen(obj);

% Send the data
if(nargin>4)
    fwrite(obj,msg,typeOfMsg);
else
    fwrite(obj,msg,'uint8');
    % You can change the parameters of the fwrite function to send data as
    % float, int16 or other types of data.
    % Below there are some examples, where each element of 'msg' is
    % represented as float and int16, respectively:
    % fwrite(obj,msg,'float');
    % fwrite(obj,msg,'int16');
end

% Close the socket
fclose(obj);
end