open Js.Promise;

type state = {
  expenses: list(Expense.t),
  orders: list(Order.t),
};

type action =
  | OrdersLoaded(list(Order.t))
  | ExpensesLoaded(list(Expense.t))
  | LoadOrders
  | LoadExpenses;

let component = ReasonReact.reducerComponent("PAndLContainer");

let make = (~startDate: Date.t, ~endDate: Date.t, _children) => {
  ...component,
  didMount: self => {
    self.send(LoadOrders);
    self.send(LoadExpenses);
    ();
  },
  reducer: (action, state) =>
    switch (action) {
    | OrdersLoaded(orders) => ReasonReact.Update({...state, orders})
    | ExpensesLoaded(expenses) => ReasonReact.Update({...state, expenses})
    | LoadOrders =>
      ReasonReact.SideEffects(
        (
          self =>
            OrderStore.getClosedOrders(startDate, endDate)
            |> then_(orders => {
                 let notReturned =
                   orders
                   |> List.filter((o: Order.t) =>
                        switch (o.returned) {
                        | None => true
                        | Some(_) => false
                        }
                      );
                 self.send(OrdersLoaded(notReturned));
                 resolve();
               })
            |> ((_) => ())
        ),
      )
    | LoadExpenses =>
      ReasonReact.SideEffects(
        (
          self =>
            ExpenseStore.getAllInRange(startDate, endDate)
            |> then_(expenses => {
                 self.send(ExpensesLoaded(expenses));
                 resolve();
               })
            |> ((_) => ())
        ),
      )
    },
  initialState: _self => {expenses: [], orders: []},
  render: self =>
    <div>
      <PAndLReport
        startDate
        endDate
        expenses=self.state.expenses
        orders=self.state.orders
      />
    </div>,
};