type t = float;

let now = Js.Date.now;

let day = 86400.0;

let toDisplay = t => Js.Date.fromFloat(t) |> Js.Date.toDateString;

let oneMonthBefore = t => t -. day *. 30.44;