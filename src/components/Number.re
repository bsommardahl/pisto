type t = int;

let cleanNonNumeric = n => n |> Js.String.replaceByRe([%bs.re "/\\D/g"], "");

let fromString = (str: string): t =>
  switch (str |> cleanNonNumeric) {
  | "" => 0
  | s => s |> int_of_string
  };
