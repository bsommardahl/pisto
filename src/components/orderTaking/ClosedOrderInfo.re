open ReactUtils;

let component = ReasonReact.statelessComponent("ClosedOrderInfo");

let make = (~order: Order.orderVm, ~paidDateChanged, _children) => {
  ...component,
  render: _self =>
    <div className="paid-date">
      <table>
        <tbody>
          <tr>
            <td colSpan=2> <h2> (sloc("order.created.header")) </h2> </td>
          </tr>
          <tr>
            <th> (sloc("order.created.date")) </th>
            <td> (s(order.createdOn |> Date.toDisplay)) </td>
          </tr>
        </tbody>
        (
          switch (order.paid) {
          | None => <tbody />
          | Some(paid) =>
            <tbody>
              <tr>
                <td colSpan=2> <h2> (sloc("order.paid.header")) </h2> </td>
              </tr>
              <tr>
                <th> (sloc("order.paid.date")) </th>
                <td>
                  <EditableDate date=paid.on onChange=paidDateChanged />
                </td>
              </tr>
              <tr>
                <th> (sloc("order.paid.by")) </th>
                <td> (s(paid.by)) </td>
              </tr>
            </tbody>
          }
        )
        (
          switch (order.returned) {
          | None => <tbody />
          | Some(returned) =>
            <tbody>
              <tr>
                <td colSpan=2>
                  <h2> (sloc("order.returned.header")) </h2>
                </td>
              </tr>
              <tr>
                <th> (sloc("order.returned.date")) </th>
                <td> (s(returned.on |> Date.toDisplay)) </td>
              </tr>
              <tr>
                <th> (s("order.returned.by")) </th>
                <td> (s(returned.by)) </td>
              </tr>
            </tbody>
          }
        )
      </table>
      <Button
        local=true
        onClick=(
          (_) =>
            order
            |> Order.fromVm
            |> WebhookEngine.fireForOrder(ReprintReceipt)
            |> Most.observe((response: Order.t) => Js.log(response))
            |> ignore
        )
        label="order.printReceiptCopy"
      />
    </div>,
};