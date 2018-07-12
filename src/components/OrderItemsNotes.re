type state = {value: string};

type action =
  | AddNote(string);
let component = ReasonReact.reducerComponent("OrderItemsNotes");

let make = (~onCancel=() => (), ~isOpen=false, ~label: string, _children) => {
  ...component,
  initialState: () => {value: ""},
  reducer: (action, _state) =>
    switch (action) {
    | AddNote(value) => ReasonReact.Update({value: value})
    },
  render: self => {
    let str = ReasonReact.stringToElement;
    <div>
      <BsReactstrap.Modal isOpen className="modal">
        <BsReactstrap.ModalHeader className="modal-header">
          (ReactUtils.sloc(label))
        </BsReactstrap.ModalHeader>
        <BsReactstrap.ModalBody className="modal-content">
          <NotesInput onFinish=(value => self.send(AddNote(value))) />
          <div> (str("-" ++ self.state.value)) </div>
        </BsReactstrap.ModalBody>
        <BsReactstrap.ModalFooter className="modal-footer">
          <div className="spaceDivider" />
          <Button
            local=true
            className="cancel-button-card"
            label="action.cancelModal"
            onClick=(_ => onCancel())
          />
        </BsReactstrap.ModalFooter>
      </BsReactstrap.Modal>
    </div>;
  },
};