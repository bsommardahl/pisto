open ReactUtils;

type state = {name: string};

type action =
  | ChangeName(string)
  | ClearInputs;

let component = ReasonReact.reducerComponent("ExpenseTypeManagementNew");

let make = (~create, _children) => {
  ...component,
  initialState: () => {name: ""},
  reducer: (action, _state) =>
    switch (action) {
    | ChangeName(newVal) => ReasonReact.Update({name: newVal})
    | ClearInputs => ReasonReact.Update({name: ""})
    },
  render: self => {
    let finishedEnteringData = () => {
      let newExpenseType: ExpenseType.New.t = {name: self.state.name};
      self.send(ClearInputs);
      create(newExpenseType);
    };
    let getVal = ev => ReactDOMRe.domElementToObj(
                         ReactEventRe.Form.target(ev),
                       )##value;
    <tr>
      <td />
      <td>
        <input
          value=self.state.name
          onChange=(ev => self.send(ChangeName(getVal(ev))))
        />
      </td>
      <td>
        <Button
          local=true
          onClick=((_) => finishedEnteringData())
          label="action.create"
        />
      </td>
    </tr>;
  },
};