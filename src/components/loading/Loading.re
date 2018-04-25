type state = {loading: list(string)};

type action =
  | Products(bool)
  | Orders(bool)
  | Expenses(bool);

let component = ReasonReact.reducerComponent("Loading");

let make = _children => {
  ...component,
  initialState: _self => {loading: []},
  didMount: self => {
    ProductStore.connection.remote.active
    |> Most.observe(() => self.send(Products(true)))
    |> ignore;
    ProductStore.connection.remote.paused
    |> Most.observe(() => self.send(Products(false)))
    |> ignore;
    OrderStore.connection.remote.active
    |> Most.observe(() => self.send(Orders(true)))
    |> ignore;
    OrderStore.connection.remote.paused
    |> Most.observe(() => self.send(Orders(false)))
    |> ignore;
    ExpenseStore.connection.remote.active
    |> Most.observe(() => self.send(Expenses(true)))
    |> ignore;
    ExpenseStore.connection.remote.paused
    |> Most.observe(() => self.send(Expenses(false)))
    |> ignore;
    ReasonReact.NoUpdate;
  },
  reducer: (action, state) =>
    switch (action) {
    | Expenses(true) =>
      ReasonReact.Update({
        loading: List.concat([state.loading, ["expenses"]]),
      })
    | Expenses(false) =>
      let newList = state.loading |> List.filter(x => x !== "expenses");
      Js.log(newList);
      ReasonReact.Update({loading: newList});
    | Products(true) =>
      ReasonReact.Update({
        loading: List.concat([state.loading, ["products"]]),
      })
    | Products(false) =>
      let newList = state.loading |> List.filter(x => x !== "products");
      Js.log(newList);
      ReasonReact.Update({loading: newList});
    | Orders(true) =>
      ReasonReact.Update({
        loading: List.concat([state.loading, ["orders"]]),
      })
    | Orders(false) =>
      let newList = state.loading |> List.filter(x => x !== "orders");
      Js.log(newList);
      ReasonReact.Update({loading: newList});
    },
  render: self =>
    if (self.state.loading |> List.length === 0) {
      <div />;
    } else {
      <div className="loading">
        <div className="lds-facebook"> <div /> <div /> <div /> </div>
      </div>;
    },
};