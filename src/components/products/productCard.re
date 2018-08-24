let component = ReasonReact.statelessComponent("ProductCard");

let make = (~product: Product.t, ~onSelect, _children) => {
  ...component,
  render: _self =>
    <Button
      onClick=((_) => onSelect(product))
      className="product-card"
      label=product.name
      subLabel=product.sku />,
};