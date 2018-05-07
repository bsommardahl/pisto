type message = {
  key: string,
  language: string,
  content: string,
};

let m = (key, language, content) => {key, language, content};

[@bs.module] external en : Js.Array.t('a) = "./lang/en.json";

[@bs.module] external es : Js.Array.t('a) = "./lang/es.json";

let messages = (codex, languageCode) =>
  codex
  |> Js.Array.map(x => m(x##key, languageCode, x##content))
  |> Array.to_list;

let dictionary: list(message) =
  List.concat([messages(en, "EN"), messages(es, "ES")]);

let translate = key => {
  let language = Config.App.get().language;
  let matches =
    dictionary
    |> List.filter((message: message) =>
         message.key === key && message.language === language
       );
  switch (matches |> List.length) {
  | 0 => key ++ "." ++ language
  | _ => (matches |. List.nth(0)).content
  };
};