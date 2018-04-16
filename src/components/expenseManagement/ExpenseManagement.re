open ReactUtils;

open Js.Promise;

type view =
  | List
  | New
  | Modifying(Expense.t);

type state = {
  currentView: view,
  expenses: list(Expense.t),
  allVendors: list(Vendor.t),
  allExpenseTypes: list(ExpenseType.t),
};

type action =
  | ChangeView(view)
  | ExpenseTypesLoaded(list(ExpenseType.t))
  | LoadExpenseTypes
  | VendorsLoaded(list(Vendor.t))
  | LoadVendors
  | ExpensesLoaded(list(Expense.t))
  | LoadExpenses
  | ExpenseRemoved(Expense.t)
  | ExpenseModified(Expense.t)
  | NewExpenseCreated(Expense.t);

let component = ReasonReact.reducerComponent("ExpenseManagement");

let make = _children => {
  ...component,
  subscriptions: self => [
    Sub(
      () => Js.Global.setInterval(() => self.send(LoadExpenses), 5000),
      Js.Global.clearInterval,
    ),
  ],
  didMount: self => {
    self.send(LoadExpenses);
    self.send(LoadExpenseTypes);
    self.send(LoadVendors);
    ReasonReact.NoUpdate;
  },
  initialState: () => {
    expenses: [],
    allExpenseTypes: [],
    allVendors: [],
    currentView: List,
  },
  reducer: (action, state) =>
    switch (action) {
    | ExpensesLoaded(expenses) => ReasonReact.Update({...state, expenses})
    | LoadExpenses =>
      ReasonReact.SideEffects(
        (
          self =>
            ExpenseStore.getAll()
            |> Js.Promise.then_(expenses => {
                 self.send(
                   ExpensesLoaded(
                     expenses
                     |> List.filter((e: Expense.t) =>
                          e.date > (Date.now() |> Date.startOfDay)
                        ),
                   ),
                 );
                 Js.Promise.resolve();
               })
            |> ignore
        ),
      )
    | LoadVendors =>
      ReasonReact.SideEffects(
        (
          self =>
            VendorStore.getAll()
            |> then_(vendors => {
                 self.send(VendorsLoaded(vendors));
                 resolve();
               })
            |> ignore
        ),
      )
    | VendorsLoaded(vendors) =>
      ReasonReact.Update({...state, allVendors: vendors})
    | LoadExpenseTypes =>
      ReasonReact.SideEffects(
        (
          self =>
            ExpenseTypeStore.getAll()
            |> then_(expenseTypes => {
                 self.send(ExpenseTypesLoaded(expenseTypes));
                 resolve();
               })
            |> ignore
        ),
      )
    | ExpenseTypesLoaded(expenseTypes) =>
      ReasonReact.Update({...state, allExpenseTypes: expenseTypes})
    | ExpenseRemoved(exp) =>
      ReasonReact.Update({
        ...state,
        currentView: List,
        expenses:
          state.expenses |> List.filter((d: Expense.t) => d.id !== exp.id),
      })
    | ExpenseModified(exp) =>
      ReasonReact.Update({
        ...state,
        currentView: List,
        expenses:
          state.expenses
          |> List.map((d: Expense.t) => d.id === exp.id ? exp : d),
      })
    | NewExpenseCreated(exp) =>
      ReasonReact.Update({
        ...state,
        currentView: List,
        expenses: List.concat([state.expenses, [exp]]),
      })
    | ChangeView(view) => ReasonReact.Update({...state, currentView: view})
    },
  render: self => {
    let removeExpense = (p: Expense.t) => {
      ExpenseStore.remove(p.id)
      |> then_((_) => {
           self.send(ExpenseRemoved(p));
           resolve();
         })
      |> ignore;
      ();
    };
    let modifyExpense = (modifiedExpense: Expense.t) =>
      ExpenseStore.update(modifiedExpense)
      |> then_((_) => {
           self.send(ExpenseModified(modifiedExpense));
           resolve();
         })
      |> ignore;
    let createExpense = (newExpense: Expense.New.t) => {
      ExpenseStore.add(newExpense)
      |> Js.Promise.then_((newExpense: Expense.t) => {
           self.send(NewExpenseCreated(newExpense));
           Js.Promise.resolve();
         })
      |> ignore;
      ();
    };
    <div className="expense-management">
      (
        switch (self.state.currentView) {
        | List =>
          <ExpenseManagementList
            expenses=self.state.expenses
            onModify=(d => self.send(ChangeView(Modifying(d))))
            onRemove=(d => removeExpense(d))
            onNew=((_) => self.send(ChangeView(New)))
          />
        | Modifying(expense) =>
          <ExpenseManagementEditor
            expense
            allExpenseTypes=self.state.allExpenseTypes
            allVendors=self.state.allVendors
            onCancel=(() => self.send(ChangeView(List)))
            onCreate=(e => createExpense(e))
            onModify=(e => modifyExpense(e))
            onRemove=(e => removeExpense(e))
          />
        | New =>
          <ExpenseManagementEditor
            allExpenseTypes=self.state.allExpenseTypes
            allVendors=self.state.allVendors
            onCancel=(() => self.send(ChangeView(List)))
            onCreate=(e => createExpense(e))
          />
        }
      )
    </div>;
  },
};