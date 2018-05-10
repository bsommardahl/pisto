type view =
  | Home
  | Order
  | Pay
  | AllOrders
  | Admin
  | Products
  | Config
  | Daily
  | Expenses
  | ExpenseTypes
  | Vendors
  | Logs;

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
          | ["pay"] => self.send(Show(Pay))
          | ["orders"] => self.send(Show(AllOrders))
          | ["admin"] => self.send(Show(Admin))
          | ["products"] => self.send(Show(Products))
          | ["config"] => self.send(Show(Config))
          | ["daily"] => self.send(Show(Daily))
          | ["expenses"] => self.send(Show(Expenses))
          | ["logs"] => self.send(Show(Logs))
          | ["expenseTypes"] => self.send(Show(ExpenseTypes))
          | ["vendors"] => self.send(Show(Vendors))
          | p => Js.log("I don't know this path. " ++ (p |> joinStrings))
          }
        ),
      ReasonReact.Router.unwatchUrl,
    ),
  ],
  render: self => {
    let onStartNewOrder = customerName =>
      ReasonReact.Router.push("order?customerName=" ++ customerName);
    let goHome = () => ReasonReact.Router.push("/");
    let goToOrder = order =>
      ReasonReact.Router.push(
        "/order?orderId=" ++ (order |> Order.fromVm).id,
      );
    let queryString = ReasonReact.Router.dangerouslyGetInitialUrl().search;
    let orderId =
      switch (Util.QueryParam.get("orderId", queryString)) {
      | None => ""
      | Some(orderId) => orderId
      };
    <div>
      (
        switch (self.state.currentView) {
        | Home => <Home onStartNewOrder />
        | Order => <OrderScreen goBack=goHome />
        | Pay =>
          <PayScreen
            orderId
            onPay=((_c, _o) => goHome())
            onCancel=(o => goToOrder(o))
          />
        | AllOrders => <AllOrders />
        | Admin => <Admin />
        | Products => <ProductManagement />
        | Config => <ConfigManagement />
        | Logs => <LogManagement />
        | ExpenseTypes => <ExpenseTypeManagement />
        | Vendors => <VendorManagement />
        | Expenses => <ExpenseManagement />
        | Daily =>
          <PAndLContainer
            startDate=(ConfigurableDate.now() |> Date.startOfDay)
            endDate=(ConfigurableDate.now() |> Date.endOfDay)
          />
        }
      )
    </div>;
  },
};