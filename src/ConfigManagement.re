let component = ReasonReact.statelessComponent("ConfigManagement");

let make = _children => {
  ...component,
  render: _self => {
    let goBack = (_) => ReasonReact.Router.push("/admin");
    <div className="admin-menu">
      <div className="header">
        <div className="header-menu">
          <Button
            className="wide-card quiet-card"
            onClick=goBack
            label="nav.back"
            local=true
          />
        </div>
        <div className="header-options">
          (ReactUtils.sloc("admin.config.header"))
        </div>
      </div>
      <SyncManagement />
      <AppConfigManagement />
    </div>;
  },
};