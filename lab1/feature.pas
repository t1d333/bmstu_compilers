program PrintNumbers;


function AddNumbers(a, b..integer): integer;
begin
    AddNumbers := a + b;
end;


var
  i: Integer;
  A: array[1:5] of Integer;
  
begin
  for i := 1 to 5 do
  begin
    A[i] := i;
  end;

  for i := 1 to 5 do
  begin
    writeln(A[i]);
  end;
end.
