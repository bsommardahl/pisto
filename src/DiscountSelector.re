open Util;

let component = ReasonReact.statelessComponent("DiscountSelector");

let make = (~discounts: list(Discount.t), ~selected, _children) => {
  ...component,
  render: _self =>
    discounts
    |> List.map((d: Discount.t) =>
         <div onClick=((_) => selected(d)) className="card card-discount">
           (s(d.name))
         </div>
       )
    |> Array.of_list
    |> ReasonReact.arrayToElement,
};