type state = {
  value: string,
  numbers: int,
};

type action =
  | Reset
  | StringToInt
  | Change(string)
  | Add
  | Subtract
  | ChangeToString;

let component = ReasonReact.reducerComponent("QuantitySelector");

let intOrDefault = (opt: option(string)) =>
  switch (opt) {
  | None => "1"
  | Some(s) => s
  };

type t = int;

let cleanNonNumeric = n => n |> Js.String.replaceByRe([%bs.re "/\\D/g"], "");

let fromString = (str: string) : t =>
  switch (str |> cleanNonNumeric) {
  | "" => 0
  | s => s |> int_of_string
  };

let make = (~value="1", ~numbers=1, _children) => {
  ...component,
  initialState: () => {value, numbers},
  reducer: (action, state) =>
    switch (action) {
    | Change(text) => ReasonReact.Update({...state, value: text})
    | StringToInt =>
      ReasonReact.Update({...state, numbers: fromString(state.value)})
    | Add =>
      ReasonReact.UpdateWithSideEffects(
        {...state, numbers: state.numbers + 1},
        (self => self.send(ChangeToString)),
      )
    | Subtract =>
      ReasonReact.UpdateWithSideEffects(
        {
          ...state,
          numbers: state.numbers > 1 ? state.numbers - 1 : state.numbers,
        },
        (self => self.send(ChangeToString)),
      )
    | ChangeToString =>
      ReasonReact.Update({...state, value: Js.Int.toString(state.numbers)})
    | Reset => ReasonReact.Update({...state, value: "1"})
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
        value=self.state.value
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