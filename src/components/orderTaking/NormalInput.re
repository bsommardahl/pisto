open ReactUtils;

type state = {value: string};

type action =
  | Reset
  | Change(string);

let component = ReasonReact.reducerComponent("NormalInput");

let stringOrDefault = (opt: option(string)) =>
  switch (opt) {
  | None => ""
  | Some(s) => s
  };

let make = (~value="", ~onFinish, ~className="", _children) => {
  ...component,
  initialState: () => {value: value},
  reducer: (action, _state) =>
    switch (action) {
    | Change(text) => ReasonReact.Update({value: text})
    | Reset => ReasonReact.Update({value: ""})
    },
  render: self =>
    <div>
      <input
        onChange={ev => self.send(Change(getVal(ev)))}
        className={"search-input " ++ className}
        value={self.state.value}
        placeholder="Search a product..."
      />
      <Button
        onClick={_ => onFinish(self.state.value)}
        local=true
        label="modal.SearchProduct"
      />
    </div>,
};