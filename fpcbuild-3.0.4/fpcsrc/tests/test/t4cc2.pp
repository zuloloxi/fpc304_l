{ %fail }

{$mode fpc}

type
  tchararr = array[1..4] of char;

var
  l: longint;
  
begin
  l[1] := 'a';
  l[2] := 'b';
  l[3] := 'c';
  l[4] := 'd';
end.

