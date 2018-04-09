open OrderData;

let convertFloatOption = d : option(float) => Js.Nullable.toOption(d);

let convertIntOption = d : option(int) => Js.Nullable.toOption(d);

let convertStringOption = s : option(string) => Js.Nullable.toOption(s);

let convertDate = d => d;

let mapOrderItemFromJs = itemJs : OrderData.Order.orderItem => {
  sku: itemJs##sku,
  name: itemJs##name,
  suggestedPrice: itemJs##suggestedPrice,
  addedOn: convertDate(itemJs##addedOn),
  salePrice: itemJs##salePrice,
  taxCalculation: itemJs##taxCalculation |> Tax.Calculation.toMethod,
};

let mapOrderFromJs = orderJs : OrderData.Order.order => {
  id: orderJs##_id,
  customerName: orderJs##customerName,
  orderItems:
    switch (Js.Nullable.toOption(orderJs##orderItems)) {
    | Some(orderItems) =>
      orderItems |> Array.map(i => mapOrderItemFromJs(i)) |> Array.to_list
    | None => []
    },
  createdOn: convertDate(orderJs##createdOn),
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
  lastUpdated: convertFloatOption(orderJs##lastUpdated),
  removed: false,
};

let vmFromExistingOrder = (o: OrderData.Order.order) => {
  let vm: OrderData.Order.orderVm =
    OrderData.Order.{
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

let vmToUpdateOrder = (vm: Order.orderVm) : Order.updateOrder => {
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

let orderItemToJs = (orderItem: OrderData.Order.orderItem) => {
  "sku": orderItem.sku,
  "name": orderItem.name,
  "suggestedPrice": orderItem.suggestedPrice,
  "addedOn": orderItem.addedOn,
  "salePrice": orderItem.salePrice,
  "taxCalculation":
    orderItem.taxCalculation |> Tax.Calculation.toDelimitedString,
};

let updateOrderToJs =
    (
      id: string,
      rev: string,
      originalOrder: OrderData.Order.order,
      updateOrder: OrderData.Order.updateOrder,
    ) => {
  "_id": id,
  "_rev": rev,
  "orderItems":
    updateOrder.orderItems |> List.map(i => orderItemToJs(i)) |> Array.of_list,
  "customerName": updateOrder.customerName,
  "lastUpdated": Js.Date.now(),
  "discounts":
    updateOrder.discounts
    |> List.map(d => d |> Discount.mapToJs)
    |> Array.of_list,
  "paid":
    switch (updateOrder.paid) {
    | None => Js.Nullable.null
    | Some(paid) => Js.Nullable.return(paid |> Paid.toJs)
    },
  "createdOn": originalOrder.createdOn,
};

let newOrderToJs = (order: OrderData.Order.newOrder) => {
  "customerName": order.customerName,
  "orderItems":
    order.orderItems |> List.map(i => orderItemToJs(i)) |> Array.of_list,
  "discounts":
    order.discounts |> List.map(d => d |> Discount.mapToJs) |> Array.of_list,
  "createdOn": Js.Date.now(),
};