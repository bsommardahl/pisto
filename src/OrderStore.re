open Js.Promise;

open PouchdbImpl;

open OrderConversion;

open OrderData;

let db = PouchdbImpl.connect("orders", Config.Database.livePouchDbConfig);

let add = (newOrder: Order.newOrder) : Js.Promise.t(Order.order) =>
  db
  |> post(newOrder |> newOrderToJs)
  |> then_(revResponse =>
       db
       |> get(revResponse##id)
       |> then_(order => {
            let o = mapOrderFromJs(order);
            Js.log(
              "orderStore:: added new order for " ++ newOrder.customerName,
            );
            resolve(o);
          })
     );

type paidExists = {. "paid": {. "$exists": bool}};

let orderIsPaid = (existing: bool) : paidExists => {
  "paid": {
    "$exists": existing,
  },
};

let getOpenOrders = () =>
  db
  |> find(Pouchdb.QueryBuilder.query(~selector=orderIsPaid(false), ()))
  |> Js.Promise.then_(response => Js.Promise.resolve(response##docs))
  |> Js.Promise.then_(docs => {
       let mapped: array(Order.order) =
         docs |> Array.map(d => mapOrderFromJs(d));
       Js.log(
         "orderStore:: got open orders: "
         ++ string_of_int(docs |> Array.length),
       );
       Js.Promise.resolve(mapped);
     })
  |> Js.Promise.then_(orders => Js.Promise.resolve(Array.to_list(orders)));

type paidInDateRange = {
  .
  "paid": {
    .
    "on": {
      .
      "$gt": float,
      "$lt": float,
    },
  },
};

let orderPaidDateInRange =
    (startDate: float, endDate: float)
    : paidInDateRange => {
  "paid": {
    "on": {
      "$gt": startDate,
      "$lt": endDate,
    },
  },
};

let getClosedOrders = (startDate: float, endDate: float) =>
  db
  |> find(
       Pouchdb.QueryBuilder.query(
         ~selector={
           "$and": [
             orderIsPaid(true),
             orderPaidDateInRange(startDate, endDate),
           ],
         },
         (),
       ),
     )
  |> Js.Promise.then_(response => Js.Promise.resolve(response##docs))
  |> Js.Promise.then_(docs => {
       let mapped: array(Order.order) =
         docs |> Array.map(d => mapOrderFromJs(d));
       Js.log(
         "orderStore:: got closed orders: "
         ++ string_of_int(docs |> Array.length),
       );
       Js.Promise.resolve(mapped);
     })
  |> Js.Promise.then_(orders => Js.Promise.resolve(Array.to_list(orders)));

let update = (updateOrder: Order.updateOrder) : Js.Promise.t(Order.order) =>
  db
  |> get(updateOrder.id)
  |> Js.Promise.then_(orderJs => {
       let rev = orderJs##_rev;
       let id = orderJs##_id;
       db
       |> put(
            updateOrderToJs(
              id,
              rev,
              orderJs |> OrderConversion.mapOrderFromJs,
              updateOrder,
            ),
          )
       |> ignore;
       Js.log("orderStore:: updated order for " ++ orderJs##customerName);
       Js.Promise.resolve(mapOrderFromJs(orderJs));
     });

let get = (orderId: string) =>
  db
  |> get(orderId)
  |> then_(js => {
       let order = mapOrderFromJs(js);
       Js.log("orderStore:: got order for " ++ js##customerName);
       Js.Promise.resolve(order);
     });

let remove = (orderId: string) : Js.Promise.t(unit) =>
  db
  |> PouchdbImpl.get(orderId)
  |> then_(order =>
       db
       |> remove(order)
       |> then_((_) => {
            Js.log("orderStore:: removed order with id: " ++ orderId);
            resolve();
          })
     );