open Util;

let component = ReasonReact.statelessComponent("ProductCard");

let make = (~product: Product.t, ~onSelect, _children) => {
  ...component,
  render: _self =>
    <div onClick=((_) => onSelect(product)) id="product-card">
      (s(product.name))
    </div>,
};