[%bs.raw {|require('./index.css')|}];

[%bs.raw {|require('./loader.css')|}];

[%bs.raw {| window.Intercom("boot", { app_id: "y3fecu0h" })|}];

[@bs.module "./registerServiceWorker"]
external register_service_worker : unit => unit = "default";

ReactDOMRe.renderToElementWithId(<App />, "root");

register_service_worker();

let handleError = (message: string, url: string, line: int, col: int) : bool => {
  Logger.logError(
    message,
    "line:" ++ (line |> string_of_int) ++ "|col:" ++ (col |> string_of_int),
    url,
  )
  |> ignore;
  true;
};

[%raw {| window.onerror = handleError |}];