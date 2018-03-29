type t = int;

let toDisplay = t => Printf.sprintf("L.%.2f", (t |> float_of_int) /. 100.0);