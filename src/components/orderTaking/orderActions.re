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
      cashier: Cashier.t,
      order: Order.orderVm,
      onFinish: Order.orderVm => unit,
    )
    : unit => {
  let totals =
    OrderItemCalculation.getTotals(order.discounts, order.orderItems);
  let paidOrder = {
    ...order,
    paid:
      Some({
        on: Date.now(),
        by: cashier.name,
        subTotal: totals.subTotal,
        tax: totals.tax,
        discount: totals.discounts,
        total: totals.total,
      }),
  };
  let stream =
    paidOrder |> Order.fromVm |> WebhookEngine.fireForOrder(BeforeOrderPaid);
  stream
  |> Most.observe((orderFromWebhook: Order.t) =>
       saveOrder(
         orderFromWebhook |> Order.toVm,
         (vm: Order.orderVm) => {
           orderFromWebhook |> WebhookEngine.fireForOrder(OrderPaid) |> ignore;
           onFinish(vm);
         },
       )
     )
  |> ignore;
};

let returnOrder =
    (
      cashier: Cashier.t,
      order: Order.orderVm,
      onFinish: Order.orderVm => unit,
    ) =>
  saveOrder(
    {...order, returned: Some({on: Date.now(), by: cashier.name})},
    (vm: Order.orderVm) => {
      vm
      |> Order.fromVm
      |> WebhookEngine.fireForOrder(OrderReturned)
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

type userIntent =
  | Paying
  | Building
  | Returning;

type state = {userIntent};

type action =
  | ChangeIntent(userIntent);

let component = ReasonReact.reducerComponent("OrderActions");

let make = (~order: Order.orderVm, ~onFinish, _children) => {
  ...component,
  initialState: () => {userIntent: Building},
  reducer: (action, _state) =>
    switch (action) {
    | ChangeIntent(intent) => ReasonReact.Update({userIntent: intent})
    },
  render: self => {
    let items = order.orderItems |> Array.of_list;
    let disablePayButton = items |> Array.length === 0;
    let saveButton =
      <Button
        local=true
        className="save-button-card"
        onClick=((_) => saveOrder(order, onFinish))
        label="action.save"
      />;
    let payButton =
      <Button
        local=true
        className="pay-button-card"
        disabled=disablePayButton
        onClick=((_) => self.send(ChangeIntent(Paying)))
        label="action.pay"
      />;
    let deleteButton =
      <Button
        local=true
        className="remove-button-card"
        onClick=((_) => removeOrder(order, onFinish))
        label="action.delete"
      />;
    let returnButton =
      <Button
        local=true
        className="quiet-card"
        onClick=((_) => self.send(ChangeIntent(Returning)))
        label="action.return"
      />;
    let cancelButton =
      <Button
        onClick=((_) => onFinish(order))
        local=true
        label="action.cancel"
      />;
    <div className="order-actions">
      (
        switch (self.state.userIntent, order.paid, order.returned, order.id) {
        | (Paying, _, _, _) =>
          <PinInput
            onFailure=(() => self.send(ChangeIntent(Building)))
            onSuccess=(cashier => payOrder(cashier, order, onFinish))
          />
        | (Returning, _, _, Some(_id)) =>
          <PinInput
            onFailure=(() => self.send(ChangeIntent(Building)))
            onSuccess=(cashier => returnOrder(cashier, order, onFinish))
          />
        | (Building, None, None, Some(_id)) =>
          <span> saveButton payButton deleteButton </span>
        | (Building, None, None, None) => <span> saveButton payButton </span>
        | (Building, Some(_paid), None, Some(_id)) => returnButton
        | (_, _, _, _) => ReasonReact.nullElement
        }
      )
      cancelButton
    </div>;
  },
};