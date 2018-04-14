let s = (str: string) => ReasonReact.stringToElement(str);

let sopt = (str: option(string)) =>
  switch (str) {
  | Some(thing) => ReasonReact.stringToElement(thing)
  | None => ReasonReact.stringToElement("")
  };