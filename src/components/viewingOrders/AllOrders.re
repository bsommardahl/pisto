type state = {
  startDate: float,
  endDate: float,
  orders: list(Order.orderVm),
  interval: int,
};

type actions =
  | LoadOrders(list(Order.orderVm))
  | ViewOrder(string);

let component = ReasonReact.reducerComponent("AllOrders");

let loadClosedOrders = (state, send) => {
  let getPaidDateForComparison = (fl: option(Paid.t)) =>
    switch (fl) {
    | None => 0.0
    | Some(n) => n.on
    };
  OrderStore.getClosedOrders(state.startDate, state.endDate)
  |> Js.Promise.then_(orders => {
       let vms =
         orders
         |> List.fast_sort((a: Order.t, b: Order.t) =>
              compare(
                getPaidDateForComparison(b.paid),
                getPaidDateForComparison(a.paid),
              )
            )
         |> List.map(Order.vmFromExistingOrder);
       send(LoadOrders(vms));
       Js.Promise.resolve();
     })
  |> ignore;
};

let make = (~startDate, ~endDate, _children) => {
  ...component,
  initialState: () => {startDate, endDate, orders: [], interval: (-1)},
  didMount: self => {
    loadClosedOrders(self.state, self.send);
    ();
  },
  reducer: (action, state) =>
    switch (action) {
    | LoadOrders(orders) => ReasonReact.Update({...state, orders})
    | ViewOrder(id) =>
      ReasonReact.SideEffects(
        (_self => ReasonReact.Router.push("order?orderId=" ++ id)),
      )
    },
  subscriptions: self => [
    Sub(
      () =>
        Js.Global.setInterval(
          () => loadClosedOrders(self.state, self.send),
          5000,
        ),
      Js.Global.clearInterval,
    ),
  ],
  render: self =>
    <div className="all-orders">
      <div className="header">
        <div className="header-menu" />
        <div className="header-options">
          (ReactUtils.s("allOrders.header" |> Lang.translate))
        </div>
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