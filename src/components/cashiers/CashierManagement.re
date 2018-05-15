open Js.Promise;

type state = {cashiers: list(Cashier.t)};

type action =
  | LoadCashiers(list(Cashier.t))
  | CashierRemoved(Cashier.t)
  | CashierModified(Cashier.t)
  | NewCashierCreated(Cashier.t);

let component = ReasonReact.reducerComponent("CashierManagement");

let make = _children => {
  ...component,
  didMount: self => {
    CashierStore.getAll()
    |> Js.Promise.then_(cashiers => {
         self.send(LoadCashiers(cashiers));
         Js.Promise.resolve();
       })
    |> ignore;
    ReasonReact.NoUpdate;
  },
  initialState: () => {cashiers: []},
  reducer: (action, state) =>
    switch (action) {
    | LoadCashiers(cashiers) => ReasonReact.Update({cashiers: cashiers})
    | CashierRemoved(dis) =>
      ReasonReact.Update({
        cashiers:
          state.cashiers |> List.filter((d: Cashier.t) => d.id !== dis.id),
      })
    | CashierModified(dis) =>
      ReasonReact.Update({
        cashiers:
          state.cashiers
          |> List.map((d: Cashier.t) => d.id !== dis.id ? dis : d),
      })
    | NewCashierCreated(dis) =>
      ReasonReact.Update({cashiers: List.concat([state.cashiers, [dis]])})
    },
  render: self => {
    let goBack = (_) => ReasonReact.Router.push("/admin");
    let removeCashier = (p: Cashier.t) => {
      CashierStore.remove(p.id)
      |> then_((_) => {
           self.send(CashierRemoved(p));
           resolve();
         })
      |> ignore;
      ();
    };
    let modifyCashier = (modifiedCashier: Cashier.t) =>
      CashierStore.update(modifiedCashier)
      |> then_((_) => {
           self.send(CashierModified(modifiedCashier));
           resolve();
         })
      |> ignore;
    let createCashier = (newCashier: Cashier.New.t) => {
      CashierStore.add(newCashier)
      |> Js.Promise.then_((newCashier: Cashier.t) => {
           self.send(NewCashierCreated(newCashier));
           Js.Promise.resolve();
         })
      |> ignore;
      ();
    };
    <div className="admin-menu">
      <div className="header">
        <div className="header-menu">
          <div className="card wide-card quiet-card" onClick=goBack>
            (ReactUtils.s("Atras"))
          </div>
        </div>
        <div className="header-options">
          (ReactUtils.s("Gestion de Descuentos"))
        </div>
      </div>
      <div className="cashier-management">
        <table className="table">
          <thead>
            <tr>
              <th />
              <th> (ReactUtils.s("Nombre")) </th>
              <th> (ReactUtils.s("Pin")) </th>
            </tr>
          </thead>
          <tbody>
            (
              self.state.cashiers
              |> List.map((d: Cashier.t) =>
                   <CashierManagementRow
                     cashier=d
                     remove=removeCashier
                     modify=modifyCashier
                     key=d.id
                   />
                 )
              |> Array.of_list
              |> ReasonReact.arrayToElement
            )
          </tbody>
          <tfoot> <CashierManagementNew create=createCashier /> </tfoot>
        </table>
      </div>
    </div>;
  },
};