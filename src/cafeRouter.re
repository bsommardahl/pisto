type view =
  | Home
  | Order
  | AllOrders
  | Admin
  | Products
  | Sync
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
          | ["orders"] => self.send(Show(AllOrders))
          | ["admin"] => self.send(Show(Admin))
          | ["products"] => self.send(Show(Products))
          | ["sync"] => self.send(Show(Sync))
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
        | Logs => <LogManagement />
        | ExpenseTypes => <ExpenseTypeManagement />
        | Vendors => <VendorManagement />
        | Expenses =>
          <div>
            <div className="header">
              <div className="header-menu">
                <div className="card wide-card quiet-card" onClick=goBack>
                  (ReactUtils.s("Atras"))
                </div>
              </div>
              <div className="header-options">
                (ReactUtils.s("Gestion de Gastos"))
              </div>
            </div>
            <ExpenseManagement />
          </div>
        | Daily =>
          <PAndLContainer
            startDate=(Date.now() |> Date.startOfDay)
            endDate=(Date.now() |> Date.endOfDay)
          />
        }
      )
    </div>;
  },
};