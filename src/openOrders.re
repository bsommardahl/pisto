open OrderData;

type state = {orders: list(Order.orderVm)};

type action =
  | SelectOrder(Order.orderVm);

let getOrders = () => [];

/* CafeStore.getOpenOrders(CafeStore.retrieveAllOrders()); */
let component = ReasonReact.reducerComponent("OpenOrders");

let make = _children => {
  ...component,
  initialState: () => {orders: getOrders()},
  reducer: (action, _state) =>
    switch (action) {
    | SelectOrder(order) =>
      ReasonReact.SideEffects(
        (
          _self =>
            switch (order.id) {
            | Some(id) => ReasonReact.Router.push("order?orderId=" ++ id)
            | None => ()
            }
        ),
      )
    },
  render: self => {
    let selectOpenOrder = (order: Order.orderVm) =>
      self.send(SelectOrder(order));
    <div className="active-orders">
      (
        self.state.orders
        |> List.map(o =>
             <OpenOrderCard order=o onSelect=(_event => selectOpenOrder(o)) />
           )
        |> Array.of_list
        |> ReasonReact.arrayToElement
      )
    </div>;
  },
};