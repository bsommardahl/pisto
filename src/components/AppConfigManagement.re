type state = {
  language: string,
  deviceId: string,
  now: option(Date.t),
};

type action =
  | LoadConfig
  | ConfigLoaded(Config.App.t)
  | SaveConfig
  | DeviceIdChanged(string)
  | LanguageChanged(string)
  | NowChanged(option(Date.t));

let component = ReasonReact.reducerComponent("SyncManagement");

let make = _children => {
  ...component,
  initialState: () => {language: "EN", deviceId: "", now: None},
  didMount: self => {
    self.send(LoadConfig);
    ReasonReact.NoUpdate;
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
    | ConfigLoaded(config) =>
      ReasonReact.Update({
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
    | NowChanged(newVal) => ReasonReact.Update({...state, now: newVal})
    | DeviceIdChanged(newVal) =>
      ReasonReact.Update({...state, deviceId: newVal})
    },
  render: self => {
    let getVal = ev => ReactDOMRe.domElementToObj(
                         ReactEventRe.Form.target(ev),
                       )##value;
    <div className="config-management">
      <h2> (ReactUtils.sloc("admin.config.application.header")) </h2>
      <table>
        <tr>
          <th> (ReactUtils.sloc("admin.config.application.language")) </th>
          <td>
            <input
              value=self.state.language
              onChange=(ev => self.send(LanguageChanged(getVal(ev))))
            />
          </td>
        </tr>
        <tr>
          <th> (ReactUtils.sloc("admin.config.application.deviceId")) </th>
          <td>
            <input
              value=self.state.deviceId
              onChange=(ev => self.send(DeviceIdChanged(getVal(ev))))
            />
          </td>
        </tr>
        <tr>
          <th> (ReactUtils.sloc("admin.config.application.now")) </th>
          <td>
            (
              switch (self.state.now) {
              | None =>
                <Button
                  local=true
                  label="admin.config.application.now.set"
                  onClick=((_) => self.send(NowChanged(Some(Date.now()))))
                />
              | Some(d) =>
                <div>
                  <span>
                    <DateInput
                      value=d
                      onChange=(newD => self.send(NowChanged(Some(newD))))
                    />
                  </span>
                  <Button
                    local=true
                    label="admin.config.application.now.default"
                    onClick=((_) => self.send(NowChanged(None)))
                  />
                </div>
              }
            )
          </td>
        </tr>
      </table>
      <Button
        onClick=((_) => self.send(SaveConfig))
        local=true
        label="action.save"
      />
    </div>;
  },
};