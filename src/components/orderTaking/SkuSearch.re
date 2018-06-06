let component = ReasonReact.statelessComponent("SkuSearch");

let make =
    (
      ~acceptInput=true,
      ~allProducts: list(Product.t),
      ~productFound,
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
      acceptInput
      className="sku-search"
      onFinish=(sku => findProduct(sku))
    />;
  },
};