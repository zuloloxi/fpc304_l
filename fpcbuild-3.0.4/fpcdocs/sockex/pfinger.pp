program pfinger;

uses sockets,errors;

Var
  Addr : TInetSockAddr;
  S : Longint;
  Sin,Sout : Text;
  Line : string;

begin
  Addr.sin_family:=AF_INET;
  { port 79 in network order }
  Addr.sin_port:=79 shl 8;
  { localhost : 127.0.0.1 in network order }
  Addr.sin_addr.s_addr:=((1 shl 24) or 127);
  S:=fpSocket(AF_INET,SOCK_STREAM,0);
  If Not Connect (S,ADDR,SIN,SOUT) Then
    begin
    Writeln ('Couldn''t connect to localhost');
    Writeln ('Socket error : ',strerror(SocketError));
    halt(1);
    end;
  rewrite (sout);
  reset(sin);
  writeln (sout,paramstr(1));
  flush(sout);
  while not eof(sin) do
    begin
    readln (Sin,line);
    writeln (line);
    end;
  fpShutdown(s,2);
  close (sin);
  close (sout);
end.
