open Js.Promise;

type state = {webhooks: list(Webhook.t)};

type action =
  | LoadWebhooks(list(Webhook.t))
  | WebhookRemoved(Webhook.t)
  | NewWebhookCreated(Webhook.t);

let component = ReasonReact.reducerComponent("WebhookManagement");

let make = _children => {
  ...component,
  didMount: self => {
    WebhookStore.getAll()
    |> Js.Promise.then_(webhooks => {
         self.send(LoadWebhooks(webhooks));
         Js.Promise.resolve();
       })
    |> ignore;
    ReasonReact.NoUpdate;
  },
  initialState: () => {webhooks: []},
  reducer: (action, state) =>
    switch (action) {
    | LoadWebhooks(webhooks) => ReasonReact.Update({webhooks: webhooks})
    | WebhookRemoved(dis) =>
      ReasonReact.Update({
        webhooks:
          state.webhooks |> List.filter((d: Webhook.t) => d.id !== dis.id),
      })
    | NewWebhookCreated(dis) =>
      ReasonReact.Update({webhooks: List.concat([state.webhooks, [dis]])})
    },
  render: self => {
    let goBack = (_) => ReasonReact.Router.push("/admin");
    let removeWebhook = (p: Webhook.t) => {
      WebhookStore.remove(p.id)
      |> then_((_) => {
           self.send(WebhookRemoved(p));
           resolve();
         })
      |> ignore;
      ();
    };
    let createWebhook = (newWebhook: Webhook.New.t) => {
      WebhookStore.add(newWebhook)
      |> Js.Promise.then_((newWebhook: Webhook.t) => {
           self.send(NewWebhookCreated(newWebhook));
           Js.Promise.resolve();
         })
      |> ignore;
      ();
    };
    <div className="admin-menu">
      <div className="header">
        <div className="header-menu">
          <div className="card wide-card quiet-card" onClick=goBack>
            (ReactUtils.s("Atras"))
          </div>
        </div>
        <div className="header-options">
          (ReactUtils.s("Gestion de Webhooks"))
        </div>
      </div>
      <div className="webhook-management">
        <table className="table">
          <thead>
            <tr>
              <th> (ReactUtils.s("Nombre")) </th>
              <th> (ReactUtils.s("Url")) </th>
              <th> (ReactUtils.s("Evento")) </th>
              <th> (ReactUtils.s("Fuente")) </th>
              <th />
            </tr>
          </thead>
          <tbody>
            (
              self.state.webhooks
              |> List.map((d: Webhook.t) =>
                   <WebhookManagementRow
                     webhook=d
                     remove=removeWebhook
                     key=d.id
                   />
                 )
              |> Array.of_list
              |> ReasonReact.arrayToElement
            )
          </tbody>
          <tfoot> <WebhookManagementNew create=createWebhook /> </tfoot>
        </table>
      </div>
    </div>;
  },
};