type view =
  | Home
  | Order
  | AllOrders
  | Admin
  | Products
  | Sync
  | Discounts;

type customerName = string;

type state = {currentView: view};

type action =
  | ShowHome
  | ShowOrder
  | ShowAllOrders
  | ShowAdmin
  | ShowProducts
  | ShowSync
  | ShowDiscounts;

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
    | ShowSync => ReasonReact.Update({currentView: Sync})
    | ShowDiscounts => ReasonReact.Update({currentView: Discounts})
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
          | ["sync"] => self.send(ShowSync)
          | ["discounts"] => self.send(ShowDiscounts)
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
        | AllOrders => <AllOrders />
        | Admin => <Admin />
        | Products => <ProductManagement />
        | Sync => <SyncManagement />
        | Discounts => <DiscountManagement />
        }
      )
    </div>;
  },
};