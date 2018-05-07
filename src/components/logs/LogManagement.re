open Js.Promise;

type state = {logs: list(Log.t)};

type action =
  | LoadLogs(list(Log.t));

let component = ReasonReact.reducerComponent("LogManagement");

let make = _children => {
  ...component,
  didMount: self => {
    LogStore.getLogs(Date.now() |> Date.oneMonthBefore, Date.now(), 1)
    |> then_(logs => {
         self.send(LoadLogs(logs));
         resolve();
       })
    |> ignore;
    ReasonReact.NoUpdate;
  },
  initialState: () => {logs: []},
  reducer: (action, _state) =>
    switch (action) {
    | LoadLogs(logs) => ReasonReact.Update({logs: logs})
    },
  render: self => {
    let goBack = (_) => ReasonReact.Router.push("/admin");
    <div className="admin-menu">
      <div className="header">
        <div className="header-menu">
          <Button
            local=true
            className="wide-card quiet-card"
            onClick=goBack
            label="nav.back"
          />
        </div>
        <div className="header-options">
          (ReactUtils.sloc("admin.logs.header"))
        </div>
      </div>
      <div className="log-management">
        <table className="table">
          <thead>
            <tr>
              <th> (ReactUtils.sloc("log.date")) </th>
              <th> (ReactUtils.sloc("log.type")) </th>
              <th> (ReactUtils.sloc("log.level")) </th>
              <th> (ReactUtils.sloc("log.message")) </th>
              <th> (ReactUtils.sloc("log.payload")) </th>
            </tr>
          </thead>
          <tbody>
            (
              self.state.logs
              |> List.map((d: Log.t) => <LogManagementRow log=d key=d.id />)
              |> Array.of_list
              |> ReasonReact.arrayToElement
            )
          </tbody>
        </table>
      </div>
    </div>;
  },
};