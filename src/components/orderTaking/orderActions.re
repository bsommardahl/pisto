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

type state = {
  paying: bool,
  returning: bool,
};

type action =
  | StartPaying
  | StopPaying
  | StartReturning
  | StopReturning;

let component = ReasonReact.reducerComponent("OrderActions");

let make = (~order: Order.orderVm, ~onFinish, _children) => {
  ...component,
  initialState: () => {paying: false, returning: false},
  reducer: (action, state) =>
    switch (action) {
    | StartPaying => ReasonReact.Update({...state, paying: true})
    | StopPaying => ReasonReact.Update({...state, paying: false})
    | StartReturning => ReasonReact.Update({...state, returning: true})
    | StopReturning => ReasonReact.Update({...state, returning: false})
    },
  render: self => {
    let items = order.orderItems |> Array.of_list;
    let disablePayButton: Js.boolean =
      items |> Array.length > 0 ? Js.false_ : Js.true_;
    <div className="order-actions">
      (
        switch (
          self.state.paying,
          self.state.returning,
          order.paid,
          order.returned,
          order.id,
        ) {
        | (true, _, _, _, _) =>
          <PinInput
            onFailure=(() => self.send(StopPaying))
            onSuccess=(cashier => payOrder(cashier, order, onFinish))
          />
        | (_, true, _, _, Some(_id)) =>
          <PinInput
            onFailure=(() => self.send(StopReturning))
            onSuccess=(cashier => returnOrder(cashier, order, onFinish))
          />
        | (false, false, None, None, Some(_id)) =>
          <span>
            <div
              className="save-button-card card"
              onClick=((_) => saveOrder(order, onFinish))>
              (ReactUtils.s("Guardar"))
            </div>
            <div
              className="pay-button-card card"
              disabled=disablePayButton
              onClick=((_) => self.send(StartPaying))>
              (ReactUtils.s("Pagar"))
            </div>
            <div
              className="remove-button-card card"
              onClick=((_) => removeOrder(order, onFinish))>
              (ReactUtils.s("Borrar"))
            </div>
          </span>
        | (false, false, None, None, None) =>
          <span>
            <div
              className="save-button-card card"
              onClick=((_) => saveOrder(order, onFinish))>
              (ReactUtils.s("Guardar"))
            </div>
            <div
              className="pay-button-card card"
              disabled=disablePayButton
              onClick=((_) => self.send(StartPaying))>
              (ReactUtils.s("Pagar"))
            </div>
          </span>
        | (false, false, Some(_paid), None, Some(_id)) =>
          <div
            className="card quiet-card"
            onClick=((_) => self.send(StartReturning))>
            (ReactUtils.s("Devolver"))
          </div>
        | (_, _, _, _, _) => ReasonReact.nullElement
        }
      )
      <div className="card" onClick=((_) => onFinish(order))>
        (ReactUtils.s("Salir"))
      </div>
    </div>;
  },
};