let component = ReasonReact.statelessComponent("SearchModal");

let make = /*
      ~allProducts: list(Product.t),
      ~productFound,*/
    (
      ~onConfirm,
      ~onCancel=() => (),
      ~isOpen=false,
      ~label: string,
      /* ~contentLabel: string,*/
      _children,
    ) => {
  ...component,
  render: _self =>
    /*let findProduct = sku => {
        let matches =
          allProducts |> List.filter((p: Product.t) => p.sku === sku);
        if (matches |> List.length > 0) {
          productFound(matches |. List.nth(0));
        };
      };*/
    <BsReactstrap.Modal isOpen className="modal">
      <BsReactstrap.ModalHeader className="modal-header">
        (ReactUtils.sloc(label))
      </BsReactstrap.ModalHeader>
      <BsReactstrap.ModalBody
        className="modal-content"
        /* <KeyInput className="sku-search" onFinish=(sku => findProduct(sku)) />*/
      />
      <BsReactstrap.ModalFooter className="modal-footer">
        <Button
          local=true
          className="pay-button-card"
          label="action.save"
          onClick=((_) => onConfirm())
        />
        <div className="spaceDivider" />
        <Button
          local=true
          className="cancel-button-card"
          label="action.cancelModal"
          onClick=((_) => onCancel())
        />
      </BsReactstrap.ModalFooter>
    </BsReactstrap.Modal>,
};