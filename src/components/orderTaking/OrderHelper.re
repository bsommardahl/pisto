let saveOrder = (order: Order.orderVm, onFinish: Order.orderVm => unit) : unit => {
  Js.Console.log("orderActions:: Persisting order....");
  switch (order.id) {
  | None =>
    OrderStore.add({
      discounts: order.discounts,
      customerName: order.customerName,
      orderItems: order.orderItems,
      paid: order.paid,
    })
    |> Js.Promise.then_(freshOrder => {
         onFinish(freshOrder |> Order.vmFromExistingOrder);
         Js.Promise.resolve(
           Js.Console.log("orderActions:: Added new order."),
         );
       })
    |> ignore;
    ();
  | Some(_id) =>
    let o: Order.updateOrder = Order.vmToUpdateOrder(order);
    OrderStore.update(o)
    |> Js.Promise.then_(updatedOrder => {
         onFinish(updatedOrder |> Order.vmFromExistingOrder);
         Js.Promise.resolve(
           Js.Console.log("orderActions:: Updated existing order."),
         );
       })
    |> ignore;
    ();
  };
};

let payOrder =
    (
      cashier: option(Cashier.t),
      order: Order.orderVm,
      method: PaymentMethod.t,
      externalId: string,
      onFinish: Order.orderVm => unit,
    )
    : unit => {
  let totals =
    OrderItemCalculation.getTotals(order.discounts, order.orderItems);
  let paidOrder = {
    ...order,
    paid:
      Some({
        on: ConfigurableDate.now(),
        by:
          switch (cashier) {
          | None => "n/a"
          | Some(c) => c.name
          },
        subTotal: totals.subTotal,
        tax: totals.tax,
        discount: totals.discounts,
        total: totals.total,
        externalId: "",
        method: PaymentMethod.default,
      }),
  };
  WebhookEngine.getWebhooks(BeforeOrderPaid, Order)
  |> WebhookEngine.fire(paidOrder |> Order.fromVm |> Order.toJs)
  |> Js.Promise.then_((responses: list(WebhookEngine.response)) => {
       let awaitResponses =
         responses
         |> List.map((r: WebhookEngine.response) =>
              switch (r.webhook.behavior, r.payload) {
              | (Webhook.Behavior.AwaitResponse, Some(payload)) =>
                Js.log(payload);
                [
                  payload
                  |> WebhookEngine.unsafeConvert
                  |> Order.mapOrderFromJs
                  |> Order.toVm,
                ];
              | (_, _) => []
              }
            )
         |> List.concat;
       (
         switch (awaitResponses |> List.length) {
         | 0 => paidOrder
         | _ => awaitResponses |. List.nth(0)
         }
       )
       |> Js.Promise.resolve;
     })
  |> Js.Promise.then_((orderFromWebhook: Order.orderVm) =>
       saveOrder(
         orderFromWebhook,
         (vm: Order.orderVm) => {
           WebhookEngine.getWebhooks(OrderPaid, Order)
           |> WebhookEngine.fire(
                orderFromWebhook |> Order.fromVm |> Order.toJs,
              )
           |> ignore;
           onFinish(vm);
         },
       )
       |> Js.Promise.resolve
     )
  |> ignore;
  ();
};

let returnOrder =
    (
      cashier: Cashier.t,
      order: Order.orderVm,
      onFinish: Order.orderVm => unit,
    ) =>
  saveOrder(
    {
      ...order,
      returned: Some({on: ConfigurableDate.now(), by: cashier.name}),
    },
    (vm: Order.orderVm) => {
      WebhookEngine.getWebhooks(OrderReturned, Order)
      |> WebhookEngine.fire(vm |> Order.fromVm |> Order.toJs)
      |> ignore;
      onFinish(vm);
    },
  );

let removeOrder = (order: Order.orderVm, onFinish: Order.orderVm => unit) =>
  switch (order.id) {
  | None => onFinish(order)
  | Some(id) =>
    OrderStore.remove(id)
    |> Js.Promise.then_(() => {
         onFinish(order);
         Js.Promise.resolve(Js.Console.log("Removed order."));
       })
    |> ignore;
    ();
  };

let buildOrderItem = (product: Product.t, id: string) : OrderItem.t => {
  id,
  quantity: 1,
  sku: product.sku,
  name: product.name,
  suggestedPrice: product.suggestedPrice,
  addedOn: ConfigurableDate.now(),
  salePrice: product.suggestedPrice,
  taxCalculation: product.taxCalculation,
};

let buildNewOrder = (customerName: string) : Order.orderVm => {
  let order: Order.orderVm = {
    id: None,
    customerName,
    orderItems: [],
    createdOn: ConfigurableDate.now(),
    discounts: [],
    paid: None,
    returned: None,
    lastUpdated: None,
    removed: false,
    meta: None,
  };
  /* order |> Order.fromVm |> WebhookEngine.fireForOrder(OrderStarted) |> ignore; */
  order;
};

let getOrderVm = orderId =>
  OrderStore.get(orderId)
  |> Js.Promise.then_(order => {
       let vm = Order.vmFromExistingOrder(order);
       Js.Promise.resolve(vm);
     });