open ReactUtils;

type state = {
  modifying: bool,
  modifiedVendor: Vendor.t,
  originalVendor: Vendor.t,
  name: string,
};

type action =
  | EnableMod
  | CancelMod
  | SaveMod(Vendor.t)
  | ChangeName(string);

let component = ReasonReact.reducerComponent("VendorManagementRow");

let make = (~vendor, ~remove, ~modify, _children) => {
  ...component,
  initialState: () => {
    modifying: false,
    originalVendor: vendor,
    modifiedVendor: vendor,
    name: vendor.name,
  },
  reducer: (action, state) =>
    switch (action) {
    | EnableMod => ReasonReact.Update({...state, modifying: true})
    | CancelMod =>
      ReasonReact.Update({
        ...state,
        modifying: false,
        modifiedVendor: state.originalVendor,
      })
    | SaveMod(vendor) =>
      ReasonReact.Update({
        ...state,
        modifying: false,
        originalVendor: vendor,
        modifiedVendor: vendor,
      })
    | ChangeName(newVal) =>
      ReasonReact.Update({
        ...state,
        modifiedVendor: {
          ...state.modifiedVendor,
          name: newVal,
        },
      })
    },
  render: self => {
    let getVal = ev => ReactDOMRe.domElementToObj(
                         ReactEventRe.Form.target(ev),
                       )##value;
    let saveModification = (_) => {
      let modified = self.state.modifiedVendor;
      modify(modified);
      self.send(SaveMod(modified));
    };
    switch (self.state.modifying) {
    | false =>
      <tr>
        <td>
          <Button
            local=true
            onClick=((_) => self.send(EnableMod))
            label="action.edit"
          />
        </td>
        <td> (s(self.state.originalVendor.name)) </td>
        <td>
          <Button
            local=true
            onClick=((_) => remove(self.state.originalVendor))
            label="action.delete"
          />
        </td>
      </tr>
    | true =>
      <tr>
        <td>
          <Button
            local=true
            onClick=((_) => self.send(CancelMod))
            label="action.cancel"
          />
        </td>
        <td>
          <input
            value=self.state.modifiedVendor.name
            onChange=(ev => self.send(ChangeName(getVal(ev))))
          />
        </td>
        <td>
          <Button local=true onClick=saveModification label="action.save" />
        </td>
      </tr>
    };
  },
};