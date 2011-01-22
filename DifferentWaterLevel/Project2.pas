program Project2;

{$APPTYPE CONSOLE}

uses
  SysUtils,
  StdCtrls,
  Classes,
  Windows;

var fname: string;
    chunk, offstart, lqoffs, i: integer;
    height: single;
    Streami: TFileStream;
    tempb: boolean;

begin
  { TODO -oUser -cConsole Main : Insert code here }
  Writeln('Please insert the Path to the File you want to modify:');
  Readln(fname);
  try
  Streami:=TFileStream.Create(fname, fmOpenReadWrite or fmShareDenyNone);
  Writeln('Please insert the Chunk:');
  Readln(chunk);
  Writeln('Please insert the Height:');
  Readln(height);
  Streami.Seek($14+$40+((chunk-1)*16+8), SoFromBeginning);
  Streami.ReadBuffer(offstart, 4);
  Writeln(offstart);
  Streami.Seek(offstart+$8+$60, SoFromBeginning);
  Streami.ReadBuffer(lqoffs, 4);
  Streami.Seek(offstart+$8+lqoffs+$8, SoFromBeginning);
  Streami.WriteBuffer(height, Sizeof(Single));
  Streami.WriteBuffer(height, Sizeof(Single));
  for i:=0 to 81 do
  begin
  Streami.Seek(4, SoCurrent);
  Streami.WriteBuffer(height, Sizeof(Single));
  end;
  Writeln('Success!');

  except
  Writeln('An Error occurred!');
  end

end.
 