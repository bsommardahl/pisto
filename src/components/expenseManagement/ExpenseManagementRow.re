open ReactUtils;

type state = {
  modifying: bool,
  modifiedExpense: Expense.t,
  originalExpense: Expense.t,
};

type action =
  | EnableMod
  | CancelMod
  | SaveMod(Expense.t)
  | ChangeDescription(string)
  | ChangeVendor(string)
  | ChangeExpenseType(string)
  | ChangeDate(float)
  | ChangeSubTotals(list(Expense.SubTotal.t))
  | ChangeTax(int)
  | ChangeTotal(int);

let component = ReasonReact.reducerComponent("ExpenseManagementRow");

let make =
    (~expense, ~allVendors, ~allExpenseTypes, ~remove, ~modify, _children) => {
  ...component,
  initialState: () => {
    modifying: false,
    originalExpense: expense,
    modifiedExpense: expense,
  },
  reducer: (action, state) => {
    let getVendor = id =>
      allVendors |> List.find((v: Vendor.t) => v.id === id);
    let getExpenseType = id =>
      allExpenseTypes |> List.find((v: ExpenseType.t) => v.id === id);
    switch (action) {
    | EnableMod => ReasonReact.Update({...state, modifying: true})
    | CancelMod =>
      ReasonReact.Update({
        ...state,
        modifying: false,
        modifiedExpense: state.originalExpense,
      })
    | SaveMod(expense) =>
      ReasonReact.Update({
        modifying: false,
        originalExpense: expense,
        modifiedExpense: expense,
      })
    | ChangeDescription(newVal) =>
      ReasonReact.Update({
        ...state,
        modifiedExpense: {
          ...state.modifiedExpense,
          description: newVal,
        },
      })
    | ChangeVendor(newVal) =>
      ReasonReact.Update({
        ...state,
        modifiedExpense: {
          ...state.modifiedExpense,
          vendor: getVendor(newVal),
        },
      })
    | ChangeExpenseType(newVal) =>
      ReasonReact.Update({
        ...state,
        modifiedExpense: {
          ...state.modifiedExpense,
          expenseType: getExpenseType(newVal),
        },
      })
    };
  },
  render: self => {
    let getVal = ev => ReactDOMRe.domElementToObj(
                         ReactEventRe.Form.target(ev),
                       )##value;
    let saveModification = (_) => {
      let modified = self.state.modifiedExpense;
      modify(modified);
      self.send(SaveMod(modified));
    };
    let getSubTotalSum = (subTotals: list(Expense.SubTotal.t)) =>
      Belt.List.reduce(subTotals, 0, (acc, sub: Expense.SubTotal.t) =>
        acc + sub.amount
      );
    switch (self.state.modifying) {
    | false =>
      <tr>
        <td>
          <button onClick=((_) => self.send(EnableMod))>
            (s("Editar"))
          </button>
        </td>
        <td> (s(self.state.originalExpense.vendor.name)) </td>
        <td> (s(self.state.originalExpense.expenseType.name)) </td>
        <td> (s(self.state.originalExpense.description)) </td>
        <td> (s(self.state.originalExpense.date |> Date.toDisplay)) </td>
        <td>
          (
            s(
              self.state.originalExpense.subTotals
              |> getSubTotalSum
              |> Money.toDisplay,
            )
          )
        </td>
        <td> (s(self.state.originalExpense.tax |> Money.toDisplay)) </td>
        <td> (s(self.state.originalExpense.total |> Money.toDisplay)) </td>
        <td>
          <button
            onClick=(
              (_) => {
                Js.log("row:: eliminating " ++ self.state.originalExpense.id);
                remove(self.state.originalExpense);
              }
            )>
            (s("Eliminar"))
          </button>
        </td>
      </tr>
    | true =>
      <tr>
        <td>
          <button onClick=((_) => self.send(CancelMod))>
            (s("Cancelar"))
          </button>
        </td>
        <td>
          <select
            value=self.state.modifiedExpense.vendor.id
            onChange=(ev => self.send(ChangeVendor(getVal(ev))))>
            (
              allVendors
              |> List.map((v: Vendor.t) =>
                   <option value=v.id> (s(v.name)) </option>
                 )
              |> Array.of_list
              |> ReasonReact.arrayToElement
            )
          </select>
        </td>
        <td>
          <select
            value=self.state.modifiedExpense.expenseType.id
            onChange=(ev => self.send(ChangeExpenseType(getVal(ev))))>
            (
              allExpenseTypes
              |> List.map((v: ExpenseType.t) =>
                   <option value=v.id> (s(v.name)) </option>
                 )
              |> Array.of_list
              |> ReasonReact.arrayToElement
            )
          </select>
        </td>
        <td>
          <input
            value=self.state.modifiedExpense.description
            onChange=(ev => self.send(ChangeDescription(getVal(ev))))
          />
        </td>
        <td>
          <input
            value=(self.state.modifiedExpense.date |> string_of_float)
            onChange=(ev => self.send(ChangeDate(getVal(ev))))
          />
        </td>
        <td> <button onClick=saveModification> (s("Guardar")) </button> </td>
      </tr>
    };
  },
};