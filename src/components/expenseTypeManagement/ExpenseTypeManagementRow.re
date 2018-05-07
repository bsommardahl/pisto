open ReactUtils;

open Lang;

type state = {
  modifying: bool,
  modifiedExpenseType: ExpenseType.t,
  originalExpenseType: ExpenseType.t,
  name: string,
};

type action =
  | EnableMod
  | CancelMod
  | SaveMod(ExpenseType.t)
  | ChangeName(string);

let component = ReasonReact.reducerComponent("ExpenseTypeManagementRow");

let make = (~expenseType, ~remove, ~modify, _children) => {
  ...component,
  initialState: () => {
    modifying: false,
    originalExpenseType: expenseType,
    modifiedExpenseType: expenseType,
    name: expenseType.name,
  },
  reducer: (action, state) =>
    switch (action) {
    | EnableMod => ReasonReact.Update({...state, modifying: true})
    | CancelMod =>
      ReasonReact.Update({
        ...state,
        modifying: false,
        modifiedExpenseType: state.originalExpenseType,
      })
    | SaveMod(expenseType) =>
      ReasonReact.Update({
        ...state,
        modifying: false,
        originalExpenseType: expenseType,
        modifiedExpenseType: expenseType,
      })
    | ChangeName(newVal) =>
      ReasonReact.Update({
        ...state,
        modifiedExpenseType: {
          ...state.modifiedExpenseType,
          name: newVal,
        },
      })
    },
  render: self => {
    let getVal = ev => ReactDOMRe.domElementToObj(
                         ReactEventRe.Form.target(ev),
                       )##value;
    let saveModification = (_) => {
      let modified = self.state.modifiedExpenseType;
      modify(modified);
      self.send(SaveMod(modified));
    };
    switch (self.state.modifying) {
    | false =>
      <tr>
        <td>
          <Button
            local=true
            onClick=((_) => self.send(EnableMod))
            label="action.edit"
          />
        </td>
        <td> (s(self.state.originalExpenseType.name)) </td>
        <td>
          <Button
            local=true
            onClick=(
              (_) => {
                Js.log(
                  "row:: eliminating " ++ self.state.originalExpenseType.id,
                );
                remove(self.state.originalExpenseType);
              }
            )
            label="action.delete"
          />
        </td>
      </tr>
    | true =>
      <tr>
        <td>
          <Button
            local=true
            onClick=((_) => self.send(CancelMod))
            label="action.cancel"
          />
        </td>
        <td>
          <input
            value=self.state.modifiedExpenseType.name
            onChange=(ev => self.send(ChangeName(getVal(ev))))
          />
        </td>
        <td>
          <Button local=true onClick=saveModification label="action.save" />
        </td>
      </tr>
    };
  },
};