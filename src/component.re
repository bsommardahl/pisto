open OrderData;

type state = {orders: list(Order.order)};

let getOrders = () => {
  /* CafeStore.getOpenOrders(CafeStore.retrieveAllOrders()); */
  []
};

let component = ReasonReact.reducerComponent("OpenOrders");

let make = _children => {
  ...component,
  initialState: () => {orders: getOrders()},
  reducer: Reducer.openOrdersReducer,
  render: self => {
    let selectOpenOrder = (order: Order.order) =>
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