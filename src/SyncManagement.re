open Util;

open Js.Promise;

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
    let csv = Config.getValue(Config.Database.syncKey);
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
    let saveChanges = _ev => {
      let config: Config.Database.syncConnectionConfig = {
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
      Config.setValue(
        Config.Database.syncKey,
        config |> Config.Database.toString,
      );
      self.send(LoadConfig(config));
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
                _type="password"
                value=self.state.password
                onChange=(
                  ev => self.send(RemotePasswordChanged(getVal(ev)))
                )
              />
            </td>
          </tr>
        </table>
        <button className="card" onClick=saveChanges> (s("Guardar")) </button>
      </div>
    </div>;
  },
};