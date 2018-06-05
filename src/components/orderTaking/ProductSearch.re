let component = ReasonReact.statelessComponent("ProductSearch");

let make = (~allProducts: list(Product.t), ~productFound, _children) => {
  ...component,
  render: _self => {
    let findProduct = productName => {
      let matches =
        allProducts |> List.filter((p: Product.t) => p.name === productName);
      if (matches |> List.length > 0) {
        productFound(matches |. List.nth(0));
      };
    };
    <KeyInput
      className="product-search"
      onFinish=(productName => findProduct(productName))
    />;
  },
};