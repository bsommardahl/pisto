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
    orderJs##orderItems
    |> Array.map(i => mapOrderItemFromJs(i))
    |> Array.to_list,
  createdOn: convertDate(orderJs##createdOn),
  discounts:
    orderJs##discounts
    |> Array.map(d => d |> Discount.mapFromJs)
    |> Array.to_list,
  paidOn: convertFloatOption(orderJs##paidOn),
  amountPaid: convertIntOption(orderJs##amountPaid),
  paymentTakenBy: convertStringOption(orderJs##paymentTakenBy),
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
      paidOn: o.paidOn,
      discounts: o.discounts,
      amountPaid: o.amountPaid,
      paymentTakenBy: o.paymentTakenBy,
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
  amountPaid: vm.amountPaid,
  paymentTakenBy: vm.paymentTakenBy,
  paidOn: vm.paidOn,
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
  "paidOn": Js.Nullable.fromOption(updateOrder.paidOn),
  "amountPaid": Js.Nullable.fromOption(updateOrder.amountPaid),
  "paymentTakenBy": Js.Nullable.fromOption(updateOrder.paymentTakenBy),
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