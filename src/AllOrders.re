open OrderData;

open Util;

type state = {
  startDate: float,
  endDate: float,
  orders: list(Order.orderVm),
};

type actions =
  | LoadOrders(list(Order.orderVm))
  | ViewOrder(string);

let component = ReasonReact.reducerComponent("AllOrders");

let make = (~goBack, _children) => {
  ...component,
  initialState: () => {
    startDate: Date.oneMonthBefore(Date.now()),
    endDate: Date.now(),
    orders: [],
  },
  didMount: self => {
    CafeStore.getClosedOrders(self.state.startDate, self.state.endDate)
    |> Js.Promise.then_(orders => {
         let vms = orders |> List.map(OrderConversion.vmFromExistingOrder);
         self.send(LoadOrders(vms));
         Js.Promise.resolve();
       })
    |> ignore;
    ReasonReact.NoUpdate;
  },
  reducer: (action, state) =>
    switch (action) {
    | LoadOrders(orders) => ReasonReact.Update({...state, orders})
    | ViewOrder(id) =>
      ReasonReact.SideEffects(
        (_self => ReasonReact.Router.push("order?orderId=" ++ id)),
      )
    },
  render: self =>
    <div className="all-orders">
      <div className="header">
        <div className="header-menu">
          <div className="card wide-card quiet-card" onClick=goBack>
            (s("Regresar"))
          </div>
        </div>
        <div className="header-options"> (s("date filters here")) </div>
      </div>
      <OrderList
        orders=self.state.orders
        onSelect=(
          order =>
            self.send(
              ViewOrder(
                switch (order.id) {
                | Some(id) => id
                | None => ""
                },
              ),
            )
        )
      />
    </div>,
};