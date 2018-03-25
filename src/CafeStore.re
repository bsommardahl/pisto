

open Js.Promise;

let mapOrderFromJs = d => {
  let convertFloatOption = d => Js.Nullable.toOption(d);
  let convertStringOption = s => Js.Nullable.toOption(s);
  let convertDate = d => float_of_string(d);
  let mapOrderItemFromJs = i => {
    let prod = i##product;
    let product: Product.t = {
      id: prod##id,
      name: prod##name,
      tags: [],
      suggestedPrice: float_of_string(prod##suggestedPrice),
    };
    let item: OrderData.Order.orderItem = {
      product,
      addedOn: convertDate(i##addedOn),
      salePrice: float_of_string(i##salePrice),
    };
    item;
  };
  let order: OrderData.Order.order = {
    id: d##_id,
    customerName: d##customerName,
    orderItems:
      d##orderItems |> Array.map(i => mapOrderItemFromJs(i)) |> Array.to_list,
    createdOn: convertDate(d##createdOn),
    paidOn: convertFloatOption(d##paidOn),
    amountPaid: convertFloatOption(d##amountPaid),
    paymentTakenBy: convertStringOption(d##paymentTakenBy),
    lastUpdated: convertFloatOption(d##amountPaid),
    removed: false,
  };
  order;
};

let add =
    (newOrder: OrderData.Order.newOrder, db: Pouchdb.t)
    : Js.Promise.t(OrderData.Order.order) => {  
  db
  |> Pouchdb.post(newOrder)
  |> then_(revResponse => {
        db 
        |> Pouchdb.get(revResponse##id) 
        |> then_(order => {
          resolve(mapOrderFromJs(order));
        });
    });
  };


let getOpenOrders = (db: Pouchdb.t) : Js.Promise.t(list(OrderData.Order.order)) =>
  db
  |> Pouchdb.find(
       PouchDbInterface.QueryBuilder.query(
         ~selector={"paidOn": Js.Nullable.undefined},
         (),
       ),
     )
  |> Js.Promise.then_(response => Js.Promise.resolve(response##docs))
  |> Js.Promise.then_(docs => {
       let mapped: array(OrderData.Order.order) =
         docs |> Array.map(d => mapOrderFromJs(d));
       Js.Promise.resolve(mapped);
     })
  |> Js.Promise.then_(orders => Js.Promise.resolve(Array.to_list(orders)));

let getClosedOrders = (db: Pouchdb.t) : Js.Promise.t(list(OrderData.Order.order)) =>
  db
  |> Pouchdb.find(
       PouchDbInterface.QueryBuilder.query(
         ~selector={
           "paidOn": {
             "$not": Js.Nullable.undefined,
           },
         },
         (),
       ),
     )
  |> Js.Promise.then_(response => Js.Promise.resolve(response##docs))
  |> Js.Promise.then_(docs => {
       let mapped: array(OrderData.Order.order) =
         docs |> Array.map(d => mapOrderFromJs(d));
       Js.Promise.resolve(mapped);
     })
  |> Js.Promise.then_(orders => Js.Promise.resolve(Array.to_list(orders)));

let update = (updateOrder: OrderData.Order.updateOrder, db: Pouchdb.t) : Js.Promise.t(OrderData.Order.order) =>
  db
  |> Pouchdb.get(updateOrder.id)
  |> Js.Promise.then_(orderJs => {
       let rev = orderJs##rev;
       db
       |> Pouchdb.put({
            "_id": orderJs##id,
            "_rev": rev,
            "orderItems": updateOrder.orderItems,
            "customerName": updateOrder.customerName,
            "lastUpdated": Js.Date.now,
          })
       |> ignore;
       Js.Promise.resolve(mapOrderFromJs(orderJs));
     });

let get = (orderId: string, db: Pouchdb.t) : Js.Promise.t(OrderData.Order.order) =>
  db
  |> Pouchdb.get(orderId)
  |> then_(order => resolve(mapOrderFromJs(order)));

let remove = (orderId: string, db: Pouchdb.t) : Js.Promise.t(unit) =>
  db
  |> Pouchdb.get(orderId)
  |> then_(order => {
    db 
    |> Pouchdb.remove(order) 
    |> then_(_ => resolve());
  });
