let str = ReasonReact.stringToElement;

let component = ReasonReact.statelessComponent("OrderItemsNotes");

let make =
    (
      ~onCancel=() => (),
      ~onAccept=() => (),
      ~notes: list(OrderItemNote.t),
      ~addNote,
      ~removeNote=_n => (),
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
          <Button
            className="exit-modal-button-card exit-card"
            onClick=(_ => onCancel())
            iconClass="fas fa-times"
            local=true
          />
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
                           <Button
                             onClick=(_ => removeNote(note))
                             iconClass="fas fa-times"
                             className="small-card remove-button-card"
                           />
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
          <Button
            local=true
            className="pay-button-card"
            label="action.acceptModal"
            onClick=(_ => onAccept())
          />
        </BsReactstrap.ModalFooter>
      </BsReactstrap.Modal>
    </div>,
};