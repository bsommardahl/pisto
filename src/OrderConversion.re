open OrderData;

let convertFloatOption = d => Js.Nullable.toOption(d);

let convertStringOption = s => Js.Nullable.toOption(s);

let convertDate = d => d;

let mapProductFromJs = prod : Product.t => {
  id: prod##id,
  name: prod##name,
  tags: prod##tags |> Array.to_list,
  suggestedPrice: prod##suggestedPrice,
};

let mapOrderItemFromJs = item : OrderData.Order.orderItem => {
  product: mapProductFromJs(item##product),
  addedOn: convertDate(item##addedOn),
  salePrice: item##salePrice,
};

let mapOrderFromJs = orderJs : OrderData.Order.order => {
  id: orderJs##_id,
  customerName: orderJs##customerName,
  orderItems: orderJs##orderItems |> List.map(i => mapOrderItemFromJs(i)),
  createdOn: convertDate(orderJs##createdOn),
  paidOn: convertFloatOption(orderJs##paidOn),
  amountPaid: convertFloatOption(orderJs##amountPaid),
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
  amountPaid: vm.amountPaid,
  paymentTakenBy: vm.paymentTakenBy,
  paidOn: vm.paidOn,
};

let productToJs = (product: Product.t) => {
  "id": product.id,
  "name": product.name,
  "tags": product.tags |> Array.of_list,
  "suggestedPrice": product.suggestedPrice,
};

let orderItemToJs = (orderItem: OrderData.Order.orderItem) => {
  "product": productToJs(orderItem.product),
  "addedOn": orderItem.addedOn,
  "salePrice": orderItem.salePrice,
};

let newOrderToJs = (order: OrderData.Order.newOrder) => {
  "customerName": order.customerName,
  "orderItems":
    order.orderItems |> List.map(i => orderItemToJs(i)) |> Array.of_list,
  "createdOn": Js.Date.now(),
};