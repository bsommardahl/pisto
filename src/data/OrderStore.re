open Js.Promise;

open PouchdbImpl;

open OrderConversion;

let db = PouchdbImpl.connect("orders", Config.Database.livePouchDbConfig);

let add = (newOrder: Order.newOrder) : Js.Promise.t(Order.t) =>
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

module OrderPaidQuery = {
  type t = {
    .
    "paid": {
      .
      "$exists": Js.Nullable.t(bool),
      /* "on":
         Js.Nullable.t(
           {
             .
             "$gt": float,
             "$lt": float,
           },
         ), */
    },
  };
  let orderIsPaid = (existing: bool) : t => {
    "paid": {
      "$exists": Js.Nullable.return(existing),
    },
    /* "on": Js.Nullable.undefined, */
  };
  let orderPaidDateInRange = (startDate, endDate) : t => {
    "paid": {
      "$exists": Js.Nullable.return(true),
    },
    /* "on": Js.Nullable.return({"$gt": startDate, "$lt": endDate}), */
  };
};

let getOpenOrders = () =>
  db
  |> find(
       Pouchdb.QueryBuilder.query(
         ~selector=OrderPaidQuery.orderIsPaid(false),
         (),
       ),
     )
  |> Js.Promise.then_(response => Js.Promise.resolve(response##docs))
  |> Js.Promise.then_(docs => {
       let mapped: array(Order.t) =
         docs |> Array.map(d => mapOrderFromJs(d));
       Js.log(
         "orderStore:: got open orders: "
         ++ string_of_int(docs |> Array.length),
       );
       Js.Promise.resolve(mapped);
     })
  |> Js.Promise.then_(orders => Js.Promise.resolve(Array.to_list(orders)));

let getClosedOrders = (startDate: float, endDate: float) => {
  /* let sel = {
       "$and": [
         OrderPaidQuery.orderIsPaid(true),
         OrderPaidQuery.orderPaidDateInRange(startDate, endDate),
       ],
     }; */
  let sel = OrderPaidQuery.orderIsPaid(true);
  db
  |> find(Pouchdb.QueryBuilder.query(~selector=sel, ()))
  |> Js.Promise.then_(response => Js.Promise.resolve(response##docs))
  |> Js.Promise.then_(docs => {
       let mapped: array(Order.t) =
         docs |> Array.map(d => mapOrderFromJs(d));
       Js.log(
         "orderStore:: got closed orders: "
         ++ string_of_int(docs |> Array.length),
       );
       Js.Promise.resolve(mapped);
     })
  |> Js.Promise.then_(orders => Js.Promise.resolve(Array.to_list(orders)));
};

let update = (updateOrder: Order.updateOrder) : Js.Promise.t(Order.t) =>
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