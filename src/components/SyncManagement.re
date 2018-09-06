[@bs.val] external protocol: string = "window.location.protocol";

[@bs.val] external host: string = "window.location.host";

open ReactUtils;

type state = {
  host: string,
  password: string,
  username: string,
};

type action =
  | LoadConfig(Config.Database.syncConnectionConfig)
  | RemoteHostChanged(string)
  | RemoteUsernameChanged(string)
  | RemotePasswordChanged(string);

let saveConfigToLocalStorage = config =>
  LocalStorage.setValue(
    Config.Database.syncKey,
    config |> Config.Database.toString,
  );

let component = ReasonReact.reducerComponent("SyncManagement");

let make = _children => {
  ...component,
  didMount: self => {
    let queryString = ReasonReact.Router.dangerouslyGetInitialUrl().search;
    let urlConfig: option(string) = Util.QueryParam.get("cfg", queryString);
    switch (urlConfig) {
    | Some(cfg) =>
      let config = cfg |> Config.Database.toSyncConnectionConfig;
      saveConfigToLocalStorage(config);
      ReasonReact.Router.push("/");
    | None =>
      switch (LocalStorage.getValue(Config.Database.syncKey)) {
      | Some(delimited) =>
        let config = delimited |> Config.Database.toSyncConnectionConfig;
        self.send(LoadConfig(config));
      | None => ()
      }
    };
    ();
  },
  initialState: () => {host: "", password: "", username: ""},
  reducer: (action, state) =>
    switch (action) {
    | LoadConfig(config) =>
      ReasonReact.Update({
        host: config.host,
        username: config.options##auth##username,
        password: config.options##auth##password,
      })
    | RemoteHostChanged(newVal) =>
      ReasonReact.Update({...state, host: newVal})
    | RemoteUsernameChanged(newVal) =>
      ReasonReact.Update({...state, username: newVal})
    | RemotePasswordChanged(newVal) =>
      ReasonReact.Update({...state, password: newVal})
    },
  render: self => {
    let thisPageUrl = () => {
      let url = ReasonReact.Router.dangerouslyGetInitialUrl();
      let joinStrings = l => l |> Array.of_list |> Js.Array.joinWith("/");
      let host: string = protocol ++ "//" ++ host ++ "/";
      host ++ (url.path |> joinStrings);
    };
    let getConfigFromState = (): Config.Database.syncConnectionConfig => {
      host: self.state.host,
      options: {
        "auth": {
          "username": self.state.username,
          "password": self.state.password,
        },
        "auto_compaction": true,
      },
      syncOptions: {
        "live": true,
        "retry": true,
      },
    };
    let saveChanges = config => {
      saveConfigToLocalStorage(config);
      self.send(LoadConfig(getConfigFromState()));
      ReasonReact.Router.push("sync");
    };
    <div className="config-management">
      <h2> {ReactUtils.sloc("admin.config.remote.header")} </h2>
      <table>
        <tr>
          <th> {ReactUtils.sloc("admin.config.remote.host")} </th>
          <td>
            <input
              value={self.state.host}
              onChange={ev => self.send(RemoteHostChanged(getVal(ev)))}
            />
          </td>
        </tr>
        <tr>
          <th> {ReactUtils.sloc("admin.config.remote.username")} </th>
          <td>
            <input
              value={self.state.username}
              onChange={ev => self.send(RemoteUsernameChanged(getVal(ev)))}
            />
          </td>
        </tr>
        <tr>
          <th> {ReactUtils.sloc("admin.config.remote.password")} </th>
          <td>
            <input
              value={self.state.password}
              onChange={ev => self.send(RemotePasswordChanged(getVal(ev)))}
            />
          </td>
        </tr>
      </table>
      <Button
        local=true
        onClick={_ => saveChanges(getConfigFromState())}
        label="action.save"
      />
      <div className="sharing no-float">
        <h4> {ReactUtils.sloc("admin.config.remote.share")} </h4>
        <input
          value={
            thisPageUrl()
            ++ "?cfg="
            ++ (getConfigFromState() |> Config.Database.toString)
          }
          onChange={_ => ()}
        />
      </div>
    </div>;
  },
};
