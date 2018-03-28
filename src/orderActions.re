open Util;

open CafeStore;

open Pouchdb;

open OrderConversion;

let dbUrl = "http://localhost:5984/orders";

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

let make = (~order: OrderData.Order.orderVm, ~onFinish, _children) => {
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
      <button onClick=((_) => self.send(SaveOrder))>
        (s("Guardar"))
      </button>
      <button disabled=disablePayButton onClick=((_) => self.send(PayOrder))>
        (s("Pagar"))
      </button>
      <button onClick=((_) => self.send(RemoveOrder))>
        (s("Borrar"))
      </button>
    </div>;
  },
};