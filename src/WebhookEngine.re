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

let decode = (payload: Js.Json.t) => {
  Js.log("firing ");
  Js.log(payload);
  (payload |> unsafeConvert |> Order.mapOrderFromJs: Order.t);
};

let fetch = (hook: Webhook.t, payload: 'a) : Most.stream(response) =>
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
     })
  |> Most.fromPromise;

let nullWebhook: Webhook.t = {
  id: "n/a",
  name: "nullWebhook",
  source: Order,
  url: "",
  event: Unrecognized,
};

let getWebhooks = (event, source) : Most.stream(Webhook.t) =>
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
  |> Most.fromPromise
  |> Most.flatMap((x: list(Webhook.t)) => x |> Most.fromList)
  |> Most.map((x: Webhook.t) => {
       Js.log("Webhook found: " ++ x.name);
       x;
     });

let fireForOrder =
    (event: Webhook.EventType.t, order: Order.t)
    : Most.stream(Order.t) => {
  let webhooks = getWebhooks(event, Order);
  /* need to bail and return the order if there are no matching webhooks */
  let stream =
    webhooks
    |> Most.flatMap((hook: Webhook.t) =>
         switch (hook.event) {
         | Unrecognized =>
           [|{webhook: hook, payload: Js.Json.null}|] |> Most.from
         | _ => fetch(hook, order)
         }
       )
    |> Most.map((_r: response)
         /* and here need to convert to order */
         /* let js = r.payload |> unsafeConvert;
            js |> Order.mapOrderFromJs */
         => order);
  /* stream
     |> Most.observe(_ =>
          Js.log("Processed stream for " ++ (event |> Webhook.EventType.toString))
        )
     |> ignore; */
  stream;
};