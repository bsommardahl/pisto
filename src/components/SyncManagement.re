open Util;

[@bs.val] external protocol : string = "window.location.protocol";

[@bs.val] external host : string = "window.location.host";

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

let component = ReasonReact.reducerComponent("SyncManagement");

let make = _children => {
  ...component,
  didMount: self => {
    let queryString = ReasonReact.Router.dangerouslyGetInitialUrl().search;
    let urlConfig: option(string) = Util.QueryParam.get("cfg", queryString);
    let csv =
      switch (urlConfig) {
      | None => Config.getValue(Config.Database.syncKey)
      | Some(cfg) => Some(cfg)
      };
    switch (csv) {
    | None => Js.log("no sync config yet")
    | Some(configDelimited) =>
      let config = configDelimited |> Config.Database.toSyncConnectionConfig;
      self.send(LoadConfig(config));
    };
    ReasonReact.NoUpdate;
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
    let goBack = (_) => ReasonReact.Router.push("/admin");
    let getVal = ev => ReactDOMRe.domElementToObj(
                         ReactEventRe.Form.target(ev),
                       )##value;
    let thisPageUrl = () => {
      let url = ReasonReact.Router.dangerouslyGetInitialUrl();
      let joinStrings = l => l |> Array.of_list |> Js.Array.joinWith("/");
      let host: string = protocol ++ "//" ++ host ++ "/";
      host ++ (url.path |> joinStrings);
    };
    let config = () : Config.Database.syncConnectionConfig => {
      host: self.state.host,
      options: {
        "auth": {
          "username": self.state.username,
          "password": self.state.password,
        },
      },
      syncOptions: {
        "live": true,
        "retry": true,
      },
    };
    let saveChanges = () => {
      Config.setValue(
        Config.Database.syncKey,
        config() |> Config.Database.toString,
      );
      self.send(LoadConfig(config()));
      ReasonReact.Router.push("sync");
    };
    <div className="admin-menu">
      <div className="header">
        <div className="header-menu">
          <div className="card wide-card quiet-card" onClick=goBack>
            (s("Atras"))
          </div>
        </div>
        <div className="header-options">
          (s("Gestion de Configuracion"))
        </div>
      </div>
      <div className="config-management">
        <h2> (s("Remote Sync")) </h2>
        <table>
          <tr>
            <th> (s("Host")) </th>
            <td>
              <input
                value=self.state.host
                onChange=(ev => self.send(RemoteHostChanged(getVal(ev))))
              />
            </td>
          </tr>
          <tr>
            <th> (s("Username")) </th>
            <td>
              <input
                value=self.state.username
                onChange=(
                  ev => self.send(RemoteUsernameChanged(getVal(ev)))
                )
              />
            </td>
          </tr>
          <tr>
            <th> (s("Password")) </th>
            <td>
              <input
                value=self.state.password
                onChange=(
                  ev => self.send(RemotePasswordChanged(getVal(ev)))
                )
              />
            </td>
          </tr>
        </table>
        <button className="card" onClick=((_) => saveChanges())>
          (s("Guardar"))
        </button>
      </div>
      <div className="sharing no-float">
        <h4> (s("Compartir")) </h4>
        <input
          value=(
            thisPageUrl() ++ "?cfg=" ++ (config() |> Config.Database.toString)
          )
          onChange=((_) => ())
        />
      </div>
    </div>;
  },
};