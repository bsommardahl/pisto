type t = float;

let now = Js.Date.now;

let day = 86400000.0;

let toDisplayDate = t => Js.Date.fromFloat(t) |> Js.Date.toDateString;

let toDisplayTime = t => Js.Date.fromFloat(t) |> Js.Date.toTimeString;

let toDisplay = t => Js.Date.fromFloat(t) |> Js.Date.toString;

let oneMonthBefore = t => t -. day *. 30.44;

let toFloat = (formattedDate: string) : float =>
  formattedDate |> Js.Date.parse |> Js.Date.getTime;

let isValid = (formattedDate: string) : bool =>
  switch (formattedDate |> toFloat |> toDisplay) {
  | "Invalid Date" => false
  | _ => true
  };