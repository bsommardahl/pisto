let component = ReasonReact.statelessComponent("Admin");

let make = _children => {
  ...component,
  render: _self => {
    let go = (u: string) : unit => ReasonReact.Router.push(u);
    <div className="admin-menu">
      <div className="header">
        <div className="header-options">
          (ReactUtils.s("Gestion del Programa"))
        </div>
      </div>
      <div className="admin-menu-buttons">
        <div className="card" onClick=((_) => go("logs"))>
          (ReactUtils.s("Logs"))
        </div>
        <div className="card" onClick=((_) => go("products"))>
          (ReactUtils.s("Productos"))
        </div>
        <div className="card" onClick=((_) => go("expenseTypes"))>
          (ReactUtils.s("Typos de Gastos"))
        </div>
        <div className="card" onClick=((_) => go("vendors"))>
          (ReactUtils.s("Vendedores"))
        </div>
        <div className="card" onClick=((_) => go("sync"))>
          (ReactUtils.s("Syncronizacion"))
        </div>
      </div>
    </div>;
  },
};