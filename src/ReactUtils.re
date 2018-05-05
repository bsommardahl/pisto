let s = (message: string) => ReasonReact.stringToElement(message);

let sopt = (str: option(string)) =>
  switch (str) {
  | Some(thing) => ReasonReact.stringToElement(thing)
  | None => ReasonReact.stringToElement("")
  };