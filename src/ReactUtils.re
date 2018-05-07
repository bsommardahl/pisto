let s = (message: string) => ReasonReact.stringToElement(message);

let sloc = (key: string) => s(key |> Lang.translate);

let sopt = (str: option(string)) =>
  switch (str) {
  | Some(thing) => ReasonReact.stringToElement(thing)
  | None => ReasonReact.stringToElement("")
  };