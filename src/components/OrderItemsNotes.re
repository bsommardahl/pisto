let str = ReasonReact.stringToElement;

let component = ReasonReact.statelessComponent("OrderItemsNotes");

let make =
    (
      ~onCancel=() => (),
      ~value: string,
      ~notes: list(OrderItemNote.t),
      ~addNote,
      ~removeNote=_n => (),
      ~canRemoveNote: bool,
      ~isOpen=false,
      ~label: string,
      _children,
    ) => {
  ...component,
  render: _self =>
    <div>
      <BsReactstrap.Modal isOpen className="modal">
        <BsReactstrap.ModalHeader className="modal-header">
          (ReactUtils.sloc(label))
        </BsReactstrap.ModalHeader>
        <BsReactstrap.ModalBody className="modal-content">
          <NotesInput onFinish=(value => addNote(value)) />
          <div>
            <table>
              <tbody>
                (
                  notes
                  |> List.map((note: OrderItemNote.t) =>
                       <tr>
                         <td>
                           (
                             canRemoveNote ?
                               <Button
                                 onClick=(_ => removeNote(note))
                                 label="action.delete"
                                 className="small-card remove-button-card"
                                 local=true
                               /> :
                               ReasonReact.nullElement
                           )
                         </td>
                         <td> <div key=note.id> (str(note.value)) </div> </td>
                       </tr>
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