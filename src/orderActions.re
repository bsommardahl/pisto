open Util;

open CafeStore;

open Pouchdb;

let dbUrl = "http://localhost:5984/orders";

let vmToUpdateOrder =
    (vm: OrderData.Order.orderVm)
    : OrderData.Order.updateOrder => {
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

module RealCafeStore =
  CafeStore.Make(
    {
      let connect = connect;
    },
  );

let saveToStore = (order: OrderData.Order.orderVm) => {
  Js.Console.log("Persisting order....");
  switch (order.id) {
  | None =>
    let db = connect(dbUrl);
    RealCafeStore.add({
      customerName: order.customerName,
      orderItems: order.orderItems,
    })
    |> Js.Promise.then_(_order => {
         Js.Console.log("Added new order.");
         db |> Pouchdb.closeConnection;
       });
  | Some(_id) =>
    let db = connect(dbUrl);
    RealCafeStore.update(vmToUpdateOrder(order), db)
    |> Js.Promise.then_(_order => {
         Js.Console.log("Updated existing order.");
         db |> Pouchdb.closeConnection;
       });
  };
};

let removeFromStore = (order: OrderData.Order.orderVm) =>
  switch (order.id) {
  | None => ()
  | Some(id) =>
    let db = connect(dbUrl);
    db
    |> RealCafeStore.remove(id)
    |> Js.Promise.then_(() => {
         Js.Console.log("Removed order.");
         db |> Pouchdb.closeConnection |> ignore;
         Js.Promise.resolve();
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
            saveToStore({...order, paidOn: Some(Js.Date.now())});
            state.onFinish(order);
          }
        ),
      )
    | SaveOrder =>
      ReasonReact.SideEffects(
        (
          _self => {
            saveToStore(order);
            state.onFinish(order);
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
      <button onClick=((_) => self.send(RemoveOrder))>
        (s("Borrar"))
      </button>
    </div>,
};