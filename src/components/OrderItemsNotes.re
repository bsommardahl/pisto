type note = {
  id: int,
  value: string,
};
let str = ReasonReact.stringToElement;

type state = {
  value: string,
  notes: list(note),
};

type action =
  | AddNote(string)
  | DisplayNotes;

let lastId = ref(0);
let newNote = (value: string) => {
  lastId := lastId^ + 1;
  {id: lastId^, value};
};

let component = ReasonReact.reducerComponent("OrderItemsNotes");

let make = (~onCancel=() => (), ~isOpen=false, ~label: string, _children) => {
  ...component,
  initialState: () => {value: "", notes: [{id: 0, value: ""}]},
  reducer: (action, state) =>
    switch (action) {
    | AddNote(value) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, value},
        (self => self.send(DisplayNotes)),
      )
    | DisplayNotes =>
      ReasonReact.Update({
        ...state,
        notes: [newNote(state.value), ...state.notes],
      })
    },
  render: self =>
    <div>
      <BsReactstrap.Modal isOpen className="modal">
        <BsReactstrap.ModalHeader className="modal-header">
          (ReactUtils.sloc(label))
        </BsReactstrap.ModalHeader>
        <BsReactstrap.ModalBody className="modal-content">
          <NotesInput onFinish=(value => self.send(AddNote(value))) />
          <div>
            <table>
              <tbody>
                (
                  self.state.notes
                  |> List.map((note: note) =>
                       <div key=(string_of_int(note.id))>
                         (str(note.value))
                       </div>
                     )
                  |> Array.of_list
                  |> ReasonReact.arrayToElement
                )
              </tbody>
            </table>
          </div>
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
    </div>,
};