type t = float;

let now = Js.Date.now;

let day = 86400.0;

let toDisplayDate = t => Js.Date.fromFloat(t) |> Js.Date.toDateString;

let toDisplayTime = t => Js.Date.fromFloat(t) |> Js.Date.toTimeString;

let oneMonthBefore = t => t -. day *. 30.44;