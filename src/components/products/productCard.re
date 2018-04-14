let component = ReasonReact.statelessComponent("ProductCard");

let make = (~product: Product.t, ~onSelect, _children) => {
  ...component,
  render: _self =>
    <div onClick=((_) => onSelect(product)) className="product-card card">
      (ReactUtils.s(product.name))
    </div>,
};