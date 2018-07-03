let component = ReasonReact.statelessComponent("SkuSearch");

let lowercase = text => Js.String.toLowerCase(text);

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
        allProducts
        |> List.filter((p: Product.t) =>
             lowercase(p.sku) === lowercase(sku)
           );
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