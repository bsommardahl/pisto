let str = ReasonReact.stringToElement;

let component = ReasonReact.statelessComponent("CreateProductModal");

let make =
    (
      ~onCancel=() => (),
      ~onCreate=() => (),
      ~onSubmit,
      ~onClick=() => (),
      ~products: list(Product.t),
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
          <div>
            <ProductEdit
              onCancel=(_ => onCancel())
              onClick=(_ => onClick())
              products
              onSubmit
            />
          </div>
        </BsReactstrap.ModalBody>
        <BsReactstrap.ModalFooter className="modal-footer" />
      </BsReactstrap.Modal>
    </div>,
};