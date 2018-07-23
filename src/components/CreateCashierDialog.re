let str = ReasonReact.stringToElement;

let component = ReasonReact.statelessComponent("CreateCashierModal");

let make =
    (
      ~onCancel=() => (),
      ~onSubmit,
      ~isOpen=false,
      ~isUnique,
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
          <div>
            <CashierEdit isUnique onCancel=(_ => onCancel()) onSubmit />
          </div>
        </BsReactstrap.ModalBody>
        <BsReactstrap.ModalFooter className="modal-footer" />
      </BsReactstrap.Modal>
    </div>,
};