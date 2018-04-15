type state = {
  modifying: bool,
  modifiedCashier: Cashier.t,
  originalCashier: Cashier.t,
  name: string,
};

type action =
  | EnableMod
  | CancelMod
  | SaveMod(Cashier.t)
  | ChangeName(string)
  | ChangePin(string);

let component = ReasonReact.reducerComponent("CashierManagementRow");

let make = (~cashier, ~remove, ~modify, _children) => {
  ...component,
  initialState: () => {
    modifying: false,
    originalCashier: cashier,
    modifiedCashier: cashier,
    name: cashier.name,
  },
  reducer: (action, state) =>
    switch (action) {
    | EnableMod => ReasonReact.Update({...state, modifying: true})
    | CancelMod =>
      ReasonReact.Update({
        ...state,
        modifying: false,
        modifiedCashier: state.originalCashier,
      })
    | SaveMod(cashier) =>
      ReasonReact.Update({
        ...state,
        modifying: false,
        originalCashier: cashier,
        modifiedCashier: cashier,
      })
    | ChangeName(newVal) =>
      ReasonReact.Update({
        ...state,
        modifiedCashier: {
          ...state.modifiedCashier,
          name: newVal,
        },
      })
    | ChangePin(newVal) =>
      ReasonReact.Update({
        ...state,
        modifiedCashier: {
          ...state.modifiedCashier,
          pin: newVal,
        },
      })
    },
  render: self => {
    let getVal = ev => ReactDOMRe.domElementToObj(
                         ReactEventRe.Form.target(ev),
                       )##value;
    let saveModification = (_) => {
      let modified = self.state.modifiedCashier;
      modify(modified);
      self.send(SaveMod(modified));
    };
    switch (self.state.modifying) {
    | false =>
      <tr>
        <td>
          <button onClick=((_) => self.send(EnableMod))>
            (ReactUtils.s("Editar"))
          </button>
        </td>
        <td> (ReactUtils.s(self.state.originalCashier.name)) </td>
        <td> (ReactUtils.s(self.state.originalCashier.pin)) </td>
        <td>
          <button
            onClick=(
              (_) => {
                Js.log("row:: eliminating " ++ self.state.originalCashier.id);
                remove(self.state.originalCashier);
              }
            )>
            (ReactUtils.s("Eliminar"))
          </button>
        </td>
      </tr>
    | true =>
      <tr>
        <td>
          <button onClick=((_) => self.send(CancelMod))>
            (ReactUtils.s("Cancelar"))
          </button>
        </td>
        <td>
          <input
            value=self.state.modifiedCashier.name
            onChange=(ev => self.send(ChangeName(getVal(ev))))
          />
        </td>
        <td>
          <input
            value=self.state.modifiedCashier.pin
            onChange=(ev => self.send(ChangePin(getVal(ev))))
          />
        </td>
        <td>
          <button onClick=saveModification>
            (ReactUtils.s("Guardar"))
          </button>
        </td>
      </tr>
    };
  },
};