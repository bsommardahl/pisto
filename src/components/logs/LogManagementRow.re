let component = ReasonReact.statelessComponent("LogManagementRow");

let make = (~log: Log.t, _children) => {
  ...component,
  render: _self =>
    <tr>
      <td> (ReactUtils.s(log.on |> Date.toDisplay)) </td>
      <td> (ReactUtils.s(log.logType)) </td>
      <td> (ReactUtils.s(log.level |> string_of_int)) </td>
      <td> (ReactUtils.s(log.message)) </td>
      <td> <textarea> (ReactUtils.s(log.payload)) </textarea> </td>
    </tr>,
};