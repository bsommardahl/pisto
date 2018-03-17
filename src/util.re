let s = str =>
  switch (str) {
  | thing => ReasonReact.stringToElement(thing)
  | _ => ReasonReact.stringToElement("")
  };

let sopt = (str: option(string)) =>
  switch (str) {
  | Some(thing) => ReasonReact.stringToElement(thing)
  | None => ReasonReact.stringToElement("")
  };

module QueryParam = {
  type t = (string, string);
  let get = (key: string, q: string) : option(string) => {
    let queryStrToPairs = (queryStr: string) => {
      let splits = Js.String.split("&", queryStr);
      Js.Array.reduce(
        (result, str) => {
          let pair =
            switch (Js.String.split("=", str)) {
            | [|k, v|] =>
              let p = (k, v);
              Some(p);
            | [|k|] =>
              let p = (k, "");
              Some(p);
            | _ => None
            };
          switch (pair) {
          | Some(p) =>
            Js.Array.push(p, result);
            result;
          | None => result
          };
        },
        [||],
        splits,
      );
    };
    let getQueryParam = (key: string, qArr: array(t)) : option(string) => {
      let pair = Js.Array.find(((k, _v)) => k === key, qArr);
      switch (pair) {
      | Some(p) =>
        let (_, v) = p;
        Some(v);
      | None => None
      };
    };
    queryStrToPairs(q) |> getQueryParam(key);
  };
};