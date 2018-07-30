open ReactUtils;
type state = {language: string};
type action =
  | LoadConfig
  | ConfigLoaded(Config.App.t);
let component = ReasonReact.reducerComponent("ClosedOrderInfo");

let valueFromEvent = evt : string => (
                                       evt
                                       |> ReactEventRe.Form.target
                                       |> ReactDOMRe.domElementToObj
                                     )##value;

let make = (~order: Order.orderVm, ~paidDateChanged, _children) => {
  ...component,
  initialState: () => {language: "EN"},
  didMount: self => {
    self.send(LoadConfig);
    ();
  },
  reducer: (action, _state) =>
    switch (action) {
    | LoadConfig =>
      ReasonReact.SideEffects(
        (
          self => {
            let cfg = Config.App.get();
            Js.log(cfg);
            self.send(ConfigLoaded(cfg));
          }
        ),
      )
    | ConfigLoaded(config) =>
      ReasonReact.Update({language: config.language |> Js.String.toLowerCase})
    },
  render: self =>
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
                  (
                    switch (order.returned) {
                    | None =>
                      <Datetime
                        locale=self.state.language
                        value=(paid.on |> Js.Date.fromFloat)
                        onChange=(
                          moment =>
                            paidDateChanged(moment |> MomentRe.Moment.valueOf)
                        )
                      />
                    | Some(_) => s(paid.on |> Date.toDisplay)
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
                <td> (s(returned.on |> Date.toDisplay)) </td>
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