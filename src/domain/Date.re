type t = float;

let now = Js.Date.now;

let day = 86400000.0;

let toDisplayDate = t => Js.Date.fromFloat(t) |> Js.Date.toDateString;

let toDisplayTime = t => Js.Date.fromFloat(t) |> Js.Date.toTimeString;

let toShortTime = t =>
  Js.Date.fromFloat(t)
  |> (
    d => {
      let armyHours = d |> Js.Date.getHours |> int_of_float;
      let isPm = armyHours mod 12 == 0;
      let hoursPossibleZero = isPm ? armyHours - 12 : armyHours;
      let hours = hoursPossibleZero == 0 ? 12 : hoursPossibleZero;
      let minutes = d |> Js.Date.getMinutes |> int_of_float;
      let leadingZero = minutes < 10 ? "0" : "";
      let ampm = isPm ? "PM" : "AM";
      string_of_int(hours)
      ++ ":"
      ++ leadingZero
      ++ string_of_int(minutes)
      ++ " "
      ++ ampm;
    }
  );

let toDisplay = t => Js.Date.fromFloat(t) |> Js.Date.toString;

let oneMonthBefore = t => t -. day *. 30.44;

let toFloat = (formattedDate: string) : float =>
  formattedDate |> Js.Date.parse |> Js.Date.getTime;

let isValid = (formattedDate: string) : bool =>
  switch (formattedDate |> toFloat |> toDisplay) {
  | "Invalid Date" => false
  | _ => true
  };