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

let toISODate = t : string =>
  t
  |> Js.Date.fromFloat
  |> Js.Date.toISOString
  |> Js.String.split("T")
  |> (x => x[0]);

let toDisplay = t => Js.Date.fromFloat(t) |> Js.Date.toString;

let oneMonthBefore = t => t -. day *. 30.44;

let addDays = (d: int, t) : t => t +. day *. float_of_int(d);

let startOfDay = (d: t) : t => {
  let jsDate = d |> Js.Date.fromFloat;
  let _ =
    Js.Date.setUTCHoursMS(jsDate, ~hours=0., ~minutes=0., ~seconds=0., ());
  jsDate |> Js.Date.getTime;
};

let endOfDay = (d: t) : t => {
  let jsDate = d |> Js.Date.fromFloat;
  let _ =
    Js.Date.setUTCHoursMS(jsDate, ~hours=23., ~minutes=59., ~seconds=59., ());
  jsDate |> Js.Date.getTime;
};

let toFloat = (formattedDate: string) : float =>
  formattedDate |> Js.Date.parse |> Js.Date.getTime;

let isValid = (formattedDate: string) : bool =>
  switch (formattedDate |> toFloat |> toDisplay) {
  | "Invalid Date" => false
  | _ => true
  };