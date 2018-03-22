open Pouchdb;
/*
 /* not sure this is needed anymore. Depends on how I can generate order ids. */
 let getNextId = () : int => {
   /* let highestId =
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
   highestId + 1; */
   1;
 };
*/
 let add = (order: OrderData.Order.order, db: PouchDBConnection.t): Js.Promise.t(OrderData.Order.order) => {
   db |> PouchDBConnection.post(order) |> Js.Promise.then_((response) => {
     Js.log(response);
      Js.Promise.resolve(order);
   });
 };
/*
 type selector = { paidOn: option(float) };

 type findOrders = {
   selector: selector
 };

 let getOpenOrders = (db: PouchDBConnection.t) : list(OrderData.Order.order) => {
   let orders = PouchDBConnection.find({ selector: { paidOn: None } }, db)
     |> Js.Promise.then_(o=> {
       let orders = list(o.docs);
       order;
     });
   /* Js.Console.log(
     string_of_int(orders |> List.length) ++ " open orders found.",
   ); */
   orders;
 };

 /* let getClosedOrders =
     (db: PouchDBConnection.t)
     : list(OrderData.Order.order) => {
   let orders = PouchDBConnection.find({ selector: { paidOn: Some(float) }}, db)
   |> Js.Promise.then_(o=> {
     o.docs;
   });
   Js.Console.log(
     string_of_int(orders |> List.length) ++ " open orders found.",
   );
   orders;
 }; */

 let update =
     (order: OrderData.Order.order, db: PouchDBConnection.t) => {
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
     (orderId: int, db: PouchDBConnection.t)
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

 let remove = (orderId: int,  db: PouchDBConnection.t) => {
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
 }; */