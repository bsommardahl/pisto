open ReactUtils;

type state = {
  name: string,
  percent: string,
};

type action =
  | ChangeName(string)
  | ClearInputs;

let component = ReasonReact.reducerComponent("VendorManagementNew");

let make = (~create, _children) => {
  ...component,
  initialState: () => {name: "", percent: ""},
  reducer: (action, state) =>
    switch (action) {
    | ChangeName(newVal) => ReasonReact.Update({...state, name: newVal})
    | ClearInputs => ReasonReact.Update({name: "", percent: ""})
    },
  render: self => {
    let finishedEnteringData = () => {
      let newVendor: Vendor.New.t = {name: self.state.name};
      self.send(ClearInputs);
      create(newVendor);
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
        <Button
          local=true
          onClick=((_) => finishedEnteringData())
          label="action.create"
        />
      </td>
    </tr>;
  },
};