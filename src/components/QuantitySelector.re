type state = {value: int};

type action =
  | Change(string)
  | Add
  | Subtract;

let component = ReasonReact.reducerComponent("QuantitySelector");

type t = int;

let cleanNonNumeric = n => n |> Js.String.replaceByRe([%bs.re "/\\D/g"], "");

let fromString = (str: string) : t =>
  switch (str |> cleanNonNumeric) {
  | "" => 0
  | s => s |> int_of_string
  };

let make = (~onChange, ~value, _children) => {
  ...component,
  initialState: () => {value: value},
  reducer: (action, state) =>
    switch (action) {
    | Change(text) =>
      ReasonReact.UpdateWithSideEffects(
        {value: text |> int_of_string},
        (self => onChange(self.state.value)),
      )
    | Add =>
      Js.log(value);
      ReasonReact.UpdateWithSideEffects(
        {value: state.value + 1},
        (self => onChange(self.state.value)),
      );
    | Subtract =>
      Js.log(value);
      ReasonReact.UpdateWithSideEffects(
        {value: state.value > 1 ? state.value - 1 : state.value},
        (self => onChange(self.state.value)),
      );
    },
  render: self => {
    let getVal = ev => ReactDOMRe.domElementToObj(
                         ReactEventRe.Form.target(ev),
                       )##value;
    <div className="quantitySelectorDiv">
      <Button
        className="small-card"
        local=true
        label="action.minus"
        onClick=(_ => self.send(Subtract))
      />
      <input
        className="quantityInput"
        _type="numbers"
        value=(self.state.value |> string_of_int)
        onChange=(ev => self.send(Change(getVal(ev))))
      />
      <Button
        className="small-card pay-button-card"
        local=true
        label="action.plus"
        onClick=(_ => self.send(Add))
      />
    </div>;
  },
};