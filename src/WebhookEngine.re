type response = {
  webhook: Webhook.t,
  payload: Js.Json.t,
};

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

let fetch = (hook: Webhook.t, payload: 'a) : Js.Promise.t(response) =>
  payload
  |> convertPayloadToJson
  |> buildRequest
  |> Fetch.fetchWithInit(hook.url)
  |> Js.Promise.then_(Fetch.Response.json)
  |> Js.Promise.then_(json => {
       Js.log(json);
       Js.Promise.resolve(json);
     })
  |> Js.Promise.catch(err => {
       Js.log("Webhook error with " ++ hook.name);
       Js.log(err);
       let json =
         Js.Json.parseExn(
           switch (Js.Json.stringifyAny(err)) {
           | Some(s) => s
           | None => "{}"
           },
         );
       Js.Promise.resolve(json);
     })
  |> Js.Promise.then_((json: Js.Json.t) => {
       let response = {webhook: hook, payload: json};
       Js.Promise.resolve(response);
     });

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
       let filtered =
         list
         |> List.filter((x: Webhook.t) =>
              x.event === event && x.source === source
            );
       (
         switch (filtered |> List.length) {
         | 0 => [nullWebhook]
         | _ => list
         }
       )
       |> Js.Promise.resolve;
     })
  |> log("gotWebhooks");

let fetchAllWebhooksButReturnFirstOrder =
    (list: list(Webhook.t), order: Order.t)
    : Js.Promise.t(Order.t) =>
  list
  |> List.map((w: Webhook.t) => fetch(w, order))
  |> Array.of_list
  |> Js.Promise.all
  |> Js.Promise.then_(arr =>
       arr |> Array.to_list |. List.nth(0) |> Js.Promise.resolve
     )
  |> log("fetch responses")
  |> Js.Promise.then_((r: response) =>
       (
         r.payload
         |> unsafeConvert
         |> Order.mapOrderFromJs
         |> Js.Promise.resolve:
           Js.Promise.t(Order.t)
       )
     )
  |> log("first fetch responses");

let fireForOrder =
    (order: Order.t, webhooks: Js.Promise.t(list(Webhook.t)))
    : Most.stream(Order.t) =>
  webhooks
  |> Js.Promise.then_((list: list(Webhook.t)) =>
       switch (list |> List.length) {
       | 0 => Js.Promise.resolve(order)
       | _ => order |> fetchAllWebhooksButReturnFirstOrder(list)
       }
     )
  |> Most.fromPromise;