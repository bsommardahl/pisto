let component = ReasonReact.statelessComponent("ConfigManagement");

let make = _children => {
  ...component,
  render: _self => {
    let goBack = (_) => ReasonReact.Router.push("/admin");
    <div className="admin-menu">
      <div className="header">
        <div className="header-menu">
          <div className="card wide-card quiet-card" onClick=goBack>
            (ReactUtils.s("Atras"))
          </div>
        </div>
        <div className="header-options">
          (ReactUtils.s("Gestion de Configuracion"))
        </div>
      </div>
      <SyncManagement />
      <AppConfigManagement />
    </div>;
  },
};