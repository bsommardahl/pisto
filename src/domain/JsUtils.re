let convertFloatOption = d : option(float) => Js.Nullable.toOption(d);

let convertIntOption = d : option(int) => Js.Nullable.toOption(d);

let convertStringOption = s : option(string) => Js.Nullable.toOption(s);

let convertDate = d => d;