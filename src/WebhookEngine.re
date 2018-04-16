type eventType =
  | OrderPaid
  | OrderReturned;

let toString = e =>
  switch (e) {
  | OrderPaid => "OrderPaid"
  | OrderReturned => "OrderReturned"
  };

let fireFor = (event: eventType, order: Order.t) : unit => {
  let eventName = event |> toString;
  WebhookStore.getAll()
  |> Js.Promise.then_((webhooks: list(Webhook.t)) => {
       let payload =
         {"order": order |> Order.toJs, "event": eventName}
         |> Js.Json.stringifyAny;
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
  ();
};