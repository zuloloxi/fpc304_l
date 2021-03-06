{
    Free Pascal port of the OpenPTC C++ library.
    Copyright (C) 2001-2007, 2009-2012  Nikolay Nikolov (nickysn@users.sourceforge.net)
    Original C++ version by Glenn Fiedler (ptc@gaffer.org)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version
    with the following modification:

    As a special exception, the copyright holders of this library give you
    permission to link this library with independent modules to produce an
    executable, regardless of the license terms of these independent modules,and
    to copy and distribute the resulting executable under terms of your choice,
    provided that you also meet, for each linked independent module, the terms
    and conditions of the license of that module. An independent module is a
    module which is not derived from or based on this library. If you modify
    this library, you may extend this exception to your version of the library,
    but you are not obligated to do so. If you do not wish to do so, delete this
    exception statement from your version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
}

type
  TPTCFormat = class(TInterfacedObject, IPTCFormat)
  private
    FFormat: THermesFormat;
    function GetHermesFormat: PHermesFormat;
    function Equals(AFormat: IPTCFormat): Boolean; reintroduce;
    function GetR: Uint32;
    function GetG: Uint32;
    function GetB: Uint32;
    function GetA: Uint32;
    function GetBits: Integer;
    function GetIndexed: Boolean;
    function GetDirect: Boolean;
    function GetBytes: Integer;
  public
    constructor Create;
    constructor Create(ABits: Integer);
    constructor Create(ABits: Integer;
                       ARedMask, AGreenMask, ABlueMask: Uint32;
                       AAlphaMask: Uint32 = 0);
    constructor Create(AFormat: IPTCFormat);
    destructor Destroy; override;
//    procedure Assign(const format: TPTCFormat);
{    property R: Uint32 read GetR;
    property G: Uint32 read GetG;
    property B: Uint32 read GetB;
    property A: Uint32 read GetA;
    property Bits: Integer read GetBits;
    property Indexed: Boolean read GetIndexed;
    property Direct: Boolean read GetDirect;
    property Bytes: Integer read GetBytes;}
  end;

class function TPTCFormatFactory.CreateNew: IPTCFormat;
begin
  Result := TPTCFormat.Create;
end;

class function TPTCFormatFactory.CreateNew(ABits: Integer): IPTCFormat;
begin
  Result := TPTCFormat.Create(ABits);
end;

class function TPTCFormatFactory.CreateNew(ABits: Integer;
                                           ARedMask, AGreenMask, ABlueMask: Uint32;
                                           AAlphaMask: Uint32 = 0): IPTCFormat;
begin
  Result := TPTCFormat.Create(ABits, ARedMask, AGreenMask, ABlueMask, AAlphaMask);
end;

class function TPTCFormatFactory.CreateNew(AFormat: IPTCFormat): IPTCFormat;
begin
  Result := TPTCFormat.Create(AFormat);
end;

constructor TPTCFormat.Create;
begin
  { defaults }
  FFormat.r := 0;
  FFormat.g := 0;
  FFormat.b := 0;
  FFormat.a := 0;
  FFormat.bits := 0;
  FFormat.indexed := False;

  { initialize hermes }
  if not Hermes_Init then
    raise TPTCError.Create('could not initialize hermes');
end;

constructor TPTCFormat.Create(ABits: Integer);
begin
  { check bits per pixel }
  if ABits <> 8 then
    raise TPTCError.Create('unsupported bits per pixel');

  { indexed color }
  FFormat.r := 0;
  FFormat.g := 0;
  FFormat.b := 0;
  FFormat.a := 0;
  FFormat.bits := ABits;
  FFormat.indexed := True;

  { initialize hermes }
  if not Hermes_Init then
    raise TPTCError.Create('could not initialize hermes');
end;

constructor TPTCFormat.Create(ABits: Integer;
                              ARedMask, AGreenMask, ABlueMask: Uint32;
                              AAlphaMask: Uint32 = 0);
begin
  { check bits per pixel }
  if ((ABits and 7) <> 0) or (ABits <= 0) or (ABits > 32) then
    raise TPTCError.Create('unsupported bits per pixel');

  { direct color }
  FFormat.r := ARedMask;
  FFormat.g := AGreenMask;
  FFormat.b := ABlueMask;
  FFormat.a := AAlphaMask;
  FFormat.bits := ABits;
  FFormat.indexed := False;

  { initialize hermes }
  if not Hermes_Init then
    raise TPTCError.Create('could not initialize hermes');
end;

constructor TPTCFormat.Create(AFormat: IPTCFormat);
begin
  { initialize hermes }
  if not Hermes_Init then
    raise TPTCError.Create('could not initialize hermes');

  Hermes_FormatCopy(AFormat.GetHermesFormat, @FFormat)
end;

{$INFO TODO: check what happens if Hermes_Init blows up in the constructor...}
destructor TPTCFormat.Destroy;

begin
  Hermes_Done;
  inherited Destroy;
end;

function TPTCFormat.GetHermesFormat: PHermesFormat;
begin
  Result := @Fformat;
end;

{procedure TPTCFormat.Assign(const format: TPTCFormat);
begin
  if Self = format then
    exit;
  Hermes_FormatCopy(@format.Fformat, @Fformat);
end;}

function TPTCFormat.Equals(AFormat: IPTCFormat): Boolean;
begin
  Result := Hermes_FormatEquals(AFormat.GetHermesFormat, @FFormat);
end;

function TPTCFormat.GetR: Uint32;
begin
  Result := FFormat.r;
end;

function TPTCFormat.GetG: Uint32;
begin
  Result := FFormat.g;
end;

function TPTCFormat.GetB: Uint32;
begin
  Result := FFormat.b;
end;

function TPTCFormat.GetA: Uint32;
begin
  Result := FFormat.a;
end;

function TPTCFormat.GetBits: Integer;
begin
  Result := FFormat.bits;
end;

function TPTCFormat.GetIndexed: Boolean;
begin
  Result := FFormat.indexed;
end;

function TPTCFormat.GetDirect: Boolean;
begin
  Result := not FFormat.indexed;
end;

function TPTCFormat.GetBytes: Integer;
begin
  Result := FFormat.bits shr 3;
end;
