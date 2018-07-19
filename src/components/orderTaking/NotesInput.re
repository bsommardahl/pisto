type state = {value: string};

type action =
  | Reset
  | Change(string);

let component = ReasonReact.reducerComponent("NotesInput");

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
  render: self => {
    let getVal = ev => ReactDOMRe.domElementToObj(
                         ReactEventRe.Form.target(ev),
                       )##value;
    <div>
      <input
        onChange=(ev => self.send(Change(getVal(ev))))
        className=("search-input " ++ className)
        value=self.state.value
        placeholder="Notes..."
      />
      <Button
        onClick=(_ => onFinish(self.state.value))
        local=true
        label="action.addNoteModal"
      />
    </div>;
  },
};