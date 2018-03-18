[@bs.val] external unsafeJsonParse : string => 'a = "JSON.parse";

let namespace = "cafe-orders";

let retrieveAllOrders = () : list(OrderData.Order.order) =>
  Dom.Storage.(
    switch (localStorage |> getItem(namespace)) {
    | Some(json) => unsafeJsonParse(json)
    | None => []
    }
  );

let persistAllOrders = (allOrders: list(OrderData.Order.order)) =>
  Dom.Storage.(
    switch (allOrders |> Js.Json.stringifyAny) {
    | Some(json) => setItem(namespace, json, localStorage)
    | None => ()
    }
  );

let getNextId = (allOrders: list(OrderData.Order.order)) : int => {
  let highestId =
    allOrders
    |> List.map((o: OrderData.Order.order) =>
         switch (o.id) {
         | None => 0
         | Some(id) => id
         }
       )
    |> List.sort(Js.Math.max_int)
    |> List.hd;
  highestId + 1;
};

let add =
    (order: OrderData.Order.order, allOrders: list(OrderData.Order.order)) =>
  List.concat([[order], allOrders]);

let update =
    (order: OrderData.Order.order, allOrders: list(OrderData.Order.order)) =>
  /* the way I'm doing this is not working. I think I need to convert to an array and replace the member directly by it's index. */
  allOrders
  |> List.map((o: OrderData.Order.order) =>
       if (Some(o.id) == Some(order.id)) {
         order;
       } else {
         o;
       }
     );

let find =
    (orderId: int, allOrders: list(OrderData.Order.order))
    : OrderData.Order.order =>
  allOrders |> List.find((o: OrderData.Order.order) => o.id == Some(orderId));

let remove = (orderId: int, allOrders: list(OrderData.Order.order)) =>
  allOrders
  |> List.map((o: OrderData.Order.order) =>
       switch (o.id) {
       | Some(id) =>
         if (id == orderId) {
           {...o, removed: true};
         } else {
           o;
         }
       | None => o
       }
     );