type response = {
  webhook: Webhook.t,
  payload: Js.Json.t,
  error: option(Js.Promise.error),
};

let emptyJson = Js.Json.parseExn("{}");

let convertPayloadToJson = payload =>
  switch (Js.Json.stringifyAny(payload)) {
  | None => "{}"
  | Some(json) => json
  };

let buildRequest = (payload: string) =>
  Fetch.RequestInit.make(
    ~method_=Post,
    ~body=Fetch.BodyInit.make(payload),
    ~headers=
      Fetch.HeadersInit.make({
        "Content-Type": "application/json",
        "Accept": "application/json",
      }),
    (),
  );

external unsafeConvert : Js.Json.t => Js.t('a) = "%identity";

let catchFetchError = (hook: Webhook.t, promise) : Js.Promise.t(response) =>
  promise
  |> Js.Promise.catch((err: Js.Promise.error) => {
       Js.log("Error when fetching webhook: " ++ hook.name);
       Js.log(err);
       let response = {webhook: hook, payload: emptyJson, error: Some(err)};
       Js.Promise.resolve(response);
     });

let fetch = (hook: Webhook.t, payload: 'a) : Js.Promise.t(response) =>
  payload
  |> convertPayloadToJson
  |> buildRequest
  |> Fetch.fetchWithInit(hook.url)
  |> Js.Promise.then_(Fetch.Response.json)
  |> Js.Promise.then_((json: Js.Json.t) => {
       let response = {webhook: hook, payload: json, error: None};
       Js.Promise.resolve(response);
     })
  |> catchFetchError(hook);

let nullWebhook: Webhook.t = {
  id: "n/a",
  name: "nullWebhook",
  source: Order,
  url: "",
  event: Unrecognized,
};

let log = (message, promise) =>
  promise
  |> Js.Promise.then_(payload => {
       Js.log(message ++ ":: ");
       Js.log(payload);
       Js.Promise.resolve(payload);
     });

let getWebhooks = (event, source) : Js.Promise.t(list(Webhook.t)) =>
  WebhookStore.getAll()
  |> Js.Promise.then_((list: list(Webhook.t)) => {
       Js.log("All webhooks available:");
       list
       |> List.filter((x: Webhook.t) => {
            Js.log(x);
            x.event === event && x.source === source;
          })
       |> Js.Promise.resolve;
     });

let fetchAllWebhooks =
    (list: list(Webhook.t), payload: Js.t('a))
    : Js.Promise.t(list(response)) =>
  Js.Promise.(
    list
    |> List.map((w: Webhook.t) => fetch(w, payload))
    |> Array.of_list
    |> all
    |> then_(x => resolve(x |> Array.to_list))
  );

let fire =
    (payload: Js.t('a), webhooks: Js.Promise.t(list(Webhook.t)))
    : Js.Promise.t(list(response)) =>
  webhooks
  |> Js.Promise.then_((list: list(Webhook.t)) =>
       switch (list |> List.length) {
       | 0 => Js.Promise.resolve([])
       | _ => fetchAllWebhooks(list, payload)
       }
     );