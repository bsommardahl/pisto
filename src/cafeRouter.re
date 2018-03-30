type view =
  | Home
  | Order
  | AllOrders
  | Admin
  | Products;

type customerName = string;

type state = {currentView: view};

type action =
  | ShowHome
  | ShowOrder
  | ShowAllOrders
  | ShowAdmin
  | ShowProducts;

let component = ReasonReact.reducerComponent("CafeRouter");

let joinStrings = l => l |> Array.of_list |> Js.Array.joinWith(",");

let make = _children => {
  ...component,
  initialState: () => {currentView: Home},
  reducer: (action, _state) =>
    switch (action) {
    | ShowHome => ReasonReact.Update({currentView: Home})
    | ShowOrder => ReasonReact.Update({currentView: Order})
    | ShowAllOrders => ReasonReact.Update({currentView: AllOrders})
    | ShowAdmin => ReasonReact.Update({currentView: Admin})
    | ShowProducts => ReasonReact.Update({currentView: Products})
    },
  subscriptions: self => [
    Sub(
      () =>
        ReasonReact.Router.watchUrl(url =>
          switch (url.path) {
          | [] => self.send(ShowHome)
          | ["order"] => self.send(ShowOrder)
          | ["orders"] => self.send(ShowAllOrders)
          | ["admin"] => self.send(ShowAdmin)
          | ["products"] => self.send(ShowProducts)
          | p => Js.log("I don't know this path. " ++ (p |> joinStrings))
          }
        ),
      ReasonReact.Router.unwatchUrl,
    ),
  ],
  render: self => {
    let onStartNewOrder = customerName =>
      ReasonReact.Router.push("order?customerName=" ++ customerName);
    let goToAdmin = () => ReasonReact.Router.push("admin");
    let goBack = (_) => ReasonReact.Router.push("/");
    <div>
      (
        switch (self.state.currentView) {
        | Home => <Home onStartNewOrder goToAdmin />
        | Order => <OrderScreen goBack />
        | AllOrders => <AllOrders goBack=((_) => goToAdmin()) />
        | Admin => <Admin />
        | Products => <ProductManagement />
        }
      )
    </div>;
  },
};