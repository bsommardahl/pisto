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

let removeFromStore = (order: OrderData.Order.orderVm) =>
  switch (order.id) {
  | None => ()
  | Some(_id) =>
    /* RealCafeStore.remove(id)
       |> Js.Promise.then_(() => Js.Console.log("Removed order."))
       |> ignore; */
    ()
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
              {...state.order, paidOn: Some(Js.Date.now())},
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
            removeFromStore(order);
            state.onFinish(order);
          }
        ),
      )
    },
  render: self =>
    <div className="order-actions">
      <button onClick=((_) => self.send(SaveOrder))>
        (s("Guardar"))
      </button>
      <button onClick=((_) => self.send(PayOrder))> (s("Pagar")) </button>
    </div>,
  /* <button onClick=((_) => self.send(RemoveOrder))>
       (s("Borrar"))
     </button> */
};