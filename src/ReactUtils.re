let s = (message: string) => ReasonReact.string(message);

let sloc = (key: string) => s(key |> Lang.translate);

let sopt = (str: option(string)) =>
  switch (str) {
  | Some(thing) => ReasonReact.string(thing)
  | None => ReasonReact.string("")
  };

let getVal = ev => ev->ReactEvent.Form.target##value;