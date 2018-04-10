type updateOrder = {
  id: string,
  customerName: string,
  orderItems: list(OrderItem.t),
  discounts: list(Discount.t),
  paid: option(Paid.t),
};

type t = {
  id: string,
  customerName: string,
  paid: option(Paid.t),
  orderItems: list(OrderItem.t),
  discounts: list(Discount.t),
  createdOn: float,
  lastUpdated: option(float),
  removed: bool,
};

type orderVm = {
  id: option(string),
  customerName: string,
  orderItems: list(OrderItem.t),
  discounts: list(Discount.t),
  paid: option(Paid.t),
  createdOn: float,
  lastUpdated: option(float),
  removed: bool,
};

type newOrder = {
  customerName: string,
  orderItems: list(OrderItem.t),
  discounts: list(Discount.t),
  paid: option(Paid.t),
};

let mapOrderFromJs = orderJs : t => {
  id: orderJs##_id,
  customerName: orderJs##customerName,
  orderItems:
    switch (Js.Nullable.toOption(orderJs##orderItems)) {
    | Some(orderItems) =>
      orderItems
      |> Array.map(i => OrderItem.mapOrderItemFromJs(i))
      |> Array.to_list
    | None => []
    },
  createdOn: JsUtils.convertDate(orderJs##createdOn),
  discounts:
    switch (Js.Nullable.toOption(orderJs##discounts)) {
    | Some(discounts) =>
      discounts |> Array.map(d => d |> Discount.mapFromJs) |> Array.to_list
    | None => []
    },
  paid:
    switch (Js.Nullable.toOption(orderJs##paid)) {
    | None => None
    | Some(js) => Some(js |> Paid.fromJs)
    },
  lastUpdated: JsUtils.convertFloatOption(orderJs##lastUpdated),
  removed: false,
};

let vmFromExistingOrder = (o: t) => {
  let vm: orderVm = {
    id: Some(o.id),
    customerName: o.customerName,
    orderItems: o.orderItems,
    createdOn: o.createdOn,
    discounts: o.discounts,
    paid: o.paid,
    lastUpdated: o.lastUpdated,
    removed: false,
  };
  vm;
};

let vmToUpdateOrder = (vm: orderVm) : updateOrder => {
  id:
    switch (vm.id) {
    | None => ""
    | Some(id) => id
    },
  customerName: vm.customerName,
  orderItems: vm.orderItems,
  discounts: vm.discounts,
  paid: vm.paid,
};

let updateOrderToJs =
    (id: string, rev: string, originalOrder: t, updateOrder: updateOrder) => {
  "_id": id,
  "_rev": rev,
  "orderItems":
    updateOrder.orderItems
    |> List.map(i => OrderItem.orderItemToJs(i))
    |> Array.of_list,
  "customerName": updateOrder.customerName,
  "lastUpdated": Js.Date.now(),
  "discounts":
    updateOrder.discounts
    |> List.map(d => d |> Discount.mapToJs)
    |> Array.of_list,
  "paid":
    switch (updateOrder.paid) {
    | None => Js.Nullable.undefined
    | Some(paid) => Js.Nullable.return(paid |> Paid.toJs)
    },
  "createdOn": originalOrder.createdOn,
};

let newOrderToJs = (order: newOrder) => {
  "customerName": order.customerName,
  "orderItems":
    order.orderItems
    |> List.map(i => OrderItem.orderItemToJs(i))
    |> Array.of_list,
  "discounts":
    order.discounts |> List.map(d => d |> Discount.mapToJs) |> Array.of_list,
  "createdOn": Js.Date.now(),
  "paid":
    switch (order.paid) {
    | None => Js.Nullable.undefined
    | Some(paid) => Js.Nullable.return(paid |> Paid.toJs)
    },
};