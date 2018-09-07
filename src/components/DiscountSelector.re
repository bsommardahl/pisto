let component = ReasonReact.statelessComponent("DiscountSelector");

let make = (~discounts: list(Discount.t), ~selected, _children) => {
  ...component,
  render: _self =>
    discounts
    |> List.map((d: Discount.t) =>
         <div
           key={d.id}
           onClick={_ => selected(d)}
           className="card small-card card-discount">
           {ReactUtils.s(d.name)}
         </div>
       )
    |> Array.of_list
    |> ReasonReact.array,
};
