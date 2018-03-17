open Util;

let component = ReasonReact.statelessComponent("ProductCard");

/* Here, the button should push to /NewOrder?customerName=john and the view changes */
let make = (~product: Product.t, ~onSelect, _children) => {
  ...component,
  render: _self =>
    <div onClick=((_) => onSelect(product)) id="product-card">
      (s(product.name))
    </div>,
};