[@bs.val] external unsafeJsonParse : string => 'a = "JSON.parse";

let namespace = "cafe-orders";

let retrieveAllOrders = () : list(OrderData.Order.order) => {
  let orders =
    Dom.Storage.(
      switch (localStorage |> getItem(namespace)) {
      | Some(json) => unsafeJsonParse(json)
      | None => []
      }
    );
  Js.Console.log(
    string_of_int(orders |> List.length) ++ " orders retrieved.",
  );
  /* Js.Console.log(Js.Json.stringify(Array.of_list(orders))); */
  orders;
};

let persistAllOrders = (allOrders: list(OrderData.Order.order)) => {
  Dom.Storage.(
    switch (allOrders |> Js.Json.stringifyAny) {
    | Some(json) => setItem(namespace, json, localStorage)
    | None => ()
    }
  );
  Js.Console.log(
    string_of_int(allOrders |> List.length) ++ " orders persisted.",
  );
};

let getNextId = (allOrders: list(OrderData.Order.order)) : int => {
  let highestId =
    switch (allOrders |> List.length) {
    | 0 => 0
    | _ =>
      allOrders
      |> List.map((o: OrderData.Order.order) =>
           switch (o.id) {
           | None => 0
           | Some(id) => id
           }
         )
      |> List.sort(Js.Math.max_int)
      |> List.hd
    };
  Js.Console.log("new id generated");
  highestId + 1;
};

let add =
    (order: OrderData.Order.order, allOrders: list(OrderData.Order.order)) => {
  let orders = List.concat([[order], allOrders]);
  Js.Console.log("Order added to array.");
  orders;
};

let getOpenOrders =
    (allOrders: list(OrderData.Order.order))
    : list(OrderData.Order.order) => {
  let orders =
    allOrders
    |> List.filter((o: OrderData.Order.order) =>
         switch (OrderData.Order.(o.paidOn)) {
         | None => true
         | Some(paidOn) =>
           Js.Console.log(string_of_float(paidOn));
           false;
         }
       );
  Js.Console.log(
    string_of_int(orders |> List.length) ++ " open orders found.",
  );
  orders;
};

let getClosedOrders =
    (allOrders: list(OrderData.Order.order))
    : list(OrderData.Order.order) => {
  let orders =
    allOrders
    |> List.filter((o: OrderData.Order.order) =>
         switch (OrderData.Order.(o.paidOn)) {
         | None => false
         | Some(paidOn) =>
           Js.Console.log(string_of_float(paidOn));
           true;
         }
       );
  Js.Console.log(
    string_of_int(orders |> List.length) ++ " closed orders found.",
  );
  orders;
};

let update =
    (order: OrderData.Order.order, allOrders: list(OrderData.Order.order)) => {
  let orders =
    allOrders
    |> List.map((o: OrderData.Order.order) =>
         if (Some(o.id) == Some(order.id)) {
           order;
         } else {
           o;
         }
       );
  Js.Console.log("Order updated.");
  orders;
};

let find =
    (orderId: int, allOrders: list(OrderData.Order.order))
    : OrderData.Order.order => {
  let orders =
    allOrders
    |> List.find((o: OrderData.Order.order) => o.id == Some(orderId));
  switch (orders) {
  | _ => Js.Console.log("1 order found.")
  | exception Not_found => Js.Console.log("No orders found.")
  };
  orders;
};

let remove = (orderId: int, allOrders: list(OrderData.Order.order)) => {
  let orders =
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
  Js.Console.log(
    "Order removed. " ++ string_of_int(orders |> List.length) ++ " left.",
  );
  orders;
};