type state = {
  name: string,
  pin: string,
};

type action =
  | ChangeName(string)
  | ChangePin(string)
  | ClearInputs;

let component = ReasonReact.reducerComponent("CashierManagementNew");

let make = (~create, _children) => {
  ...component,
  initialState: () => {name: "", pin: ""},
  reducer: (action, state) =>
    switch (action) {
    | ChangeName(newVal) => ReasonReact.Update({...state, name: newVal})
    | ChangePin(newVal) => ReasonReact.Update({...state, pin: newVal})
    | ClearInputs => ReasonReact.Update({name: "", pin: ""})
    },
  render: self => {
    let finishedEnteringData = () => {
      let newCashier: Cashier.New.t = {
        name: self.state.name,
        pin: self.state.pin,
      };
      self.send(ClearInputs);
      create(newCashier);
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
        <input
          value=self.state.pin
          onChange=(ev => self.send(ChangePin(getVal(ev))))
        />
      </td>
      <td>
        <button onClick=((_) => finishedEnteringData())>
          (ReactUtils.s("Crear"))
        </button>
      </td>
    </tr>;
  },
};