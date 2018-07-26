let str = ReasonReact.stringToElement;

let component = ReasonReact.statelessComponent("EditProductModal");

let make =
    (
      ~onCancel=() => (),
      ~onCreate=() => (),
      ~onSubmit,
      ~products: list(Product.t),
      ~product: option(Product.t)=None,
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
          <div>
            <ProductEdit
              product
              onCancel=(_ => onCancel())
              products
              onSubmit
            />
          </div>
        </BsReactstrap.ModalBody>
        <BsReactstrap.ModalFooter className="modal-footer" />
      </BsReactstrap.Modal>
    </div>,
};