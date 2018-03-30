type t = int;

let toDisplay = t => Printf.sprintf("L.%.2f", (t |> float_of_int) /. 100.0);

let times100 = (n: float) => n *. 100.0;

let toT = (str: string) : t =>
  switch (str |> Js.String.replaceByRe([%bs.re "/\\D/g"], "")) {
  | "" => 0
  | s => s |> int_of_string
  };