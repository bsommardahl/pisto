[%bs.raw {|require('./app.css')|}];

/* Initialize the order store. */
CafeStore.retrieveAllOrders() |> CafeStore.persistAllOrders;

let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  render: _self => <div className="App"> <AppHeader /> <CafeRouter /> </div>,
};