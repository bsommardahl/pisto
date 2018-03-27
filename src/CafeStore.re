open Js.Promise;

type productJs = {
  .
  "id": string,
  "name": string,
  "tags": list(string),
  "suggestedPrice": float,
};

type orderItemJs = {
  .
  "product": productJs,
  "addedOn": float,
  "salePrice": float,
};

type orderJs = {
  .
  "_id": string,
  "_rev": string,
  "customerName": string,
  "amountPaid": Js.Nullable.t(float),
  "orderItems": list(orderItemJs),
  "paymentTakenBy": Js.Nullable.t(string),
  "paidOn": Js.Nullable.t(float),
  "createdOn": float,
  "lastUpdated": Js.Nullable.t(float),
  "removed": bool,
};

module Make = (Db: Pouchdb.init) => {
  let mapOrderFromJs = (d: orderJs) => {
    let convertFloatOption = d => Js.Nullable.toOption(d);
    let convertStringOption = s => Js.Nullable.toOption(s);
    let convertDate = d => d;
    let mapOrderItemFromJs = (i: orderItemJs) => {
      let prod: productJs = i##product;
      let product: Product.t = {
        id: prod##id,
        name: prod##name,
        tags: [],
        suggestedPrice: prod##suggestedPrice,
      };
      let item: OrderData.Order.orderItem = {
        product,
        addedOn: convertDate(i##addedOn),
        salePrice: i##salePrice,
      };
      item;
    };
    let order: OrderData.Order.order = {
      id: d##_id,
      customerName: d##customerName,
      orderItems: d##orderItems |> List.map(i => mapOrderItemFromJs(i)),
      createdOn: convertDate(d##createdOn),
      paidOn: convertFloatOption(d##paidOn),
      amountPaid: convertFloatOption(d##amountPaid),
      paymentTakenBy: convertStringOption(d##paymentTakenBy),
      lastUpdated: convertFloatOption(d##lastUpdated),
      removed: false,
    };
    order;
  };
  let dbUrl = "http://localhost:5984/orders";
  let add =
      (newOrder: OrderData.Order.newOrder)
      : Js.Promise.t(OrderData.Order.order) => {
    let db = Db.connect(dbUrl);
    db
    |> db.post(newOrder)
    |> then_(revResponse =>
         db
         |> db.get(revResponse##id)
         |> then_(order => {
              let o = mapOrderFromJs(order);
              resolve(o);
            })
       );
  };
  let getOpenOrders = db : Js.Promise.t(list(OrderData.Order.order)) => {
    /* let db = Db.connect(dbUrl);
    db
    |> db.find(
         Pouchdb.QueryBuilder.query(
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
    |> Js.Promise.then_(orders => Js.Promise.resolve(Array.to_list(orders))); */
    Js.Promise.resolve([]);
  };
  let getClosedOrders = db : Js.Promise.t(list(OrderData.Order.order)) =>
    /* db
    |> db##find(
         Pouchdb.QueryBuilder.query(
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
    |> Js.Promise.then_(orders => Js.Promise.resolve(Array.to_list(orders))); */
    Js.Promise.resolve([]);
  let update =
      (updateOrder: OrderData.Order.updateOrder, db)
      : Js.Promise.t(OrderData.Order.order) =>
    db
    |> db##get(updateOrder.id)
    |> Js.Promise.then_((orderJs: orderJs) => {
         let rev = orderJs##_rev;
         db
         |> db##put({
              "_id": orderJs##_id,
              "_rev": rev,
              "orderItems": updateOrder.orderItems,
              "customerName": updateOrder.customerName,
              "lastUpdated": Js.Date.now,
            })
         |> ignore;
         Js.Promise.resolve(mapOrderFromJs(orderJs));
       });
  let get = (orderId: string, db) : Js.Promise.t(OrderData.Order.order) =>
    db |> db##get(orderId) |> then_(order => resolve(mapOrderFromJs(order)));
  let remove = (orderId: string, db) : Js.Promise.t(unit) =>
    db
    |> db##get(orderId)
    |> then_(order => db |> db##remove(order) |> then_((_) => resolve()));
};