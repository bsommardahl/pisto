type state = {
  value: string
};

type action =
  | Reset
  | Change(string);

let component = ReasonReact.reducerComponent("NotesInput");

let getVal = ev => ReactDOMRe.domElementToObj(
  ReactEventRe.Form.target(ev),
)##value;

let make = (~onFinish, ~className="", _children) => {
  ...component,
  initialState: () => {
    value: ""
  },
  reducer: (action, _state) =>
    switch (action) {
    | Change(text) => ReasonReact.Update({value: text})
    | Reset => ReasonReact.Update({value: ""})
    },
  render: self => {
    <div>
      <input
        onChange=(ev => self.send(Change(getVal(ev))))
        className=("search-input " ++ className)
        value=self.state.value
        placeholder="Notes..."
      />
      <Button
        onClick=(_ => {
          onFinish(self.state.value);
          self.send(Reset);
        })
        local=true
        label="action.addNoteModal"
      />
    </div>;
  },
};