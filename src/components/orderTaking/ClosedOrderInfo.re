open ReactUtils;

let component = ReasonReact.statelessComponent("ClosedOrderInfo");
let language = Config.App.get().language;
let valueFromEvent = evt : string => (
                                       evt
                                       |> ReactEventRe.Form.target
                                       |> ReactDOMRe.domElementToObj
                                     )##value;

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
            <td>
              (
                language === "EN" ?
                  s(order.createdOn |> Date.toDisplayEN) :
                  s(order.createdOn |> Date.toDisplay)
              )
            </td>
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
                  (
                    switch (order.returned) {
                    | None =>
                      <Datetime
                        locale=(language |> Js.String.toLowerCase)
                        value=(paid.on |> Js.Date.fromFloat)
                        onChange=(
                          moment =>
                            paidDateChanged(moment |> MomentRe.Moment.valueOf)
                        )
                      />
                    | Some(_) =>
                      language === "EN" ?
                        s(paid.on |> Date.toDisplayEN) :
                        s(paid.on |> Date.toDisplay)
                    }
                  )
                </td>
              </tr>
              <tr>
                <th> (sloc("order.paid.by")) </th>
                <td> (s(paid.by)) </td>
              </tr>
              <tr>
                <th> (sloc("paymentMethod")) </th>
                <td> (sloc(paid.method.name)) </td>
              </tr>
              (
                switch (paid.externalId) {
                | "" => ReasonReact.null
                | id =>
                  <tr>
                    <th> (sloc(paid.method.name ++ ".externalId")) </th>
                    <td> (s(id)) </td>
                  </tr>
                }
              )
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
                <td>
                  (
                    language === "EN" ?
                      s(returned.on |> Date.toDisplayEN) :
                      s(returned.on |> Date.toDisplay)
                  )
                </td>
              </tr>
              <tr>
                <th> (sloc("order.returned.by")) </th>
                <td> (s(returned.by)) </td>
              </tr>
            </tbody>
          }
        )
      </table>
      <Button
        local=true
        onClick=(
          _ =>
            WebhookEngine.getWebhooks(ReprintReceipt, Order)
            |> WebhookEngine.fire(order |> Order.fromVm |> Order.toJs)
            |> ignore
        )
        label="order.printReceiptCopy"
      />
    </div>,
};