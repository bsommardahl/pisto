let component = ReasonReact.statelessComponent("OrderList");

let headerRow =
  <tr>
    <th />
    <th> (ReactUtils.s("order.customerName" |> Lang.translate)) </th>
    <th> (ReactUtils.s("order.subTotal" |> Lang.translate)) </th>
    <th> (ReactUtils.s("order.tax" |> Lang.translate)) </th>
    <th> (ReactUtils.s("order.total" |> Lang.translate)) </th>
    <th> (ReactUtils.s("order.paidDate" |> Lang.translate)) </th>
    <th> (ReactUtils.s("order.paidTime" |> Lang.translate)) </th>
  </tr>;

let row = (o: Order.orderVm, onSelect) => {
  let totals = o.orderItems |> OrderItemCalculation.getTotals(o.discounts);
  let paidOn =
    switch (o.paid) {
    | Some(paid) => paid.on
    | None => 0.0
    };
  <tr>
    <td>
      <Button
        local=true
        className="card"
        onClick=((_) => onSelect(o))
        label="allOrders.viewOrder"
      />
    </td>
    <td> (ReactUtils.s(o.customerName)) </td>
    <td> (ReactUtils.s(totals.subTotal |> Money.toDisplay)) </td>
    <td> (ReactUtils.s(totals.tax |> Money.toDisplay)) </td>
    <td> (ReactUtils.s(totals.total |> Money.toDisplay)) </td>
    <td> (ReactUtils.s(paidOn |> Date.toDisplayDate)) </td>
    <td> (ReactUtils.s(paidOn |> Date.toDisplayTime)) </td>
  </tr>;
};

let make = (~orders: list(Order.orderVm), ~onSelect, _children) => {
  ...component,
  render: _self =>
    <table className="order-list">
      <thead> headerRow </thead>
      <tbody>
        (
          orders
          |> List.sort((a: Order.orderVm, b: Order.orderVm) => {
               let getPaidDate = (p: option(Paid.t)) =>
                 switch (p) {
                 | None => 0.0
                 | Some(paid) => paid.on
                 };
               getPaidDate(b.paid) -. getPaidDate(a.paid) |> int_of_float;
             })
          |> List.map(o => row(o, onSelect))
          |> Array.of_list
          |> ReasonReact.arrayToElement
        )
      </tbody>
    </table>,
};