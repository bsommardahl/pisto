open Util;

let component = ReasonReact.statelessComponent("Admin");

let make = _children => {
  ...component,
  render: _self => {
    let go = (u: string) : unit => ReasonReact.Router.push(u);
    <div className="admin-menu">
      <div className="header">
        <div className="header-options"> (s("Gestion del Programa")) </div>
      </div>
      <div className="admin-menu-buttons">
        <div className="card quiet-card" onClick=((_) => go("/"))>
          (s("Atras"))
        </div>
        <div className="card" onClick=((_) => go("orders"))>
          (s("Ver Ordenes"))
        </div>
        <div className="card" onClick=((_) => go("products"))>
          (s("Productos"))
        </div>
        <div className="card" onClick=((_) => go("discounts"))>
          (s("Descuentos"))
        </div>
      </div>
      <div className="card" onClick=((_) => go("sync"))>
        (s("Syncronizacion"))
      </div>
    </div>;
    /* <div className="card" onClick=((_) => go("webhooks"))>
         (s("Webhooks"))
       </div> */
    /* <div className="card" onClick=((_) => go("users"))>
         (s("Usuarios"))
       </div> */
  },
};