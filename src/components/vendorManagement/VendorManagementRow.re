open ReactUtils;

type state = {
  modifying: bool,
  showModal:bool,
  modifiedVendor: Vendor.t,
  originalVendor: Vendor.t,
  name: string,
};

type action =
  | EnableMod
  | CancelMod
  | ShowDialog
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
    showModal:false,
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
    | ShowDialog => 
      ReasonReact.Update({...state, showModal:!state.showModal })  
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
    <div>
        <BsReactstrap.Modal 
        isOpen=(self.state.showModal)
        toggle = (self.state.showModal)
        className="modal"
        >
            <BsReactstrap.ModalHeader toggle=(self.state.showModal)>
              "Delete Vendor"
            </BsReactstrap.ModalHeader>
            <BsReactstrap.ModalBody className="modal-content">"Are you sure you want to delete this vendor?"</BsReactstrap.ModalBody>
            <BsReactstrap.ModalFooter>
                <Button 
                  local=true
                  className="remove-button-card"
                  label="action.delete"
                  onClick=((_)=>remove(self.state.originalVendor))/>
                  <div className="spaceDivider"/>
                <Button 
                  local=true
                  className="cancel-button-card"
                  label="action.cancelModal"
                  onClick=((_)=>self.send(ShowDialog))/>
            </BsReactstrap.ModalFooter>
        </BsReactstrap.Modal>  
        (switch (self.state.modifying) {
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
                  className="remove-button-card"
                  onClick=((_) => self.send(ShowDialog)/*remove(self.state.originalVendor)*/)
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
          }
          )
    </div>;
  },
};