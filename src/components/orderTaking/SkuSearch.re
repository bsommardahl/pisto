let component = ReasonReact.statelessComponent("SkuSearch");

let make =
    (
      ~allProducts: list(Product.t),
      ~productFound,
      ~maintainFocus=true,
      _children,
    ) => {
  ...component,
  render: _self => {
    let findProduct = sku => {
      let matches =
        allProducts |> List.filter((p: Product.t) => p.sku === sku);
      if (matches |> List.length > 0) {
        productFound(matches |. List.nth(0));
      };
    };
    <KeyInput
      autoFocus=maintainFocus
      maintainFocus
      className="sku-search"
      onFinish=(sku => findProduct(sku))
    />;
  },
};