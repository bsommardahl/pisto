type t = int;

let toDisplay = t =>
  "L." ++ ((t |> float_of_int) /. 100.0 |> int_of_float |> string_of_int);