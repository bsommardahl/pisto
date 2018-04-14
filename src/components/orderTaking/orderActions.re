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

let payOrder = (order: Order.orderVm, onFinish: Order.orderVm => unit) => {
  let totals =
    OrderItemCalculation.getTotals(order.discounts, order.orderItems);
  saveOrder(
    {
      ...order,
      paid:
        Some({
          on: Date.now(),
          by: "",
          subTotal: totals.subTotal,
          tax: totals.tax,
          discount: totals.discounts,
          total: totals.total,
        }),
    },
    onFinish,
  );
};

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

let component = ReasonReact.statelessComponent("OrderActions");

let make = (~closed: bool, ~order: Order.orderVm, ~onFinish, _children) => {
  ...component,
  render: _self => {
    let items = order.orderItems |> Array.of_list;
    let disablePayButton: Js.boolean =
      items |> Array.length > 0 ? Js.false_ : Js.true_;
    <div className="order-actions">
      <div
        className="save-button-card card"
        onClick=((_) => saveOrder(order, onFinish))>
        (ReactUtils.s("Guardar"))
      </div>
      (
        if (! closed) {
          <div
            className="pay-button-card card"
            disabled=disablePayButton
            onClick=((_) => payOrder(order, onFinish))>
            (ReactUtils.s("Pagar"))
          </div>;
        } else {
          ReasonReact.nullElement;
        }
      )
      <div
        className="remove-button-card card"
        onClick=((_) => removeOrder(order, onFinish))>
        (ReactUtils.s("Borrar"))
      </div>
    </div>;
  },
};