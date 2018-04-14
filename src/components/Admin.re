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
        <div className="card quiet-card" onClick=((_) => go("/"))>
          (ReactUtils.s("Atras"))
        </div>
        <div className="card" onClick=((_) => go("products"))>
          (ReactUtils.s("Productos"))
        </div>
        <div className="card" onClick=((_) => go("discounts"))>
          (ReactUtils.s("Descuentos"))
        </div>
        <div className="card" onClick=((_) => go("sync"))>
          (ReactUtils.s("Syncronizacion"))
        </div>
      </div>
    </div>;
    /* <div className="card" onClick=((_) => go("webhooks"))>
         (ReactUtils.s("Webhooks"))
       </div> */
    /* <div className="card" onClick=((_) => go("users"))>
         (ReactUtils.s("Usuarios"))
       </div> */
  },
};