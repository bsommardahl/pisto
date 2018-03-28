open Util;

open CafeStore;

open Pouchdb;

open OrderConversion;

let saveToStore =
    (
      order: OrderData.Order.orderVm,
      onFinish: OrderData.Order.orderVm => unit,
    ) => {
  Js.Console.log("orderActions:: Persisting order....");
  switch (order.id) {
  | None =>
    CafeStore.add({
      customerName: order.customerName,
      orderItems: order.orderItems,
    })
    |> Js.Promise.then_(freshOrder => {
         onFinish(freshOrder |> OrderConversion.vmFromExistingOrder);
         Js.Promise.resolve(
           Js.Console.log("orderActions:: Added new order."),
         );
       })
  | Some(_id) =>
    let o: OrderData.Order.updateOrder = vmToUpdateOrder(order);
    CafeStore.update(o)
    |> Js.Promise.then_(updatedOrder => {
         onFinish(updatedOrder |> OrderConversion.vmFromExistingOrder);
         Js.Promise.resolve(
           Js.Console.log("orderActions:: Updated existing order."),
         );
       });
  };
};

let removeFromStore =
    (
      order: OrderData.Order.orderVm,
      onFinish: OrderData.Order.orderVm => unit,
    ) =>
  switch (order.id) {
  | None => onFinish(order)
  | Some(id) =>
    CafeStore.remove(id)
    |> Js.Promise.then_(() => {
         onFinish(order);
         Js.Promise.resolve(Js.Console.log("Removed order."));
       })
    |> ignore;
    ();
  };

type state = {
  order: OrderData.Order.orderVm,
  onFinish: OrderData.Order.orderVm => unit,
};

type action =
  | PayOrder
  | SaveOrder
  | RemoveOrder;

let component = ReasonReact.reducerComponent("OrderItem");

let make =
    (~closed: bool, ~order: OrderData.Order.orderVm, ~onFinish, _children) => {
  ...component,
  initialState: () => {order, onFinish},
  reducer: (action, state) =>
    switch (action) {
    | PayOrder =>
      ReasonReact.SideEffects(
        (
          _self => {
            saveToStore(
              {...order, paidOn: Some(Js.Date.now())},
              state.onFinish,
            )
            |> ignore;
            ();
          }
        ),
      )
    | SaveOrder =>
      ReasonReact.SideEffects(
        (
          _self => {
            saveToStore(order, state.onFinish) |> ignore;
            ();
          }
        ),
      )
    | RemoveOrder =>
      ReasonReact.SideEffects(
        (
          _self => {
            removeFromStore(order, state.onFinish);
            ();
          }
        ),
      )
    },
  render: self => {
    let items = order.orderItems |> Array.of_list;
    let disablePayButton: Js.boolean =
      items |> Array.length > 0 ? Js.false_ : Js.true_;
    <div className="order-actions">
      <div
        className="save-button-card card"
        onClick=((_) => self.send(SaveOrder))>
        (s("Guardar"))
      </div>
      (
        if (! closed) {
          <div
            className="pay-button-card card"
            disabled=disablePayButton
            onClick=((_) => self.send(PayOrder))>
            (s("Pagar"))
          </div>;
        } else {
          ReasonReact.nullElement;
        }
      )
      <div
        className="remove-button-card card"
        onClick=((_) => self.send(RemoveOrder))>
        (s("Borrar"))
      </div>
    </div>;
  },
};