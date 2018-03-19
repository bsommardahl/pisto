open Util;

let saveToStore = (order: OrderData.Order.order) => {
  Js.Console.log("Persisting order....");
  switch (order.id) {
  | None =>
    let allOrders = CafeStore.retrieveAllOrders();
    allOrders
    |> CafeStore.add({
         ...order,
         id: Some(CafeStore.getNextId(allOrders)),
         createdOn: Js.Date.now(),
       })
    |> CafeStore.persistAllOrders;
    Js.Console.log("Added new order.");
  | Some(_) =>
    CafeStore.retrieveAllOrders()
    |> CafeStore.update({...order, lastUpdated: Some(Js.Date.now())})
    |> CafeStore.persistAllOrders;
    Js.Console.log("Modifying order....");
  };
};

let removeFromStore = (order: OrderData.Order.order) =>
  switch (order.id) {
  | None => ()
  | Some(id) =>
    CafeStore.retrieveAllOrders()
    |> CafeStore.remove(id)
    |> CafeStore.persistAllOrders
  };

type state = {
  order: OrderData.Order.order,
  onFinish: OrderData.Order.order => unit,
};

type action =
  | PayOrder
  | SaveOrder
  | RemoveOrder;

let component = ReasonReact.reducerComponent("OrderItem");

let make = (~order: OrderData.Order.order, ~onFinish, _children) => {
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