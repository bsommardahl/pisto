open ReactUtils;

open Js.Promise;

type state = {expenseTypes: list(ExpenseType.t)};

type action =
  | LoadExpenseTypes(list(ExpenseType.t))
  | ExpenseTypeRemoved(ExpenseType.t)
  | ExpenseTypeModified(ExpenseType.t)
  | NewExpenseTypeCreated(ExpenseType.t);

let component = ReasonReact.reducerComponent("ExpenseTypeManagement");

let make = _children => {
  ...component,
  didMount: self => {
    ExpenseTypeStore.getAll()
    |> Js.Promise.then_(expenseTypes => {
         self.send(LoadExpenseTypes(expenseTypes));
         Js.Promise.resolve();
       })
    |> ignore;
    ReasonReact.NoUpdate;
  },
  initialState: () => {expenseTypes: []},
  reducer: (action, state) =>
    switch (action) {
    | LoadExpenseTypes(expenseTypes) =>
      ReasonReact.Update({expenseTypes: expenseTypes})
    | ExpenseTypeRemoved(exp) =>
      ReasonReact.Update({
        expenseTypes:
          state.expenseTypes
          |> List.filter((d: ExpenseType.t) => d.id !== exp.id),
      })
    | ExpenseTypeModified(exp) =>
      ReasonReact.Update({
        expenseTypes:
          state.expenseTypes
          |> List.map((d: ExpenseType.t) => d.id !== exp.id ? exp : d),
      })
    | NewExpenseTypeCreated(exp) =>
      ReasonReact.Update({
        expenseTypes: List.concat([state.expenseTypes, [exp]]),
      })
    },
  render: self => {
    let goBack = (_) => ReasonReact.Router.push("/admin");
    let removeExpenseType = (p: ExpenseType.t) => {
      ExpenseTypeStore.remove(p.id)
      |> then_((_) => {
           self.send(ExpenseTypeRemoved(p));
           resolve();
         })
      |> ignore;
      ();
    };
    let modifyExpenseType = (modifiedExpenseType: ExpenseType.t) =>
      ExpenseTypeStore.update(modifiedExpenseType)
      |> then_((_) => {
           self.send(ExpenseTypeModified(modifiedExpenseType));
           resolve();
         })
      |> ignore;
    let createExpenseType = (newExpenseType: ExpenseType.New.t) => {
      ExpenseTypeStore.add(newExpenseType)
      |> Js.Promise.then_((newExpenseType: ExpenseType.t) => {
           self.send(NewExpenseTypeCreated(newExpenseType));
           Js.Promise.resolve();
         })
      |> ignore;
      ();
    };
    <div className="admin-menu">
      <div className="header">
        <div className="header-menu">
          <Button
            local=true
            className="quiet-card"
            onClick=goBack
            label="nav.back"
          />
        </div>
        <div className="header-options">
          (s("admin.expenseTypes.header" |> Lang.translate))
        </div>
      </div>
      <div className="expenseType-management">
        <table className="table">
          <thead>
            <tr>
              <th />
              <th> (s("expenseType.name" |> Lang.translate)) </th>
            </tr>
          </thead>
          <tbody>
            (
              self.state.expenseTypes
              |> List.map((d: ExpenseType.t) =>
                   <ExpenseTypeManagementRow
                     expenseType=d
                     remove=removeExpenseType
                     modify=modifyExpenseType
                     key=d.id
                   />
                 )
              |> Array.of_list
              |> ReasonReact.arrayToElement
            )
          </tbody>
          <tfoot>
            <ExpenseTypeManagementNew create=createExpenseType />
          </tfoot>
        </table>
      </div>
    </div>;
  },
};