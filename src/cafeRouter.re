type view =
  | Home
  | Order
  | AllOrders
  | Admin
  | Products
  | Sync
  | Discounts
  | Webhooks;

type customerName = string;

type state = {currentView: view};

type action =
  | Show(view);

let component = ReasonReact.reducerComponent("CafeRouter");

let joinStrings = l => l |> Array.of_list |> Js.Array.joinWith(",");

let make = _children => {
  ...component,
  initialState: () => {currentView: Home},
  reducer: (action, _state) =>
    switch (action) {
    | Show(view) => ReasonReact.Update({currentView: view})
    },
  subscriptions: self => [
    Sub(
      () =>
        ReasonReact.Router.watchUrl(url =>
          switch (url.path) {
          | [] => self.send(Show(Home))
          | ["order"] => self.send(Show(Order))
          | ["orders"] => self.send(Show(AllOrders))
          | ["admin"] => self.send(Show(Admin))
          | ["products"] => self.send(Show(Products))
          | ["sync"] => self.send(Show(Sync))
          | ["discounts"] => self.send(Show(Discounts))
          | ["webhooks"] => self.send(Show(Webhooks))
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
        | Webhooks => <WebhookManagement />
        }
      )
    </div>;
  },
};