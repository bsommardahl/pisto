type state = {value: int};

type action =
  | Change(string)
  | Add
  | Subtract;

let component = ReasonReact.reducerComponent("QuantitySelector");

let make = (~onChange, ~value, _children) => {
  ...component,
  initialState: () => {value: value},
  reducer: (action, state) =>
    switch (action) {
    | Change(text) =>
      ReasonReact.UpdateWithSideEffects(
        {value: text |> Number.fromString},
        (self => onChange(self.state.value)),
      )
    | Add =>
      ReasonReact.UpdateWithSideEffects(
        {value: state.value + 1},
        (self => onChange(self.state.value)),
      )
    | Subtract =>
      ReasonReact.UpdateWithSideEffects(
        {value: state.value > 1 ? state.value - 1 : state.value},
        (self => onChange(self.state.value)),
      )
    },
  render: self => {
    let getVal = ev => ReactDOMRe.domElementToObj(
                         ReactEventRe.Form.target(ev),
                       )##value;
    <div className="quantitySelectorDiv">
      <div className="quantityDivider" />
      <Button
        className="smallItems-card"
        iconClass="fas fa-angle-down"
        onClick=(_ => self.send(Subtract))
      />
      <input
        className="quantityInput"
        _type="numbers"
        value=(self.state.value |> string_of_int)
        onChange=(ev => self.send(Change(getVal(ev))))
      />
      <div className="quantityDivider" />
      <Button
        className="smallItems-card pay-button-card"
        iconClass="fas fa-angle-up"
        onClick=(_ => self.send(Add))
      />
    </div>;
  },
};