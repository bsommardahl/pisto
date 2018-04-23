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
          <button onClick=((_) => self.send(EnableMod))>
            (s("Editar"))
          </button>
        </td>
        <td> (s(self.state.originalVendor.name)) </td>
        <td>
          <button
            onClick=(
              (_) => {
                Js.log("row:: eliminating " ++ self.state.originalVendor.id);
                remove(self.state.originalVendor);
              }
            )>
            (s("Eliminar"))
          </button>
        </td>
      </tr>
    | true =>
      <tr>
        <td>
          <button onClick=((_) => self.send(CancelMod))>
            (s("Cancelar"))
          </button>
        </td>
        <td>
          <input
            value=self.state.modifiedVendor.name
            onChange=(ev => self.send(ChangeName(getVal(ev))))
          />
        </td>
        <td> <button onClick=saveModification> (s("Guardar")) </button> </td>
      </tr>
    };
  },
};