

type state = {
  modifying: bool,
  modifiedDiscount: Discount.t,
  originalDiscount: Discount.t,
  name: string,
};

type action =
  | EnableMod
  | CancelMod
  | SaveMod(Discount.t)
  | ChangeName(string)
  | ChangePercent(string);

let component = ReasonReact.reducerComponent("DiscountManagementRow");

let make = (~discount, ~remove, ~modify, _children) => {
  ...component,
  initialState: () => {
    modifying: false,
    originalDiscount: discount,
    modifiedDiscount: discount,
    name: discount.name,
  },
  reducer: (action, state) =>
    switch (action) {
    | EnableMod => ReasonReact.Update({...state, modifying: true})
    | CancelMod =>
      ReasonReact.Update({
        ...state,
        modifying: false,
        modifiedDiscount: state.originalDiscount,
      })
    | SaveMod(discount) =>
      ReasonReact.Update({
        ...state,
        modifying: false,
        originalDiscount: discount,
        modifiedDiscount: discount,
      })
    | ChangeName(newVal) =>
      ReasonReact.Update({
        ...state,
        modifiedDiscount: {
          ...state.modifiedDiscount,
          name: newVal,
        },
      })
    | ChangePercent(newVal) =>
      ReasonReact.Update({
        ...state,
        modifiedDiscount: {
          ...state.modifiedDiscount,
          percent: newVal |> int_of_string,
        },
      })
    },
  render: self => {
    let getVal = ev => ReactDOMRe.domElementToObj(
                         ReactEventRe.Form.target(ev),
                       )##value;
    let saveModification = (_) => {
      let modified = self.state.modifiedDiscount;
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
        <td> (ReactUtils.s(self.state.originalDiscount.name)) </td>
        <td> (ReactUtils.s(self.state.originalDiscount.percent |> string_of_int)) </td>
        <td>
          <button
            onClick=(
              (_) => {
                Js.log(
                  "row:: eliminating " ++ self.state.originalDiscount.id,
                );
                remove(self.state.originalDiscount);
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
            value=self.state.modifiedDiscount.name
            onChange=(ev => self.send(ChangeName(getVal(ev))))
          />
        </td>
        <td>
          <input
            value=(self.state.modifiedDiscount.percent |> string_of_int)
            onChange=(ev => self.send(ChangePercent(getVal(ev))))
          />
        </td>
        <td> <button onClick=saveModification> (ReactUtils.s("Guardar")) </button> </td>
      </tr>
    };
  },
};