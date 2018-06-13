type state = {value: string};

type action =
  | Reset
  | Change(string);

let component = ReasonReact.reducerComponent("QuantitySelector");

let intOrDefault = (opt: option(string)) =>
  switch (opt) {
  | None => "1"
  | Some(s) => s
  };

let make = (~value="1", ~onPlus, ~onSubtract, _children) => {
  ...component,
  initialState: () => {value: value},
  reducer: (action, _state) =>
    switch (action) {
    | Change(number) => ReasonReact.Update({value: number})
    | Reset => ReasonReact.Update({value: "1"})
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
        onClick=(_ => onSubtract(self.state.value))
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
        onClick=(_ => onPlus(self.state.value))
      />
    </div>;
  },
};