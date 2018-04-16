let component = ReasonReact.statelessComponent("WebhookManagementRow");

let make = (~webhook: Webhook.t, ~remove, _children) => {
  ...component,
  render: _self =>
    <tr>
      <td> (ReactUtils.s(webhook.name)) </td>
      <td> (ReactUtils.s(webhook.url)) </td>
      <td> (ReactUtils.s(webhook.event)) </td>
      <td>
        <button onClick=((_) => remove(webhook))>
          (ReactUtils.s("Eliminar"))
        </button>
      </td>
    </tr>,
};