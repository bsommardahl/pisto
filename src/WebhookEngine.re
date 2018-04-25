type eventType =
  | OrderPaid
  | OrderReturned
  | ReprintReceipt
  | PrintOrder
  | Error;

let toString = e =>
  switch (e) {
  | OrderPaid => "OrderPaid"
  | OrderReturned => "OrderReturned"
  | ReprintReceipt => "ReprintReceipt"
  | PrintOrder => "PrintOrder"
  | Error => "Error"
  };

let fire = (eventName: string, payload: option(string)) =>
  WebhookStore.getAll()
  |> Js.Promise.then_((webhooks: list(Webhook.t)) => {
       webhooks
       |> List.filter((x: Webhook.t) => x.event === eventName)
       |> List.iter((hook: Webhook.t) => {
            Js.log(hook);
            Resync.(
              Refetch.(
                request(
                  `POST,
                  hook.url,
                  ~headers=[`ContentType("application/json")],
                  ~body=
                    `String(
                      switch (payload) {
                      | None => "{}"
                      | Some(json) => json
                      },
                    ),
                )
                |> fetch
                |> Future.flatMap(
                     fun
                     | Response.Ok(_, response) => Response.text(response)
                     | Response.Error({reason}, _) => Future.from(reason),
                   )
                |> Future.whenResolved(Js.log)
              )
            );
          });
       Js.Promise.resolve();
     })
  |> ignore;

let fireFor = (event: eventType, order: Order.t) : unit => {
  let eventName = event |> toString;
  let payload =
    {"order": order |> Order.toJs, "event": eventName} |> Js.Json.stringifyAny;
  fire(eventName, payload);
  ();
};

let fireForAny = (event: eventType, payload: 'a) : unit => {
  {"payload": payload, "event": event |> toString}
  |> Js.Json.stringifyAny
  |> fire(event |> toString);
  ();
};