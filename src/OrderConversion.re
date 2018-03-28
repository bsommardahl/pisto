open OrderData;

let convertFloatOption = d : option(float) => Js.Nullable.toOption(d);

let convertIntOption = d : option(int) => Js.Nullable.toOption(d);

let convertStringOption = s : option(string) => Js.Nullable.toOption(s);

let convertDate = d => d;

let mapOrderItemFromJs = itemJs : OrderData.Order.orderItem => {
  productCode: itemJs##productCode,
  name: itemJs##name,
  suggestedPrice: itemJs##suggestedPrice,
  addedOn: convertDate(itemJs##addedOn),
  salePrice: itemJs##salePrice,
  taxCalculation:
    switch (itemJs##taxCalculation |> Js.String.split("|")) {
    | [|"totalFirst", rate|] => Tax.TotalFirst(int_of_string(rate))
    | [|"subTotalFirst", rate|] => Tax.SubTotalFirst(int_of_string(rate))
    | _ => Tax.Exempt
    },
};

let mapOrderFromJs = orderJs : OrderData.Order.order => {
  id: orderJs##_id,
  customerName: orderJs##customerName,
  orderItems:
    orderJs##orderItems
    |> Array.map(i => mapOrderItemFromJs(i))
    |> Array.to_list,
  createdOn: convertDate(orderJs##createdOn),
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

let orderItemToJs = (orderItem: OrderData.Order.orderItem) => {
  "productCode": orderItem.productCode,
  "name": orderItem.name,
  "suggestedPrice": orderItem.suggestedPrice,
  "addedOn": orderItem.addedOn,
  "salePrice": orderItem.salePrice,
  "taxCalculation":
    switch (orderItem.taxCalculation) {
    | TotalFirst(rate) => "totalFirst|" ++ string_of_int(rate)
    | SubTotalFirst(rate) => "subTotalFirst|" ++ string_of_int(rate)
    | Exempt => "exempt|"
    },
};

let updateOrderToJs =
    (id: string, rev: string, updateOrder: OrderData.Order.updateOrder) => {
  "_id": id,
  "_rev": rev,
  "orderItems":
    updateOrder.orderItems |> List.map(i => orderItemToJs(i)) |> Array.of_list,
  "customerName": updateOrder.customerName,
  "lastUpdated": Js.Date.now(),
  "paidOn": Js.Nullable.fromOption(updateOrder.paidOn),
  "amountPaid": Js.Nullable.fromOption(updateOrder.amountPaid),
  "paymentTakenBy": Js.Nullable.fromOption(updateOrder.paymentTakenBy),
};

let newOrderToJs = (order: OrderData.Order.newOrder) => {
  "customerName": order.customerName,
  "orderItems":
    order.orderItems |> List.map(i => orderItemToJs(i)) |> Array.of_list,
  "createdOn": Js.Date.now(),
};