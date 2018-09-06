let component = ReasonReact.statelessComponent("DeleteModal");

let make =
    (
      ~onConfirm,
      ~onCancel=() => (),
      ~isOpen=false,
      ~label: string,
      ~contentLabel: string,
      _children,
    ) => {
  ...component,
  render: _self =>
    <BsReactstrap.Modal isOpen className="modal">
      <BsReactstrap.ModalHeader className="modal-header">
        {ReactUtils.sloc(label)}
        <Button
          className="exit-modal-button-card exit-card"
          onClick={_ => onCancel()}
          iconClass="fas fa-times"
          local=true
        />
      </BsReactstrap.ModalHeader>
      <BsReactstrap.ModalBody className="modal-body">
        {ReactUtils.sloc(contentLabel)}
      </BsReactstrap.ModalBody>
      <BsReactstrap.ModalFooter className="modal-footer">
        <Button
          local=true
          className="remove-button-card"
          label="action.delete"
          onClick={_ => onConfirm()}
        />
        <div className="spaceDivider" />
        <Button
          local=true
          className="cancel-button-card"
          label="action.cancelModal"
          onClick={_ => onCancel()}
        />
      </BsReactstrap.ModalFooter>
    </BsReactstrap.Modal>,
};
