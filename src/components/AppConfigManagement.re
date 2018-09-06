open ReactUtils;

type state = {
  language: string,
  deviceId: string,
  now: option(Date.t),
  editingDate: bool,
};

type action =
  | LoadConfig
  | ShowDateTime
  | ConfigLoaded(Config.App.t)
  | SaveConfig
  | DeviceIdChanged(string)
  | LanguageChanged(string)
  | NowChanged(option(Date.t));

let language = Config.App.get().language;
let component = ReasonReact.reducerComponent("SyncManagement");
let make = _children => {
  ...component,
  initialState: () => {
    language: "EN",
    deviceId: "",
    now: None,
    editingDate: false,
  },
  didMount: self => {
    self.send(LoadConfig);
    ();
  },
  reducer: (action, state) =>
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
    | ShowDateTime => ReasonReact.Update({...state, editingDate: true})
    | ConfigLoaded(config) =>
      ReasonReact.Update({
        ...state,
        language: config.language,
        deviceId: config.deviceId,
        now: config.now,
      })
    | SaveConfig =>
      ReasonReact.SideEffects(
        (
          _self =>
            Config.App.set({
              language: state.language,
              deviceId: state.deviceId,
              now: state.now,
            })
        ),
      )
    | LanguageChanged(newVal) =>
      ReasonReact.Update({...state, language: newVal})
    | NowChanged(newVal) =>
      ReasonReact.Update({...state, editingDate: false, now: newVal})
    | DeviceIdChanged(newVal) =>
      ReasonReact.Update({...state, deviceId: newVal})
    },
  render: self => {
    let handleDateChange = moment =>
      self.send(NowChanged(Some(moment |> MomentRe.Moment.valueOf)));
    <div className="config-management">
      <h2> {ReactUtils.sloc("admin.config.application.header")} </h2>
      <table>
        <tr>
          <th> {ReactUtils.sloc("admin.config.application.language")} </th>
          <td>
            <input
              value={self.state.language}
              onChange={ev => self.send(LanguageChanged(getVal(ev)))}
            />
          </td>
        </tr>
        <tr>
          <th> {ReactUtils.sloc("admin.config.application.deviceId")} </th>
          <td>
            <input
              value={self.state.deviceId}
              onChange={ev => self.send(DeviceIdChanged(getVal(ev)))}
            />
          </td>
        </tr>
        <tr>
          <th> {ReactUtils.sloc("admin.config.application.now")} </th>
          <td>
            {
              switch (self.state.now) {
              | None =>
                <Button
                  local=true
                  label="admin.config.application.now.set"
                  onClick=(_ => self.send(NowChanged(Some(Date.now()))))
                />
              | Some(d) =>
                <div>
                  {
                    if (self.state.editingDate) {
                      <Datetime
                        className="date"
                        value={d |> Js.Date.fromFloat}
                        locale={language |> Js.String.toLowerCase}
                        timeFormat=false
                        input=false
                        onChange=handleDateChange
                      />;
                    } else {
                      <input
                        type_="text"
                        value={
                          language === "ES" ?
                            d |> Date.toDisplayDate : d |> Date.toDisplayDateEN
                        }
                        onClick=(_ => self.send(ShowDateTime))
                      />;
                    }
                  }
                  <Button
                    local=true
                    label="admin.config.application.now.default"
                    onClick=(_ => self.send(NowChanged(None)))
                  />
                </div>
              }
            }
          </td>
        </tr>
      </table>
      <Button
        onClick={_ => self.send(SaveConfig)}
        local=true
        label="action.save"
      />
    </div>;
  },
};